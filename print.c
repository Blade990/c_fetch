#include "print.h"

#include <stdio.h>


void print_all(char *os,
               char *kernel,
               struct cpu *cpu,
               struct disk_space *disk,
               struct mem_info *mem,
               struct packages *pkg,
               long int full_part,
               long int decimal_part)
{
    printf(BLUE "╭──────────────────────────────╮\n" RESET);

    printf(BLUE "│ " CYAN "%-18s" RESET " : %s\n", "Sistema", os);
    printf(BLUE "│ " CYAN "%-18s" RESET " : %s\n", "Kernel", kernel);
    printf(BLUE "│ " CYAN "%-18s" RESET " : %s\n", "CPU model", cpu->core_model);
    printf(BLUE "│ " CYAN "%-18s" RESET " : %d\n", "CPU cores", cpu->core_numb);
    printf(BLUE "│ " CYAN "%-18s" RESET " : %ld.%02ld GHz\n","CPU max freq", full_part, decimal_part);

    printf(BLUE "│ " CYAN "%-18s" RESET " : %.2f GB\n","Disk (/home)", disk->available_space);

    printf(BLUE "│ " CYAN "%-18s" RESET " : %.2f GB\n", "Mem free", mem->mem_free);

    printf(BLUE "│ " CYAN "%-18s" RESET " : %.2f GB\n","Mem used", mem->mem_used);

    printf(BLUE "│ " CYAN "%-18s" RESET " : flatpak (app) %d | pkg manager: %d | snap %d\n",
           "Packages",
           pkg->pkg_flatpak,
           pkg->pkg_manager,
           pkg->pkg_snap);

    printf(BLUE "╰──────────────────────────────╯\n" RESET);
}
