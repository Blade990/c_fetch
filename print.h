#ifndef PRINT_H
#define PRINT_H

/* ===== FORWARD DECLARATIONS ===== */
/* We tell the compiler that these structures exist somewhere. */
typedef struct mem_info mem_info_t ;
typedef struct os_info os_info_t;
typedef struct cpu_info cpu_info_t;
typedef struct disk_info disk_info_t;
typedef struct packages_info packages_info_t;

struct packages;

// ===== COLORI =====
#define BLUE  "\033[34m"
#define CYAN  "\033[36m"
#define RESET "\033[0m"

// ===== PRINT FUNCTION =====
void print_all(os_info_t * os_data,
              // struct cpu *cpu,
               //struct disk_space *disk,
               //struct mem_info *mem,
               mem_info_t *mem_data,
               disk_info_t *disk_data,
               packages_info_t  *pkg_data,
               cpu_info_t *cpu_data
               );

#endif
