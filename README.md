# ft_malloc
Recode the malloc C function

◦ mmap(2)
◦ munmap(2)
◦ getpagesize(3)
◦ getrlimit(2)
◦ les fonctions de la libpthread

Vous devrez donc “pré-allouer” des zones mémoire pour y stocker vos “petits” et “moyens” malloc.
La taille de ces zones devra impérativement être un multiple de getpagesize().
Chaque zone doit pouvoir contenir au moins 100 allocations.
Les mallocs “TINY”, de 1 à n octets, seront stockés dans des zones de N octets
Les mallocs “SMALL”, de (n+1) à m octets, seront stockés dans des zones de M octets
Les mallocs “LARGE”, de (m+1) octets et plus, seront stockés hors zone, c’est à dire simplement avec un mmap(), ils seront en quelquesorte une zone à eux tout seul.
• C’est à vous de définir la taille de n, m, N et M afin de trouver un bon compromis entre vitesse (économie d’appel système) et économie de mémoire.