#ifndef CPU_INFO_H
#define CPU_INFO_H

//Standard header for size_t in C
#include <stddef.h>

/* Keys used to parse the files - Standard across all x86/x64 Linux systems */
#define CPU_MODEL_KEY "model name"
#define CPU_CORES_KEY "processor"
#define CPU_FREQ_KEY  "cpu MHz"

/* Universal paths for CPU data in Linux */
#define CPU_INFO_PATH "/proc/cpuinfo"
#define CPU_MAX_FREQ_PATH "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq"

typedef struct cpu_info {
    char *core_model; /* CPU model name (e.g., "AMD Ryzen 7 5800X") */
    size_t core_numb; /* Total number of CPU cores/threads */
    double max_freq; /* Maximum CPU frequency in MHz */
} cpu_info_t;


/* Prototye Main functions */
cpu_info_t *get_cpu_info(void);
void free_cpu_info(cpu_info_t *info);

/* Prototype Subfunction */
char* get_cpu_model(void);
size_t get_cpu_cores(void);
double get_cpu_freq(void);
#endif
