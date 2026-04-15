#include "system.h"
#include "packages.h"
#include "print.h"

#include <stdio.h>
#include <stdlib.h>



int main(void) {

    char *kernel_ver = NULL;
    char *os_name = NULL;

    struct cpu *cpu = NULL;
    struct disk_space *disk = NULL;
    struct mem_info *mem_info = NULL;
    struct packages *pkg_info = NULL;

    long int full_part, decimal_part;

    // ===== GET INFO =====
    if (!(kernel_ver = kernel_info())) {
        fprintf(stderr, "kernel_info failed\n");
        goto cleanup;
    }

    if (!(os_name = os_name_inf())) {
        fprintf(stderr, "os_name_inf failed\n");
        goto cleanup;
    }

    if (!(cpu = cpu_info())) {
        fprintf(stderr, "cpu_info failed\n");
        goto cleanup;
    }

    if (!(disk = disk_usage())) {
        fprintf(stderr, "disk_usage failed\n");
        goto cleanup;
    }

    if (!(mem_info = m_info())) {
        fprintf(stderr, "mem_info failed\n");
        goto cleanup;
    }

    if (!(pkg_info = get_packages())) {
        fprintf(stderr, "get_packages failed\n");
        goto cleanup;
    }

    // ===== FORMAT CPU FREQ =====
    full_part = cpu->max_freq / 100;
    decimal_part = cpu->max_freq % 100;

    // ===== PRINT =====
    print_all(os_name,
              kernel_ver,
              cpu,
              disk,
              mem_info,
              pkg_info,
              full_part,
              decimal_part);

cleanup:
    free(kernel_ver);
    free(os_name);

    if (cpu) {
        free(cpu->core_model);
        free(cpu);
    }

    free(disk);
    free(mem_info);
    free(pkg_info);

    return 0;
}
