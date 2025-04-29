

#include "cache.h"


void evict_from_cache(cache_list *cache);


// Cache Lookup
char *cache_get(cache_list *cache, const char *url, int *object_size) {
    pthread_rwlock_rdlock(&cache->lock);

    cache_block *p = cache->head;
    while (p) {
        if (strcmp(p->url, url) == 0) {
            // Move this block to the front (most recently used)
            move_to_front(cache, p);
            *object_size = p->size;

            char *copy = malloc(p->size);
            memcpy(copy, p->content, p->size);
            pthread_rwlock_unlock(&cache->lock);
            return copy;
        }
        p = p->next;
    }

    pthread_rwlock_unlock(&cache->lock);
    return NULL;
}


// Eviction Policy (Approx LRU)
void cache_put(cache_list *cache, const char *url, const char *buf, int size) {
    if (size > MAX_OBJECT_SIZE) return;

    pthread_rwlock_wrlock(&cache->lock);

    // Evict until there’s space
    while (cache->total_size + size > MAX_CACHE_SIZE) {
        evict_from_cache(cache);
    }

    cache_block *new_block = malloc(sizeof(cache_block));
    new_block->url = strdup(url);
    new_block->content = malloc(size);
    memcpy(new_block->content, buf, size);
    new_block->size = size;

    insert_at_front(cache, new_block);
    cache->total_size += size;

    pthread_rwlock_unlock(&cache->lock);
}


void evict_from_cache(cache_list *cache) {
    if (!cache->tail) return;

    cache_block *victim = cache->tail;

    remove_from_list(cache, victim);
    cache->total_size -= victim->size;
    free(victim->url);
    free(victim->content);
    free(victim);
}

