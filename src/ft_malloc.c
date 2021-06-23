#include "ft_libc.h"

const double margin = 1.2; // allocate 20% more than requested
const size_t min_margin = 50; // allocate minimum 50 bytes more than requested
const size_t n = 100, m = 1000;

struct mem_t mem;

static void *alloc(size_t size)
{
    void *addr;

    addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    if (addr == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }
    return addr;
}

static void expand_zone(struct zone_t **zone, int nzones, size_t size)
{
    void *addr;
    struct zone_t *tmp;

    addr = alloc(nzones * sizeof (struct zone_t));
    if (*zone == NULL)
    {
        *zone = addr;
        tmp = addr;
    }
    else
    {
        tmp = *zone;
        while (tmp->next != NULL)
            tmp = tmp->next;
        tmp->next = addr;
        tmp = addr;
    }
    for (int i = 0; i < nzones; ++i)
    {
        tmp->next = i == nzones - 1 ? NULL : addr + (i + 1) * sizeof (struct zone_t);
        tmp->addr = alloc(size);
        tmp->nbytes_used = 0;
        tmp->nbytes_allocated = size;
        tmp->alloc = NULL;
        tmp = tmp->next;
    }
}

static void init()
{
    size_t N = n * (n * margin > n + min_margin ? n * margin : n + min_margin);
    size_t M = m * (m * margin > m + min_margin ? m * margin : m + min_margin);
    size_t pagesize = getpagesize();

    N = pagesize * ((N / pagesize) + 1);
    M = pagesize * ((M / pagesize) + 1);
    if (mem.tiny == NULL)
        expand_zone(&mem.tiny, 3, N);
    if (mem.small == NULL)
        expand_zone(&mem.small, 3, M);
}

static void *create_alloc(void *addr, size_t size, void *next)
{
    struct alloc_t *new;
   
    new = alloc(sizeof (struct alloc_t));
    new->addr = addr;
    new->nbytes_used = 0;
    new->nbytes_allocated = (size * margin > size + min_margin) ? size * margin : size + min_margin;
    new->next = next;
    mem.nbytes_tot += new->nbytes_allocated;
    return new;
}

static void *create_alloc_in_zone(struct zone_t *first_zone, size_t size)
{
    for (struct zone_t *zone = first_zone; zone != NULL; zone = zone->next)
    {
        if (zone->nbytes_allocated - zone->nbytes_used >= size)
        {
            if (zone->alloc == NULL)
            {
                zone->alloc = create_alloc(zone->addr, size, NULL);
                zone->nbytes_used += size;
                return zone->alloc->addr;
            }
            for (struct alloc_t *alloc = zone->alloc; alloc != NULL; alloc = alloc->next)
            {
                if (alloc->next == NULL)
                {
                    if ((size_t)(zone->addr + zone->nbytes_allocated - (alloc->addr + alloc->nbytes_allocated)) >= size)
                    {
                        alloc->next = create_alloc(alloc->addr + alloc->nbytes_allocated, size, NULL);
                        zone->nbytes_used += size;
                        return alloc->next->addr;
                    }
                }
                else if ((size_t)(alloc->next->addr - (alloc->addr + alloc->nbytes_allocated)) >= size)
                {
                    alloc->next = create_alloc(alloc->addr + alloc->nbytes_allocated, size, alloc->next);
                    zone->nbytes_used += size;
                    return alloc->next->addr;
                }
            }
        }
    }
    expand_zone(&first_zone, 3, first_zone->nbytes_allocated); // &first_zone is not updating the real first_zone but is not updated beacause != NULL
    return create_alloc_in_zone(first_zone, size); // same here
}

void *ft_malloc(size_t size)
{
    void *addr;

    if (mem.tiny == NULL || mem.small == NULL)
        init();
    if (size < n)
        addr = create_alloc_in_zone(mem.tiny, size);
    else if (size < m)
        addr = create_alloc_in_zone(mem.small, size);
    else
    {
        addr = alloc(size * margin);
        if (mem.large == NULL)
            mem.large = create_alloc(addr, size, NULL);
        else
        {
            for (struct alloc_t *alloc = mem.large; alloc != NULL; alloc = alloc->next)
            {
                if (alloc->next == NULL)
                {
                    alloc->next = create_alloc(addr, size, NULL);
                    break ;
                }
            }
        }
    }
    return addr;
}