#include "disk_info.h"
#include "dynarray/dynarray.h"

#include <sys/statvfs.h>
#include <sys/statvfs.h>

disk_info_t *get_disk_info(void) {
    disk_info_t *info = NULL;
    struct statvfs fs_info;
    char *path_user = getenv("HOME");

    if (path_user == NULL) {
        fprintf(stderr, "Error: HOME environment variable is not defined.\n");
        return NULL;
    }

    if (statvfs(path_user, &fs_info) != 0) {
        perror("statvfs() failed");
        return NULL;
    }

    /* Dynamic allocation via dynarray */
    dynarray_t da_struct = danew(sizeof(disk_info_t));
    if (darealloc(&da_struct, 1) != 0) {
        return NULL;
    }
    info = (disk_info_t *)da_struct.ptr;

    /* Use helper functions to populate the struct */
    info->total_space = calc_total_space(&fs_info);
    info->available_space = calc_available_space(&fs_info);
    info->free_space = calc_free_space(&fs_info);

    /* Percentage calculations */
    if (info->total_space > 0.0) {
        info->free_percentage = (info->available_space / info->total_space) * 100.0;
        info->used_percentage = 100.0 - info->free_percentage;
    } else {
        info->free_percentage = 0.0;
        info->used_percentage = 0.0;
    }

    return info;
}

/* 1. Calculate total space in GB */
double calc_total_space(const struct statvfs *fs_info) {
    if (!fs_info){
        return 0.0;
    }

    return (double)(fs_info->f_blocks * fs_info->f_frsize) / GB_CONVERSION;
}

/* 2. Calculate available space for non-root user in GB */
double calc_available_space(const struct statvfs *fs_info) {
    if (!fs_info) {
        return 0.0;
    }

    return (double)(fs_info->f_bavail * fs_info->f_frsize) / GB_CONVERSION;
}

/* 3. Calculate total free space in GB */
double calc_free_space(const struct statvfs *fs_info) {
    if (!fs_info) {
        return 0.0;
    }

    return (double)(fs_info->f_bfree * fs_info->f_frsize) / GB_CONVERSION;
}

/* free the  struct if exist */
void free_disk_info(disk_info_t *info) {
    dynarray_t da_struct;
    if (!info) {
        return;
    }
    da_struct.ptr = info;
    da_struct.ts = sizeof(disk_info_t);
    dafree(&da_struct);
}
