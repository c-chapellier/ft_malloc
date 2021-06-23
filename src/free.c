#include "libc.h"

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
}

static void dealloc_alloc(struct alloc_t **first, struct alloc_t *alloc)
{
    if (alloc->prev == NULL)
        *first = alloc->next;
    else
        alloc->prev->next = alloc->next;
    dealloc(alloc->addr, alloc->nbytes_allocated);
    mem.nbytes_tot -= alloc->nbytes_allocated;
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
    if (addr == NULL) return ;
    if (check_zone(mem.tiny, addr) == 1) return ;
    if (check_zone(mem.small, addr) == 1) return ;
    for (struct alloc_t *alloc = mem.large; alloc != NULL; alloc = alloc->next)
    {
        if (addr == alloc->addr)
        {
            dealloc_alloc(&mem.large, alloc);
            return ;
        }
    }
    abort();
    // dealloc(addr, 1);
}
