#ifndef MEM_INF_H
#define  MEM_INF_H

/* Const path Mem info  */
#define MEM_INFO_PATH "/proc/meminfo"

/* Keys to look for in /proc/meminfo */
#define KEY_MEM_TOT "MemTotal:"
//#define KEY_MEM_FREE "MemFree:"
#define KEY_MEM_AVAIL "MemAvailable:"
/* CONST for form Kb to GB */
#define CONV_GB (1.00/(1024.0 * 1024.0))

typedef struct mem_info {
    double total;        /* RAM totale in GB */
    double available;    /* RAM disponibile (reale) in GB */
    double used;         /* RAM effettivamente usata in GB */
    double used_perc;    /* Percentuale occupata */
    double free_perc;    /* Percentuale libera */
} mem_info_t;

// Prototype function
mem_info_t* get_mem_info(void);
void free_mem_info(mem_info_t *mem_data);
double get_mem_available(void);
double get_mem_total(void);
#endif
