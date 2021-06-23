#include "libc.h"

void show_alloc_mem()
{
    int i;

    printf(MAGENTA "\nTINY : %p\n", mem.tiny);
    i = 0;
    for (struct zone_t *zone = mem.tiny; zone != NULL; zone = zone->next)
    {
        printf("\tZ[%d] : %p - %p : %8zu octets [%8zu used]\n", i, zone->addr, zone->addr + zone->nbytes_allocated, zone->nbytes_allocated, zone->nbytes_used);
        for (struct alloc_t *alloc = zone->alloc; alloc != NULL; alloc = alloc->next)
        {
            printf("\t\t%p - %p : %zu octets\n", alloc->addr, alloc->addr + alloc->nbytes_allocated, alloc->nbytes_allocated);
        }
        ++i;
    }
    printf("SMALL : %p\n", mem.small);
    i = 0;
    for (struct zone_t *zone = mem.small; zone != NULL; zone = zone->next)
    {
        printf("\tZ[%d] : %p - %p : %8zu octets [%8zu used]\n", i, zone->addr, zone->addr + zone->nbytes_allocated, zone->nbytes_allocated, zone->nbytes_used);
        for (struct alloc_t *alloc = zone->alloc; alloc != NULL; alloc = alloc->next)
        {
            printf("\t\t%p - %p : %zu octets\n", alloc->addr, alloc->addr + alloc->nbytes_allocated, alloc->nbytes_allocated);
        }
        ++i;
    }
    printf("LARGE : %p\n", mem.large);
    for (struct alloc_t *alloc = mem.large; alloc != NULL; alloc = alloc->next)
    {
        printf("\t%p - %p : %zu octets\n", alloc->addr, alloc->addr + alloc->nbytes_allocated, alloc->nbytes_allocated);
    }
    printf("Total : %zu octets\n\n" RESET, mem.nbytes_tot);
}