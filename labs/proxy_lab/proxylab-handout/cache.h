/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000  // 1 MiB
#define MAX_OBJECT_SIZE 102400  // 100 KiB

/*
This setup allows:
O(1) insert/remove
O(n) search by URL (you can improve it with a hash map if needed)
LRU eviction by popping from the tail

Thread-Safe Access Using Reader-Writer Lock
Use pthread_rwlock_t:
pthread_rwlock_rdlock(&lock) for concurrent readers
pthread_rwlock_wrlock(&lock) for writers (insert/remove)
*/
typedef struct cache_block
{
    char *url;                // Unique key
    char *content;            // The cached web object
    int size;                 // Size of the object in bytes
    struct cache_block *prev; // For LRU: older
    struct cache_block *next; // For LRU: newer
} cache_block;

typedef struct
{
    cache_block *head; // Most recently used
    cache_block *tail; // Least recently used
    int total_size;
    pthread_rwlock_t lock; // Reader-writer lock for thread-safe access
} cache_list;

cache_list * initial_cache();

char *cache_get(cache_list *cache, const char *url, int *object_size);

void cache_put(cache_list *cache, const char *url, const char *buf, int size);