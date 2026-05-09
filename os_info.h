#ifndef  OS_INFO_H
#define OS_INFO_H

#include <stddef.h>

/* Constants for file parsing */
#define OS_RELEASE_PATH "/etc/os-release"
#define PRETTY_NAME_KEY "PRETTY_NAME=\""

/* Constants for file parsing */
#define OS_RELEASE_PATH "/etc/os-release"
#define PRETTY_NAME_KEY "PRETTY_NAME=\""

/*
 * Structure to hold Operating System and Kernel information.
 * Strings are managed via dynarray pointers.
 */
typedef struct os_info {
    char *os_name;    /* Distribution name (e.g., "EndeavourOS") */
    char *kernel_ver; /* Kernel version (e.g., "6.19.13") */
} os_info_t;

/*  prototypes */
os_info_t* get_os_info(void);
void free_os_info(os_info_t *info);
#endif
