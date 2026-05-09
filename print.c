#include "print.h"
#include "disk_info.h"
#include "os_info.h"
#include "cpu_info.h"
#include "mem_info.h"
#include "packages.h"

#include <stdio.h>


void print_all(os_info_t *os_data,mem_info_t *mem_data,disk_info_t *disk_data, packages_info_t *pkg_data,cpu_info_t *cpu_data) {

    printf(BLUE "╭──────────────────────────────╮\n" RESET);

    /* ===== 1. OS & KERNEL SECTION ===== */
    if (os_data != NULL) {
        if (os_data->os_name != NULL && os_data->os_name[0] != '\0') {
            printf(BLUE "│ " CYAN "%-12s" RESET " : %s\n", "Sistema", os_data->os_name);
        }
        if (os_data->kernel_ver != NULL && os_data->kernel_ver[0] != '\0') {
            printf(BLUE "│ " CYAN "%-12s" RESET " : %s\n", "Kernel", os_data->kernel_ver);
        }
    }
    /* ===== 2. CPU SECTION ===== */
    if (cpu_data != NULL) {
        if (cpu_data->core_model != NULL && cpu_data->core_model[0] != '\0') {
            printf(BLUE "│ " CYAN "%-12s" RESET " : %s\n", "CPU model", cpu_data->core_model);
        }
        if (cpu_data->core_numb > 0) {
            printf(BLUE "│ " CYAN "%-12s" RESET " : %lu\n", "CPU cores", (unsigned long)cpu_data->core_numb);
        }
        if (cpu_data->max_freq > 0) {
            printf(BLUE "│ " CYAN "%-12s" RESET " : %lu MHz\n", "CPU max freq", (unsigned long)cpu_data->max_freq);
        }
    }

    /* ===== 3. MEMORY SECTION (NEW) ===== */
    if (mem_data != NULL && mem_data->total > 0.0) {

        /* Mostra la memoria usata rispetto al totale in GB */
        printf(BLUE "│ " CYAN "%-12s" RESET " : %.2f / %.2f GB\n", "Memory",
            mem_data->used,
            mem_data->total);

         /* Mostra le percentuali di occupazione e libertà */
        printf(BLUE "│ " CYAN "%-12s" RESET " : %.1f%% used | %.1f%% free\n", "Mem Usage",
            mem_data->used_perc,
            mem_data->free_perc);
    }

    /* ===== 4. DISK SECTION ===== */
    if (disk_data != NULL) {
        /* First line: shows the available space in GB */
        printf(BLUE "│ " CYAN "%-12s" RESET " : %.2f GB\n","Disk Avail", disk_data->available_space);

        /* Second line: shows explicitly what the two percentages represent */
        printf(BLUE "│ " CYAN "%-12s" RESET " : %.1f%% used | %.1f%% free\n","Disk Usage",
            disk_data->used_percentage,
            disk_data->free_percentage);
    }

    /* ===== 3. PACKAGES SECTION ===== */
    if (pkg_data != NULL) {
        if (pkg_data->pkg_manager > 0 || pkg_data->pkg_flatpak > 0 || pkg_data->pkg_snap > 0) {
            printf(BLUE "│ " CYAN "%-12s" RESET " : ", "Packages");

            int first = 1;
            if (pkg_data->pkg_manager > 0) {
                printf("%d (pkg)", pkg_data->pkg_manager);
                first = 0;
            }

            if (pkg_data->pkg_flatpak > 0) {
                if (!first) {
                    printf(" | ");
                }
                printf("%d (flatpak)", pkg_data->pkg_flatpak);
                first = 0;
            }

            if (pkg_data->pkg_snap > 0) {
                if (!first) {
                    printf(" | ");
                }
                printf("%d (snap)", pkg_data->pkg_snap);
            }
            printf("\n");
        }
    }


    //printf(BLUE "│ " CYAN "%-18s" RESET " : %s\n", "CPU model", cpu->core_model);
    //printf(BLUE "│ " CYAN "%-18s" RESET " : %d\n", "CPU cores", cpu->core_numb);
    //printf(BLUE "│ " CYAN "%-18s" RESET " : %ld.%02ld GHz\n","CPU max freq", full_part, decimal_part);

    // printf(BLUE "│ " CYAN "%-18s" RESET " : %.2f GB\n","Disk (/home)", disk->available_space);

    //printf(BLUE "│ " CYAN "%-18s" RESET " : %.2f GB\n", "Mem free", mem->mem_free);

    //printf(BLUE "│ " CYAN "%-18s" RESET " : %.2f GB\n","Mem used", mem->mem_used);


    printf(BLUE "╰──────────────────────────────╯\n" RESET);
}
