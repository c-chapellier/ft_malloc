#include "libc.h"

struct mem_t mem;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int nbytes_zones_alloc = 0;
int nbytes_allocs_alloc = 0;
int nbytes_real_alloc = 0;

#define REAL_SIZE(size) (size * MARGIN > size + MIN_MARGIN) ? size * MARGIN : size + MIN_MARGIN

static void *alloc(size_t size, int flag)
{
    void *addr;

    addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    if (addr == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }
    if (flag == 0)
        nbytes_zones_alloc += size;
    if (flag == 1)
        nbytes_allocs_alloc += size;
    if (flag == 2)
        nbytes_real_alloc += size;
    return addr;
}

static void expand_zone(struct zone_t **zone, int nzones, size_t size)
{
    void *addr;
    struct zone_t *tmp;

    addr = alloc(nzones * sizeof (struct zone_t), 0);
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
        tmp->addr = alloc(size, 2);
        tmp->nbytes_used = 0;
        tmp->nbytes_allocated = size;
        tmp->alloc = NULL;
        tmp = tmp->next;
    }
}

static void init()
{
    size_t N = 100 * (ZONE_N * MARGIN > ZONE_N + MIN_MARGIN ? ZONE_N * MARGIN : ZONE_N + MIN_MARGIN);
    size_t M = 100 * (ZONE_M * MARGIN > ZONE_M + MIN_MARGIN ? ZONE_M * MARGIN : ZONE_M + MIN_MARGIN);
    size_t pagesize = getpagesize();

    N = pagesize * ((N / pagesize) + 1);
    M = pagesize * ((M / pagesize) + 1);
    expand_zone(&mem.tiny, 1, N);
    expand_zone(&mem.small, 1, M);
}

static void *create_alloc(void *addr, size_t size, void *next)
{
    struct alloc_t *new;
   
    new = alloc(sizeof (struct alloc_t), 1);
    new->addr = addr;
    new->nbytes_used = 0;
    new->nbytes_allocated = size;
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
    expand_zone(&first_zone, 1, first_zone->nbytes_allocated); // &first_zone is not updating the real first_zone but is not updated beacause != NULL
    return create_alloc_in_zone(first_zone, size); // same here
}

void *malloc(size_t size)
{
    void *addr;

    // printf("hello -----------------------------------------\n");
    pthread_mutex_lock(&mutex);
    if (mem.tiny == NULL && mem.small == NULL)
        init();
    if (size < ZONE_N)
        addr = create_alloc_in_zone(mem.tiny, REAL_SIZE(size));
    else if (size < ZONE_M)
        addr = create_alloc_in_zone(mem.small, REAL_SIZE(size));
    else
    {
        size *= MARGIN;
        addr = alloc(size, 2);
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
    pthread_mutex_unlock(&mutex);
    return addr;
}