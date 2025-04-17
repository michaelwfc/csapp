// #include "csapp.h"
#include <stddef.h>
#include <stdio.h>
#include <pthread.h>
#include "quick_sort.h"


#define nele_max_sort_serial 10




typedef struct task_queue {
    // Define the task queue structure
    sort_task_t **tasks;
    size_t capacity;
    size_t size;
    pthread_mutex_t lock;
    pthread_cond_t cond;
} task_queue;

typedef task_queue *task_queue_ptr;

/**
 * Structure representing a sorting task.
 * This structure holds the necessary information to perform a sorting task.
 *
 * @param base Pointer to the starting address of the data to be sorted.
 * @param nele Number of elements in the data to be sorted.
 * @param tq Pointer to the task queue that manages this task.
 */
typedef struct sort_task {
    data_t *base;         // Pointer to the starting address of the data to be sorted
    size_t nele;          // Number of elements in the data to be sorted
    task_queue_ptr tq;    // Pointer to the task queue that manages this task
} sort_task_t;


void swap(data_t *a, data_t *b);
size_t partition(data_t *base, size_t nele);


/*
Sets up data structures
Calls recursive sort routine
Keeps joining threads until none left
Frees data structures
*/
void tqsort(data_t *base, size_t nele)
{
    init_task(nele);
    global_base = base;
    global_end = global_base + nele - 1;
    task_queue_ptr tq = new_task_queue();
    tqsort_helper(base, nele, tq);
    join_tasks(tq);
    free_task_queue(tq);
}



/**
Recursive sort routine:
Small partition: Sort serially
Large partition: Spawn new sort task

 * A helper function for parallel sorting using a task queue.
 * This function decides whether to use sequential sorting or to create a sorting task for parallel sorting based on the number of elements.
 * 
 * @param base Pointer to the data array to be sorted.
 * @param nele Number of elements in the data array.
 * @param tq Pointer to the task queue, used for task management and execution.
 */
static void tqsort_helper(data_t *base, size_t nele,
                          task_queue_ptr tq)
{
    // If the number of elements is less than or equal to the threshold for sequential sorting, use sequential sorting directly
    if (nele <= nele_max_sort_serial)
    {
        /* Use sequential sort */
        qsort_serial(base, nele);
        return;
    }
    
    // Create a sorting task for parallel sorting
    sort_task_t *t = new_task(base, nele, tq);
    
    // Spawn the sorting task for execution
    spawn_task(tq, sort_thread, (void *)t);
}

/* Thread routine for many-threaded quicksort
Get task parameters
Perform partitioning step
Call recursive sort routine on each partition
*/
static void *sort_thread(void *vargp)
{
    sort_task_t *t = (sort_task_t *)vargp;
    data_t *base = t->base;
    size_t nele = t->nele;
    task_queue_ptr tq = t->tq;
    free(vargp);
    size_t m = partition(base, nele);
    if (m > 1)
        tqsort_helper(base, m, tq);
    if (nele - 1 > m + 1)
        tqsort_helper(base + m + 1, nele - m - 1, tq);
    return NULL;
}


/**
 * Allocates and initializes a new sorting task.
 *
 * @param base Pointer to the starting address of the data to be sorted.
 * @param nele Number of elements in the data to be sorted.
 * @param tq Pointer to the task queue that manages this task.
 * @return Pointer to the newly created sorting task.
 */
sort_task_t *new_task(data_t *base, size_t nele, task_queue_ptr tq) {
    sort_task_t *t = (sort_task_t *)malloc(sizeof(sort_task_t));
    t->base = base;
    t->nele = nele;
    t->tq = tq;
    return t;
}
task_queue_ptr new_task_queue() {
    task_queue_ptr tq = (task_queue_ptr)malloc(sizeof(task_queue));
    tq->tasks = (sort_task_t **)malloc(10 * sizeof(sort_task_t *));
    tq->capacity = 10;
    tq->size = 0;
    pthread_mutex_init(&tq->lock, NULL);
    pthread_cond_init(&tq->cond, NULL);
    return tq;
}

void free_task_queue(task_queue_ptr tq) {
    free(tq->tasks);
    pthread_mutex_destroy(&tq->lock);
    pthread_cond_destroy(&tq->cond);
    free(tq);
}

void spawn_task(task_queue_ptr tq, void *(*thread_func)(void *), void *arg) {
    pthread_t tid;
    pthread_create(&tid, NULL, thread_func, arg);
    pthread_detach(tid);
}

void join_tasks(task_queue_ptr tq) {
    // Wait for all tasks to complete
    pthread_mutex_lock(&tq->lock);
    while (tq->size > 0) {
        pthread_cond_wait(&tq->cond, &tq->lock);
    }
    pthread_mutex_unlock(&tq->lock);
}


/*
base: Starting address
nele: Number of elements in subrange
*/
void qsort_serial(data_t *base, size_t nele)
{
    if (nele <= 1)
        return;
    if (nele == 2)
    {
        if (base[0] > base[1])
            swap(base, base + 1);
        return;
    }

    /* Partition returns index of pivot */
    size_t m = partition(base, nele);
    // Recursively sort L or R if has more than one element
    if (m > 1)
        qsort_serial(base, m); // left part
    if (nele - 1 > m + 1)
        qsort_serial(base + m + 1, nele - m - 1); // right part
}

void swap(data_t *a, data_t *b)
{
    data_t tmp = *a;
    *a = *b;
    *b = tmp;
}

size_t partition(data_t *base, size_t nele)
{
    // Choose the last element as the pivot
    data_t pivot = base[nele - 1];
    size_t i = 0; // Index of the smaller element

    for (size_t j = 0; j < nele - 1; j++)
    {
        // If the current element is smaller than or equal to the pivot
        if (base[j] <= pivot)
        {
            // Swap base[i] and base[j]
            swap(base + i, base + j);
            i++;
        }
    }

    // Swap the pivot element with the element at i+1
    swap(base + i, base + nele - 1);

    // Return the partitioning index
    return i;
}

int main(int argc, char const *argv[])
{
    data_t arr[] = {5, 2, 9, 1, 7, 3, 8, 4, 6};
    size_t nele = sizeof(arr) / sizeof(arr[0]);
    qsort_serial(arr, nele);
    for (size_t i = 0; i < nele; i++)
    {
        printf("%d ", arr[i]);
    }
}

