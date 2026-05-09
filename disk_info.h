#ifndef DISK_INFO_H
#define DISK_INFO_H

#include <sys/statvfs.h>

#define GB_CONVERSION (1024.0 * 1024.0 * 1024.0)

typedef struct disk_info {
    double total_space;      /* Spazio totale in GB */
    double free_space;       /* Spazio libero in GB */
    double available_space;  /* Spazio disponibile per l'utente non-root in GB */
    double free_percentage;  /* Percentuale di spazio libero */
    double used_percentage;  /* Percentuale di spazio occupato */
} disk_info_t;

/* Funzione principale */
disk_info_t *get_disk_info(void);
void free_disk_info(disk_info_t *info);

/* Sottofunzioni di calcolo */
double calc_total_space(const struct statvfs *fs_info);
double calc_available_space(const struct statvfs *fs_info);
double calc_free_space(const struct statvfs *fs_info);

#endif
