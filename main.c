#include "os_info.h"
#include "cpu_info.h"
#include "disk_info.h"
#include "mem_info.h"
#include "packages.h"
#include "print.h"


#include <stdio.h>
#include <stdlib.h>



int main(void) {
    /* --- VARIABLE DECLARATIONS AND INITIALIZATIONS --- */
    os_info_t *os_data = NULL;
    cpu_info_t *cpu_data = NULL;
    packages_info_t *pkg_data = NULL;
    disk_info_t *disk_data = NULL;
    mem_info_t *mem_data = NULL;


    /* ===== 1. GET OS & KERNEL INFO (Using Dynarray) ===== */
    os_data = get_os_info();
    if (!os_data){
        fprintf(stderr, "Error: get_os_info failed\n");
    }

    /* ===== 2. GET CPU INFO ===== */
    cpu_data = get_cpu_info();
    if (cpu_data == NULL) {
        fprintf(stderr, "Error: get_cpu_info failed\n");
        goto cleanup;
    }

    /* ===== 3. GET PACKAGES INFO ===== */
    pkg_data = get_packages();
    if (pkg_data == NULL) {
        fprintf(stderr, "Error: get_packages failed\n");
        goto cleanup;
    }

    /* ===== 3. GET DISK INFO ===== */
    disk_data = get_disk_info();
    if (disk_data == NULL) {
        fprintf(stderr, "Error: get_disk_info failed\n");
        goto cleanup;
    }

    /* ===== 5. GET MEMORY INFO ===== */
    mem_data = get_mem_info();
    if (mem_data == NULL) {
        fprintf(stderr, "Error: get_mem_info failed\n");
        goto cleanup;
    }

    /*if (!(mem_info = m_info())) {
        fprintf(stderr, "mem_info failed\n");
        goto cleanup;
        }

    if (!(pkg_info = get_packages())) {
        fprintf(stderr, "get_packages failed\n");
        goto cleanup;
    }*/

    // ===== FORMAT CPU FREQ =====
   /*
       full_part = cpu->max_freq / 100;
      decimal_part = cpu->max_freq % 100;
   */
    // ===== PRINT =====
    print_all(os_data,mem_data,disk_data,pkg_data,cpu_data);

cleanup:
    /* ===== 5. CLEANUP / FREE MEMORY ===== */
    if (os_data) {
        free_os_info(os_data);
    }
    if(cpu_data){
        free_cpu_info(cpu_data);
    }
    if(pkg_data){
        free_packages_info(pkg_data);
    }

    //free(disk);
    //free(mem_info);
     return 0;
}
