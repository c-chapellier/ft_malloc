#include "libc.h"

void show_alloc_mem_hist()
{
    size_t i = 0, nbytes_tot_allocated = 0;

    printf(MAGENTA "\nALLOCATION_HYSTORY\n");
    for (struct alloc_t *alloc = mem.hist; alloc != NULL; alloc = alloc->next)
    {
        printf("\t%p - %p : %8zu bytes\n", alloc->addr, alloc->addr + alloc->nbytes_allocated, alloc->nbytes_allocated);
        nbytes_tot_allocated += alloc->nbytes_allocated;
        ++i;
    }
    printf("Total : %zu bytes\n\n" RESET, nbytes_tot_allocated);
}