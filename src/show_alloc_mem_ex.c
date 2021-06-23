#include "ft_libc.h"

static void hexdump(void *addr, size_t size, char *prefix ,int flags)
{
    char buf[8 * 80];
    int nchar_printed;

    buf[0] = '\0';
    if (flags & SHOW_ALLOC_MEM_EX_QUIET) nchar_printed = 16;
    else if (flags & SHOW_ALLOC_MEM_EX_FULL) nchar_printed = INT32_MAX;
    else nchar_printed = 16 * 8;
    for (int i = 0; i < size && i < nchar_printed - 1; ++i)
    {
        if (i % 16 == 0) sprintf(buf, "%s\n%s%p    %02x ", buf, prefix, &((char *)addr)[i], ((char *)addr)[i]);
        else if (i % 8 == 7) sprintf(buf, "%s%02x    ", buf, ((char *)addr)[i]);
        else sprintf(buf, "%s%02x ", buf, ((char *)addr)[i]);
    }
    if (size > nchar_printed) sprintf(buf, "%s..", buf);
    printf(RESET "%s\n" MAGENTA, buf);
}

void show_alloc_mem_ex()
{
    int i;

    printf(MAGENTA "\nTINY : %p\n", mem.tiny);
    i = 0;
    for (struct zone_t *zone = mem.tiny; zone != NULL; zone = zone->next)
    {
        printf("\tZ[%d] : %p - %p : %8zu octets [%8zu used]\n", i, zone->addr, zone->addr + zone->nbytes_allocated, zone->nbytes_allocated, zone->nbytes_used);
        for (struct alloc_t *alloc = zone->alloc; alloc != NULL; alloc = alloc->next)
        {
            printf("\t\t%p - %p : %zu octets", alloc->addr, alloc->addr + alloc->nbytes_allocated, alloc->nbytes_allocated);
            hexdump(alloc->addr, alloc->nbytes_used, "\t\t\t", SHOW_ALLOC_MEM_EX_STANDARD);
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
            printf("\t\t%p - %p : %zu octets", alloc->addr, alloc->addr + alloc->nbytes_allocated, alloc->nbytes_allocated);
            hexdump(alloc->addr, alloc->nbytes_used, "\t\t\t", SHOW_ALLOC_MEM_EX_STANDARD);
        }
        ++i;
    }
    printf("LARGE : %p\n", mem.large);
    for (struct alloc_t *alloc = mem.large; alloc != NULL; alloc = alloc->next)
    {
        printf("\t%p - %p : %zu octets", alloc->addr, alloc->addr + alloc->nbytes_allocated, alloc->nbytes_allocated);
        hexdump(alloc->addr, alloc->nbytes_used, "\t\t", SHOW_ALLOC_MEM_EX_STANDARD);
    }
    printf("Total : %zu octets\n\n" RESET, mem.nbytes_tot);
}