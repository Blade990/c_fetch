#ifndef FETCH_H
#define FETCH_H
#include <sys/utsname.h>

/* package managers */
/* ABSOLUTE PATH */
/* directory: each subdirectory = one installed package */
#define PACMAN_PATH "/var/lib/pacman/local"
/* ABSOLUTE PATH */
/* file: must be parsed searching for "install ok installed" */
#define DPKG_STATUS_FILE "/var/lib/dpkg/status"

/* sandbox package managers */
/* ABSOLUTE PATH */
/* directory: each directory = one flatpak application */
#define FLATPAK_SYS_DIR "/var/lib/flatpak/app"
/* RELATIVE PATH (relative to $HOME) */
/* directory: $HOME/.local/share/flatpak/app */
#define FLATPAK_USER_DIR ".local/share/flatpak/app"
/* ABSOLUTE PATH */
/* directory: contains .snap package files */
#define SNAP_DIR "/var/lib/snapd/snaps"


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

// struct that identify how many packages are on distro and which pacakge manager control them (pacman, apt, dnf) on different distro
struct packages{
    int pkg_manager; // counter of package for distro's package manager on machine
    int pkg_flatpak; // counter of packae for flatpak package on machine
    int pkg_snap;  // counter of package for snap package on machine
};

//prototype function
struct cpu *cpu_info(void); // fill cpu struct fields
struct disk_space *disk_usage(void);// fill storage memory (disk) struct fields
char * kernel_info(void); // find kernel version
char * os_name_inf(void); // find os name
int cpu_max_freq(void); // find frquency in GHz
struct mem_info * m_info(void); // fill the struct mem_info

// package manager function
struct packages * get_packages(void); // fill the struct packages fields
int count_pkg_dir(const char *path);  // for all the package and sandbox managaer he count the installed package
void pkg_manager_field(struct packages *p);
void pkg_flatpak_field(struct packages *p);
void pkg_snap_field(struct packages *p);




#endif // end of the header file
