#include "libc.h"

#define NOT_FOUND           0
#define ENOUGH_SPACE        1
#define NOT_ENOUGH_SPACE    2

static int check_zone(struct zone_t *first, void *addr, size_t size)
{
    for (struct zone_t *zone = first; zone != NULL; zone = zone->next)
    {
        for (struct alloc_t *alloc = zone->alloc; alloc != NULL; alloc = alloc->next)
        {
            if (addr == alloc->addr)
            {
                if (alloc->nbytes_allocated >= size)
                {
                    mem.nbytes_tot += size - alloc->nbytes_used;
                    alloc->nbytes_used = size;
                    return ENOUGH_SPACE;
                }
                return NOT_ENOUGH_SPACE;
            }
        }
    }
    return NOT_FOUND;
}

void *realloc(void *ptr, size_t size)
{
    int rc;
    
    pthread_mutex_lock(&mutex);
    rc = check_zone(mem.tiny, ptr, size);
    if (rc == ENOUGH_SPACE)
        return ptr;
    if (rc == NOT_FOUND)
    {
        rc = check_zone(mem.small, ptr, size);
        if (rc == ENOUGH_SPACE)
            return ptr;
    }
    if (rc == NOT_FOUND)
    {
        for (struct alloc_t *alloc = mem.large; alloc != NULL; alloc = alloc->next)
        {
            if (ptr == alloc->addr)
            {
                if (alloc->nbytes_allocated >= size)
                {
                    mem.nbytes_tot += size - alloc->nbytes_used;
                    alloc->nbytes_used = size;
                    return ptr;
                }
                rc = NOT_ENOUGH_SPACE;
                break ;
            }
        }
    }
    if (rc == NOT_FOUND)
        abort();
    pthread_mutex_unlock(&mutex);
    free(ptr);
    return malloc(size);
}