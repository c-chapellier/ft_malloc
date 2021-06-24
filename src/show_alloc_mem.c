#include "libc.h"

void show_alloc_mem()
{
    int i;

    pthread_mutex_lock(&mutex);
    printf(MAGENTA "\nTINY : %p\n", mem.tiny);
    i = 0;
    for (struct zone_t *zone = mem.tiny; zone != NULL; zone = zone->next)
    {
        printf("\tZ[%d] : %p - %p : %8zu bytes [%8zu used]\n", i, zone->addr, zone->addr + zone->nbytes_allocated, zone->nbytes_allocated, zone->nbytes_used);
        for (struct alloc_t *alloc = zone->alloc; alloc != NULL; alloc = alloc->next)
        {
            printf("\t\t%p - %p : %8zu bytes\n", alloc->addr, alloc->addr + alloc->nbytes_allocated, alloc->nbytes_allocated);
        }
        ++i;
    }
    printf("SMALL : %p\n", mem.small);
    i = 0;
    for (struct zone_t *zone = mem.small; zone != NULL; zone = zone->next)
    {
        printf("\tZ[%d] : %p - %p : %8zu bytes [%8zu used]\n", i, zone->addr, zone->addr + zone->nbytes_allocated, zone->nbytes_allocated, zone->nbytes_used);
        for (struct alloc_t *alloc = zone->alloc; alloc != NULL; alloc = alloc->next)
        {
            printf("\t\t%p - %p : %8zu bytes\n", alloc->addr, alloc->addr + alloc->nbytes_allocated, alloc->nbytes_allocated);
        }
        ++i;
    }
    printf("LARGE : %p\n", mem.large);
    for (struct alloc_t *alloc = mem.large; alloc != NULL; alloc = alloc->next)
    {
        printf("\t%p - %p : %8zu bytes\n", alloc->addr, alloc->addr + alloc->nbytes_allocated, alloc->nbytes_allocated);
    }
    printf("Total : %zu bytes\n\n" RESET, mem.nbytes_tot);
    printf("%d %d %d\n", nbytes_zones_alloc, nbytes_allocs_alloc, nbytes_real_alloc);
    pthread_mutex_unlock(&mutex);
}