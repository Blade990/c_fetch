#ifndef PRINT_H
#define PRINT_H

#include "system.h"
#include "packages.h"

// ===== COLORI =====
#define BLUE  "\033[34m"
#define CYAN  "\033[36m"
#define RESET "\033[0m"

// ===== PRINT FUNCTION =====
void print_all(char *os,
               char *kernel,
               struct cpu *cpu,
               struct disk_space *disk,
               struct mem_info *mem,
               struct packages *pkg,
               long int full_part,
               long int decimal_part);

#endif
