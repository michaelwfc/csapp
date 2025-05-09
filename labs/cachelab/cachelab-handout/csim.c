#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h> // For getopt 
#include <stdint.h>  // For uint64_t
#include <string.h>  // for memcpy
#include "cachelab.h"

#define MEMORY_SIZE 64

/* Always use a 64-bit variable to hold memory addresses*/
typedef long long unsigned int MemAddrType; //%llx same as uint64_t

/* Cache parameters struct */
typedef struct CacheParams{
    unsigned verbose_flag;
    unsigned s; // s bits for set address
    unsigned E; // E line for each set
    unsigned b; // b blocks for each line
    // uint64_t S; // S =2^s sets
    // uint64_t B; // B = 2^b blocks per line
    char *trace_file;     /* valgrind trace file */
} CacheParams;





/* Usage Info */
void csim_help_info()
{
    printf("Usage: ./csim [-hv] -s <num> -E <num> -b <num> -t <tracefile>\n");
    printf("Options:\n");
    printf("  -h              Print this help message.\n");
    printf("  -v              Optional verbose flag.\n");
    printf("  -s <num>        Number of set index bits.\n");
    printf("  -E <num>        Number of lines per set.\n");
    printf("  -b <num>        Number of block offset bits.\n");
    printf("  -t <tracefile>  Trace file.\n");
    printf("\nExamples:\n");
    printf("  linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n");
    printf("  linux>  ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
}

/* Missing arguments error. */
void missing_args_error(char* program_name)
{
    printf("%s: Missing required command line argument\n", program_name);
}

/* Missing input file error. */
void missing_file_error(char *file)
{
    printf("%s: No such file or directory\n", file);
}


/**
 * https://leetcode.com/problems/lru-cache/description/
 * Your LRUCache struct will be instantiated and called as such:
 * LRUCache* obj = lRUCacheCreate(capacity);
 * int param_1 = lRUCacheGet(obj, key);

 * lRUCachePut(obj, key, value);

 * lRUCacheFree(obj);
*/



/**
 * the fields of the LEUCache struct:
 * memories(linked_list):
 *
 * implement the methods:
 * create(capacity)
 * get(LRUCache * cache, unsigned int address): return memory blocks
 * update(LRUCache * cache,unsigned int address):
 * free()
 *
 * @param: char * blocks;    // Pointer to a dynamically allocated char array (string)
 * Memory for the blocks field is allocated dynamically using malloc(): 
 *    blocks = (char *)malloc(b * sizeof(char)); // Allocating memory to blocks
 * Free the dynamically allocated memory:  
 *    free(line_code.blocks);
 */

typedef struct LineNode
{
    unsigned int tag;
    unsigned int valid;
    char *blocks;          // Pointer to a dynamically allocated char array (string)
    struct LineNode *prev; // Pointer to previous node
    struct LineNode *next; // Pointer to next node
} LineNode;


typedef struct CacheSet
{
    unsigned int E;
    unsigned int line_size;
    struct LineNode *head;
    struct LineNode *tail;

} CacheSet;


typedef struct LRUCache
{
    unsigned s;
    unsigned E;
    unsigned b;
    unsigned tag_offset; //s+b
    MemAddrType S;
    MemAddrType B;
    MemAddrType tag_mask;
    MemAddrType set_mask;
    MemAddrType block_mask; // to get the block binary value
    MemAddrType address_mask; // to get the address binary value to copy from memory to cache
    // sets is a dynamic array of set, and set consists of E lines, lines is build by lined list
    // each element in sets is a pointer to CacheSet
    CacheSet** sets; 
} LRUCache;


/*
LRUCache((unsigned s, unsigned E, unsigned b) Initialize the LRU cache with s,E,b

capacity = C= S*E*B bytes
*/
LRUCache *lRUCacheCreate(unsigned s, unsigned E, unsigned b)
{
    
    unsigned tag_offset = s + b;
    unsigned  t = MEMORY_SIZE -s-b;

    // create tag_mask and set_mask by s,E,b,t
    MemAddrType mask = 0xFFFFFFFFFFFFFFFF;
    MemAddrType tag_mask = mask >> (s + b);
    MemAddrType set_mask = (mask << t) >> (b + t);
    MemAddrType block_mask= (mask<<(s+t) ) >>(s+t);
    MemAddrType address_mask= (mask>> b ) <<b;

    MemAddrType S = 0x1 << s;
    MemAddrType B = 0x1 << b;



    // Dynamically allocate memory for the struct on the heap, then we can return the address
    LRUCache *pcache = (LRUCache *)malloc(sizeof(LRUCache));
    
    // LRUCache cache = {s, E, b, tag_offset, S, B,  tag_mask, set_mask, address_mask, sets};
    pcache->s = s;
    pcache->E = E;
    pcache->b = b;
    pcache->tag_offset = tag_offset;
    pcache->S = S;
    pcache->B = B;
    pcache->tag_mask = tag_mask;
    pcache->set_mask = set_mask;
    pcache->block_mask = block_mask;
    pcache->address_mask = address_mask;

    // initialize the sets for cache, Allocating memory to sets
    CacheSet **sets = (CacheSet **)malloc(S * sizeof(CacheSet*));
    pcache->sets = sets;

    // initialize the sets for cache
    for (unsigned i = 0; i < S; i++)
    {   
        // CacheSet * set = sets[i];   // get the cache set pointer
        CacheSet * set = (CacheSet *)malloc(sizeof(CacheSet));
        set->E = E;                 // initialize the cache set
        set->head = NULL;
        set->tail = NULL;
        set->line_size=0;
        pcache->sets[i]= set;       // set the sets with cache set pointer
    }
    return pcache;
}

/**
 * int lookup(unsigned int address) Return the value of the address if the address exists (hit), otherwise return -1 (miss)
 * 1. matched by set + tag
 * 2. if hit, then get the data blocks, here we just return wheater hit or not
    return 0: no hit
    return 1: hit
 *
*/
int lRUCacheLookup(LRUCache *pcache, char operation, MemAddrType address, unsigned size)
{
    // how to matched by set + tag?
    MemAddrType target_set_index = address >> pcache->b & pcache->set_mask;
    MemAddrType target_tag_index = address >> (pcache->tag_offset) & pcache->tag_mask;

    // Use CacheSet pointer to save the cache set at the pointer(memory address), not just use CacheSet(local))
    CacheSet* target_set = pcache->sets[target_set_index];

    // if find the target tag and is valid, which is hit
    int hit=0;

    // find the target tag/line by for loops
    LineNode* hit_line;

    
    LineNode* current_line = target_set->head; 
    for(int i=0; i <= target_set->line_size; i++){
        if(current_line==NULL ){break;}
        else if( current_line->valid && current_line->tag == target_tag_index){
            hit_line = current_line;
            hit = 1;
            // move the hit_line to the head of the cache
            // head ->  prev ->   hit-line -> next -> tail
            //          prev ->       next
            //          prev   <-    next
            // hit-line -> head
            //     hit-line <- head
            if(hit_line!= target_set->head){
                LineNode* temp = hit_line->prev; // Use '->' to access the members of the struct via pointer
                temp->next = hit_line->next; 
                if(hit_line->next!= NULL){
                    hit_line->next->prev = temp;
                }
                hit_line->next = target_set->head;
                target_set->head->prev = hit_line;
                hit_line->prev = NULL;
                target_set->head = hit_line;
            }
            // if hit a line, how to get the data blocks?
            // bytes = hit_line.blocks [offset:offset + size];
            break;
        }else{
            // update the current line by linked list
            current_line = current_line->next;
        }
    }
    // TODO: return the bytes in cache?
    return hit;
}

/*
if missed, update the cache with memeory address:
    locate the catch set by set_tag_index
        if cache set is not full, just add memory to cache line as head 
        if cache set is full, evict by least recently used 
    return 0: add cache line
    return 1: eviction

The functions get and put must each run in O(1) average time complexity.
*/
int lRUCachePut(LRUCache *pcache, char operation, MemAddrType address, unsigned size){

    // if miss , when and how to evict the data blocks and update?
    // if the cache set is full, add a new line to the cache set
    MemAddrType target_set_index = address >> pcache->b & pcache->set_mask;
    MemAddrType target_tag_index = address >> (pcache->tag_offset) & pcache->tag_mask;

    // find the target set
    CacheSet* target_set = pcache->sets[target_set_index];


    // initial a new line node and add new line to head
    LineNode* new_line = (LineNode*)malloc(sizeof(LineNode));
    new_line->tag = target_tag_index;
    new_line->valid = 1;
    new_line->prev = NULL;
    new_line->next = target_set->head;
    if(target_set->head!= NULL){
        target_set->head-> prev = new_line;
    }

    // move the local memory to block in this cache set line
    //To copy B bytes from a specific memory address into a dynamically allocated char array (blocks), you can use the memcpy function from the C standard library. Here's how to do it step by step:
    // Steps:
    // Allocate memory for the blocks array using malloc.
    // Use memcpy to copy B bytes from the source address to the dynamically allocated memory (blocks).
    // each block is a byte, blocks= B=2^b bytes
    // new_line->blocks = (char*)malloc(cache->B * sizeof(char)); // Allocate memory to blocks

    // MemAddrType memory_addr = address &(cache->address_mask);
    // Cast the 64-bit integer to a pointer to a char (or another data type you need)
    // char *pmemory_addr = (char*)memory_addr;  // Explicitly cast the 64-bit integer to a pointer
    // TODO:access= 16=1 0000 = memory_addr  can not access the memory address at adress 0x10
    // memcpy(new_line->blocks, pmemory_addr,cache->B);
    
    //when i reach the end, evict an existing line node
    //will replace the line that was last accessed the furthest in the past. 
    int eviction=0;
    if(target_set->line_size<(target_set->E)){
        // if the target_set is not full, set the new line as head node
        // set the head of the set
        target_set->head = new_line;           // add the new line node to the head of the cach
        
        // how to set the tail of the set: if tail is null,update , if not no need to do anything
        // which will be used in evction by tail with O(1) 
        if(target_set->tail==NULL){target_set->tail= new_line->next;}

        target_set->line_size++;
    }else{   
        // if the target_set is full
        eviction =1;
        if(target_set->E==1){
            // if the cache set has only one line, just remove it and add the new line
            LineNode* evicted_line = target_set->head;
            // free(evicted_line->blocks);
            free(evicted_line);
            new_line->next = NULL; // set the next as null
            target_set->head = new_line;
        }
        else{
            // set th tail of set
            LineNode* evicted_line = target_set->tail;
            if(evicted_line->prev!= NULL){
                evicted_line->prev->next = NULL;   // set the previous line node
            }
            target_set->tail = evicted_line->prev; // set the tail to the previous line node
            
            // free(evicted_line->blocks);           // free the memory allocated for blocks
            free(evicted_line);                   // free the memory allocated for line node
            target_set->head = new_line;
        }



    }
    return eviction;
}



void lRUCacheFree(LRUCache *pcache)
{
    for(int i=0 ; i<pcache->S;  i++){
        CacheSet* cache_set = pcache->sets[i];
        LineNode* current_line = cache_set->head;
        while(current_line){
            LineNode* next_line = current_line->next;
            free(current_line);
            current_line = next_line;
        }
        free(cache_set);
        
    }
    free(pcache->sets);
    free(pcache);
}


void run_cache_simulator(CacheParams cache_params){
    
    // initialize hits, misses, evictions
    int hits = 0;
    int misses = 0;
    int evictions = 0;

    // initialize LRU cache
    LRUCache *pcache = lRUCacheCreate(cache_params.s, cache_params.E, cache_params.b);

    // open the trace file
    FILE *ptrace_file = fopen(cache_params.trace_file, "r");
    if(ptrace_file == NULL){
        missing_file_error(cache_params.trace_file);
        exit(EXIT_FAILURE);
    }

    // Reading lines like " M 20,1" or "L 19,3"
    char operation;
    MemAddrType address;
    unsigned size;

    while(fscanf(ptrace_file," %c %llx,%d", &operation, &address, &size) > 0){
        
        int hit = 0;  // initial the hit/eviction value
        int eviction=0;
        /*
        error: comparison between pointer and integer [-Werror]
        In C, a string literal like "I" is a pointer to a character array (i.e., a char*), while operation is a char variable, which is just a single character. Comparing a char with a char* results in the error "comparison between pointer and integer."
        How to Fix It
        You should compare the char variable operation with a character literal ('I'), not a string literal ("I").
        */
        if(operation =='I'){continue;} 
        else if(operation=='L'){
            /*
            1. cache lookup
            2. if hit, Handling Cache Hits
                load data from  block in cache line (ingore for the lab)
            3. if miss, Handling cache misses
                if cache_set is not full:
                if cache_set is full: do eviction
            */

            int hit = lRUCacheLookup(pcache, operation, address, size);
            if(hit==1){
                hits++;
                if(cache_params.verbose_flag)printf("%c %llx,%d hit\n", operation, address, size );
            }
            else{
                misses++;
                eviction =  lRUCachePut(pcache, operation, address, size);
                if(eviction){
                    evictions++;
                    if(cache_params.verbose_flag)printf("%c %llx,%d miss eviction\n", operation, address, size);
                }else{
                    if(cache_params.verbose_flag)printf("%c %llx,%d miss\n", operation, address, size);
                }

            }
        }
        else if(operation=='S'){
            /*
            1. cache lookup
            2. if hit, Handling Cache Hits
                write to memory: Write-through Cache or Write-back Cache
            3. if miss, Handling Cache Misses
                Update the cache with the new data from the store operation
                - if cache is not full, 
                - if cache is full, do Eviction:
            */
            hit = lRUCacheLookup(pcache, operation, address, size);
            if(hit==1){
                hits++;
                if(cache_params.verbose_flag)printf("%c %llx,%d hit\n", operation, address, size );
            }
            else{
                misses++;
                eviction =  lRUCachePut(pcache, operation, address, size);
                if(eviction){
                    evictions++;
                    if(cache_params.verbose_flag)printf("%c %llx,%d miss eviction\n", operation, address, size);
                }else{
                    if(cache_params.verbose_flag)printf("%c %llx,%d miss\n", operation, address, size);
                }

            }
            }
        else if(operation=='M'){
            /*
            1. data load : lookup + put
            2. data store: lookup + put
            */

            int load_hit = lRUCacheLookup(pcache, operation, address, size);
            if(load_hit==1){
                hits++;
                if(cache_params.verbose_flag)printf("%c %llx,%d hit ", operation, address, size);
                }
            else{
                misses++;
                int load_eviction = lRUCachePut(pcache, operation, address, size);
                
                if(load_eviction){
                    evictions++;
                    if(cache_params.verbose_flag)printf("%c %llx,%d miss eviction ", operation, address, size);
                }else{
                    if(cache_params.verbose_flag)printf("%c %llx,%d miss ", operation, address, size);
                }
                }

            
            int store_hit = lRUCacheLookup(pcache, operation, address, size);
            if(store_hit==1){
                hits++;
                if(cache_params.verbose_flag)printf("hit\n");
                }
            else{
                misses++;
                int store_eviction = lRUCachePut(pcache, operation, address, size);
                if(store_eviction){
                    evictions++;
                    if(cache_params.verbose_flag)printf("miss eviction\n");
                }else{
                    if(cache_params.verbose_flag)printf("miss\n");
                }
                }
        }
    }

    // close the trace file
    fclose(ptrace_file);

    // print the summary
    printSummary(hits, misses, evictions);

    // free cache
    lRUCacheFree(pcache);

}


/* Read in command line arguments with getopt()
 *
 * `getopt()` to handle two program options:
 *   n: with no associated value.
 *   t: which expects an associated value.
 *
 * `atoi()` is a function in C that converts a string into an integer numerical representation.
 */
CacheParams parseOptions(int argc, char **argv)
{   
    CacheParams cache_params;
    // Declare variables
    char *program_name = NULL;
    int opt;
    int help_flag = 0,  verbose_flag=0, s_flag = 0, E_flag = 0, b_flag = 0, t_flag = 0;
    program_name = argv[0];
    // getopt loop, "a:b:c" means option 'a' takes an argument, 'b' takes an argument, 'c' has no argument
    while ((opt = getopt(argc, argv, "hvs:E:b:t:"))!=-1){
        switch (opt){  
        case 'h': // Optional help flag that prints usage info
            help_flag = 1;
            break;
        case 'v': // Optional verbose flag that displays trace info
            verbose_flag = 1;
            cache_params.verbose_flag =verbose_flag;
            break;
        
        case 's': // Number of set index bits (S = 2^s is the number of sets)
            s_flag = 1;
            cache_params.s = atoi(optarg);
            break;
        case 'E':  // Associativity (number of lines per set)
            E_flag = 1;
            cache_params.E = atoi(optarg);
            break;
        case 'b': // Number of block bits (B = 2^b is the block size)
            b_flag = 1;
            cache_params.b = atoi(optarg);
            break;
        // Name of the valgrind trace to replay
        case 't':
            t_flag = 1;
            cache_params.trace_file = optarg;
            break;
        // No command line arguments, print usage info and exit
        default:
            csim_help_info();
            // return cache_params;
        }
    }

    /* If you set `-h` argument, program will print usage info, then terminate.
     * It dooe not care the remaining arguments right or false.
     */
    if (help_flag == 1)
    {
        csim_help_info();
        exit(EXIT_SUCCESS);
    }

    // exit(0) indicates successful program termination & it is fully portable, While
    // exit(1) (usually) indicates unsucessful termination. However, it's usage is non-portable.

    /* If user missed anyone of these arguments, program will print Usage Info and
     * exit the program.
     */
    if (s_flag == 0 || E_flag == 0 || b_flag == 0 || t_flag == 0)
    {
        missing_args_error(program_name);
        csim_help_info();
        exit(EXIT_FAILURE);
    }
    return cache_params;
}




int main(int argc, char **argv)
{
    // step1: parse option
    CacheParams cache_params  = parseOptions(argc, argv);

    // step2: read all of the lines and analyze it
    run_cache_simulator(cache_params);


    return 0;
}
