#ifndef FT_MALLOC_H
#define FT_MALLOC_H

#include <stdio.h>
#include <errno.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/resource.h>

#ifndef DEBUG
#define DEBUG       0
#endif
#define DPRINTF(f_, ...) if (DEBUG) printf((f_), ##__VA_ARGS__)

#define RESET       "\e[0;0m"
#define BLACK       "\e[0;30m"
#define RED         "\e[0;31m"
#define GREEN       "\e[0;32m"
#define YELLOW      "\e[0;33m"
#define BLUE        "\e[0;34m"
#define MAGENTA     "\e[0;35m"
#define CYAN        "\e[0;36m"
#define WHITE       "\e[0;37m"

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
};

extern struct mem_t mem;

void	ft_free(void *ptr);
void	*ft_malloc(size_t size);
void	*ft_realloc(void *ptr, size_t size);
void    show_alloc_mem();
void    show_alloc_mem_ex();

#endif
