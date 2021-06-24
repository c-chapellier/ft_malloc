#ifndef MALLOC_H
#define MALLOC_H

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <pthread.h>

#define RESET       "\e[0;0m"
#define BLACK       "\e[0;30m"
#define RED         "\e[0;31m"
#define GREEN       "\e[0;32m"
#define YELLOW      "\e[0;33m"
#define BLUE        "\e[0;34m"
#define MAGENTA     "\e[0;35m"
#define CYAN        "\e[0;36m"
#define WHITE       "\e[0;37m"

#define MARGIN      1.2 // allocate 20% more than requested
#define MIN_MARGIN  50 // allocate minimum 50 bytes more than requested
#define ZONE_N      100
#define ZONE_M      1000

#define SHOW_ALLOC_MEM_EX_QUIET     0x01
#define SHOW_ALLOC_MEM_EX_STANDARD  0x02
#define SHOW_ALLOC_MEM_EX_FULL      0x04

struct alloc_t
{
    void            *addr;
    size_t          nbytes_used;
    size_t          nbytes_allocated;
    struct alloc_t  *prev;
    struct alloc_t  *next;
};

struct zone_t
{
    void            *addr;
    size_t          nbytes_used;
    size_t          nbytes_allocated;
    struct alloc_t  *alloc;
    struct zone_t   *next;
};

struct mem_t
{
    struct zone_t   *tiny;
    struct zone_t   *small;
    struct alloc_t  *large;
    size_t          nbytes_tot;
    struct alloc_t  *hist;
};

extern struct mem_t mem;

void	free(void *ptr);
void	*malloc(size_t size);
void	*realloc(void *ptr, size_t size);
void    show_alloc_mem();
void    show_alloc_mem_hex();
void    show_alloc_mem_hist();

#endif
