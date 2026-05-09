#include "os_info.h"
#include "dynarray/dynarray.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>

os_info_t *get_os_info(void) {
    /* 1. Allocate the main structure using dynarray (as an array of 1 object) */
    dynarray_t da_struct = danew(sizeof(os_info_t));

    if (darealloc(&da_struct, 1) != 0) {
        return NULL;
    }

    os_info_t *info = (os_info_t *)da_struct.ptr;
    info->os_name = NULL;
    info->kernel_ver = NULL;

    /* --- 2. KERNEL INFORMATION --- */
    struct utsname uts;
    if (uname(&uts) == 0) {
        size_t len = strlen(uts.release) + 1;
        dynarray_t da_kernel = danew(S8);

        if (darealloc(&da_kernel, len) == 0) {
            memcpy(da_kernel.ptr, uts.release, len);
            info->kernel_ver = (char *)da_kernel.ptr;
        }
    }

    /* --- 3. OS NAME INFORMATION --- */
    char tmpbuff[256];
    FILE *fp = fopen(OS_RELEASE_PATH, "r");
    if (fp) {
     while (fgets(tmpbuff, sizeof(tmpbuff), fp)) {
         char *hit = strstr(tmpbuff, PRETTY_NAME_KEY);
         if (hit) {
             char *start = hit + strlen(PRETTY_NAME_KEY);
             char *end = strchr(start, '\"');

            if (end){
                *end = '\0';
                size_t len = strlen(start) + 1;
                dynarray_t da_os = danew(S8);
                if (darealloc(&da_os, len) == 0) {
                    memcpy(da_os.ptr, start, len);
                    info->os_name = (char *)da_os.ptr;
                }
            }
        break;
        }
    }
    fclose(fp);
}

    /* --- 4. FALLBACK LOGIC ---
     * If detection failed, we allocate fallback strings using dynarray
     * to ensure free_os_info works consistently.
     */
    if (!info->os_name) {
        const char *fallback = "Unknown OS";
        size_t len = strlen(fallback) + 1;
        dynarray_t da_os = danew(S8);
        if (darealloc(&da_os, len) == 0) {
            memcpy(da_os.ptr, fallback, len);
            info->os_name = (char *)da_os.ptr;
        }
    }

    if (!info->kernel_ver) {
        const char *fallback = "Unknown Kernel";
        size_t len = strlen(fallback) + 1;
        dynarray_t da_kernel = danew(S8);
        if (darealloc(&da_kernel, len) == 0) {
            memcpy(da_kernel.ptr, fallback, len);
            info->kernel_ver = (char *)da_kernel.ptr;
        }
    }

    return info;
}


void free_os_info(os_info_t *info) {
    if (!info) {
        return;
    }

    /* Free strings first */
    if (info->os_name) {
        dynarray_t da = { .ptr = info->os_name, .ts = S8 };
        dafree(&da);
    }
    if (info->kernel_ver) {
        dynarray_t da = { .ptr = info->kernel_ver, .ts = S8 };
        dafree(&da);
    }

    /* Finally, free the main structure container */
    dynarray_t da_struct = { .ptr = info, .ts = sizeof(os_info_t) };
    dafree(&da_struct);
}
