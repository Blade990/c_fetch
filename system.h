#ifndef SYSTEM_H
#define SYSTEM_H


// struct of cpu that contain cpu info
struct cpu{
    char * core_model; // it will contain cpu model name of processor
    long int  max_freq; // it will contain the current max freq of cpu model in mhz OR Ghz
    int core_numb; // it will contain the number of cpu's core
};

// struct contain the differnet space on HD storgage memory
struct disk_space{
    double total_space; // Total disk space in bytestal space
    double free_space; // Free space available in bytes
    double available_space; // Usable space available for the user in bytes
};

//struct of memory info RAM
struct mem_info {
    double mem_tot; // The total amount of physical RAM in the system.
    double mem_free; // The amount of RAM that is completely free (i.e., not in use by any processes or cache).
    double mem_avi; // The amount of memory that is available for new applications, considering both free memory and reclaimable memory (from caches and buffers).
    double mem_used; // the amount of memory RAM actually used
};

//prototype function
struct cpu *cpu_info(void); // fill cpu struct fields
struct disk_space *disk_usage(void);// fill storage memory (disk) struct fields
char * kernel_info(void); // find kernel version
char * os_name_inf(void); // find os name
int cpu_max_freq(void); // find frquency in GHz
struct mem_info * m_info(void); // fill the struct mem_info

#endif
