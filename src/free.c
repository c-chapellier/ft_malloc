#include "libc.h"

static void add_to_hist(struct alloc_t *alloc)
{
    struct alloc_t *tmp;

    alloc->next = NULL;
    alloc->prev = NULL;
    if (mem.hist == NULL)
    {
        mem.hist = alloc;
        return ;
    }
    tmp = mem.hist;
    while (tmp->next != NULL)
        tmp = tmp->next;
    tmp->next = alloc;
}

static void dealloc(void *addr, size_t size)
{
    int rc;

    rc = munmap(addr, size);
    if (rc == -1)
    {
        perror("munmap");
        exit(1);
    }
}

static void dealloc_alloc_from_zone(struct zone_t *zone, struct alloc_t *alloc)
{
    if (alloc->prev == NULL)
        zone->alloc = alloc->next;
    else
        alloc->prev->next = alloc->next;
    zone->nbytes_used -= alloc->nbytes_allocated;
    mem.nbytes_tot -= alloc->nbytes_allocated;
    add_to_hist(alloc);
}

static void dealloc_alloc(struct alloc_t **first, struct alloc_t *alloc)
{
    if (alloc->prev == NULL)
        *first = alloc->next;
    else
        alloc->prev->next = alloc->next;
    dealloc(alloc->addr, alloc->nbytes_allocated);
    mem.nbytes_tot -= alloc->nbytes_allocated;
    add_to_hist(alloc);
}

static int check_zone(struct zone_t *first, void *addr)
{
    for (struct zone_t *zone = first; zone != NULL; zone = zone->next)
    {
        for (struct alloc_t *alloc = zone->alloc; alloc != NULL; alloc = alloc->next)
        {
            if (addr == alloc->addr)
            {
                dealloc_alloc_from_zone(zone, alloc);
                return 1;
            }
        }
    }
    return 0;
}

void free(void *addr)
{
    pthread_mutex_lock(&mutex);
    if (addr == NULL)
    {
        pthread_mutex_unlock(&mutex);
        return ;
    }
    if (check_zone(mem.tiny, addr) == 1)
    {
        pthread_mutex_unlock(&mutex);
        return ;
    }
    if (check_zone(mem.small, addr) == 1)
    {
        pthread_mutex_unlock(&mutex);
        return ;
    }
    for (struct alloc_t *alloc = mem.large; alloc != NULL; alloc = alloc->next)
    {
        if (addr == alloc->addr)
        {
            dealloc_alloc(&mem.large, alloc);
            pthread_mutex_unlock(&mutex);
            return ;
        }
    }
    pthread_mutex_unlock(&mutex);
    abort();
    // dealloc(addr, 1);
}
