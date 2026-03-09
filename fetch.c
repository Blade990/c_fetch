// include headers for base functions
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/utsname.h> // include the struct for the info on kernel
#include "fetch.h"
#include <dirent.h> //include to work with dir function
#include <sys/statvfs.h> // include the following struct of filesystem :

/*
struct statvfs {
     unsigned long f_bsize;    // Dimensione dei blocchi (in byte)
     unsigned long f_frsize;   // Dimensione del blocco fondamentale (in byte)
     unsigned long f_blocks;   // Numero totale di blocchi
     unsigned long f_bfree;    // Numero di blocchi liberi
     unsigned long f_bavail;   // Numero di blocchi liberi disponibili per l'utente
     unsigned long f_files;    // Numero totale di file
     unsigned long f_ffree;    // Numero di file liberi
     unsigned long f_favail;   // Numero di file liberi disponibili per l'utente
     unsigned long f_fsid;     // Identificatore del file system
     unsigned long f_flag;     // Flags del file system
     unsigned long f_namemax;  // Lunghezza massima di un nome di file
 };
 */
//macro and const for printf's look
#define RESET   "\033[0m" // Macro to reset the text color to default (terminal default color)
#define CYAN    "\033[36m"
#define BLUE    "\033[34m"
#define GREEN   "\033[32m"

/* prototype of printf look
 printf(BLUE "╭──────────────────────────────╮\n" RESET);
 printf(BLUE "│ " CYAN "Sistema" RESET "   : %s\n", info.sysname);
 printf(BLUE "│ " CYAN "Hostname" RESET "  : %s\n", info.nodename);
 printf(BLUE "│ " CYAN "Kernel" RESET "    : %s\n", info.release);
 printf(BLUE "│ " CYAN "Arch" RESET "      : %s\n", info.machine);
 printf(BLUE "╰──────────────────────────────╯\n" RESET);
 */
int main (void){
    char *kernel_ver = NULL;
    char *os_name = NULL;
    long int full_part, decimal_part; // to represnt in detail without loss of information
    struct cpu *cpu = NULL;
    struct disk_space *disk = NULL;
    struct mem_info *mem_info = NULL;
    struct packages *pkg_info = NULL;

    //this sereis of if control every failed and variables was returred with success then we can print infomation retruned on screen
    kernel_ver = kernel_info();
    if (!kernel_ver) {
        fprintf(stderr, "kernel_info failed\n");
        goto cleanup;
    }

    os_name = os_name_inf();
    if (!os_name) {
        fprintf(stderr, "os_name_inf failed\n");
        goto cleanup;
    }

    cpu = cpu_info();
    if (!cpu) {
        fprintf(stderr, "cpu_info failed\n");
        goto cleanup;
    }

    disk = disk_usage();
    if(!disk) {
        fprintf(stderr, " diskusage() failed\n");
        goto cleanup;
    }

    mem_info = m_info();
    if(!mem_info) {
        fprintf(stderr, "Errore recuperare mem inf failed\n");
        goto cleanup;
    }

    pkg_info = get_packages();
    if(!pkg_info){
        fprintf(stderr, "Error, something went wrong trying to get number packages");
        goto cleanup;
    }
    printf(BLUE "╭──────────────────────────────╮\n" RESET);
    printf(BLUE "| " CYAN "%-9s" RESET " : %s\n", "Sistema", os_name);
    printf(BLUE "| " CYAN "%-9s" RESET " : %s\n", "Kernel", kernel_ver);
    printf(BLUE "| " CYAN "%-9s" RESET " : %s\n", "CPU model", cpu->core_model);
    printf(BLUE "| " CYAN "%-9s" RESET " : %d\n", "CPU cores", cpu->core_numb);
    printf(BLUE "| " CYAN "%-9s" RESET " : %.2f GB\n","Available Space User (disk /home)", disk->available_space);
    printf(BLUE "| " CYAN "%-9s" RESET " : %.0f GB\n","Mem Free Tot",mem_info->mem_free);
    printf(BLUE "| " CYAN "%-9s" RESET " : %.0f GB\n","Mem Used",mem_info->mem_used);
    printf(BLUE "| " CYAN "%-9s" RESET " : (flatpak apps) %d | (pacman) %d | (snap) %d\n","Packages",pkg_info->pkg_flatpak,pkg_info->pkg_manager,pkg_info->pkg_snap);
    // Convert CPU speed in centi-GHz (hundredths of GHz) to a human-readable format
    // full_part  -> integer part of the GHz
    // decimal_part -> fractional part (2 digits) of the GHz
    full_part = cpu->max_freq / 100; // full part
    decimal_part = cpu->max_freq % 100;//decimal part
    printf(BLUE "| " CYAN "%-9s" RESET " : %ld.%02ld GHz\n","CPU Max Frequency", full_part, decimal_part);
    // use free function after visulization of field and variables we can free the memory
    cleanup:
    free(kernel_ver);
    free(os_name);
    free(mem_info); // we're freeing the mmeory of the struct mem_info
    free(disk); // we're freeing the mmeory of the struct disk
    free(pkg_info);// free
    if(cpu) {
       free(cpu->core_model);
       free(cpu);
    }

    return 0;
}

char * kernel_info(void) {
    struct utsname info;
    char *kernel;
    if ( uname(&info) != 0) {
        printf("Errore, impossibile trovare la struct!!!\n");
        return NULL;
    }
    kernel = malloc( strlen(info.release) + 1);
    //function strcpy(dest, src)
    strcpy(kernel, info.release);
    return kernel;
}

char * os_name_inf(void) {
    char *os;
    char tmpbuff[200];
    const char *pretty_name = "PRETTY_NAME=\"";

    //open a the file where is the name of OS in read mode
    FILE *fp = fopen("/etc/os-release", "r");
    if(fp == NULL) {
        perror("Errore apertura file\n");
        return NULL;
    }

    //fgets read and save in var tmpbuff every new line he do a free and then restart with new line
    while(fgets(tmpbuff, sizeof(tmpbuff), fp)) {
        //with strstr function we compare two string and if the string is what we want  enter the if
        if(strstr( tmpbuff, pretty_name )) {
            //assign to ptr th address of tmpbuff with index of the string we want (skip the rest)
            os = &tmpbuff[strlen(pretty_name)];

            //in this part we intialize a ptr for a single char and with
            // if and strchr if there is the char we want to remove we overwrite the char in the string
            char *cut_apice = NULL;
            if ( ( cut_apice = strchr(os,'\"') ) != NULL ) {
                //sostitution of " with char end of string \0
                *cut_apice = '\0';
            } //close  innner if
            //in this part we can avoid t use malloc using strdup instead,
            // to allocate memory and
            // use the return os outside this function
            os = strdup(os);
        }// close outer if
    }

    fclose(fp);
    return os;
}

// This function read file to take the real seed of cpu and return it to cpu_info
int cpu_speed(){
    long int cpu_cur_freq;//var which bring the current cpu_speed of cpu
    int valid = 1; // var that check if the string is a real number 1 ok, instead 0 error
    FILE *fp;
    char tmpbuff[100];
    fp =fopen( "/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq", "r");
    // check if the file open corretly
    if(!fp) {
        perror(" Errore apertura file /sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq: ");
        return -1;
    }

    // read the entire number which represent the frequency of cpu
    if(fgets(tmpbuff,sizeof(tmpbuff), fp)) {
        // cancel form buffer \n at the end string cleaning
        tmpbuff[strcspn(tmpbuff, "\n")] = '\0';

        // DEBUG: controlla cosa legge fgets
        // printf("DEBUG: tmpbuff = '%s'\n", tmpbuff);
        // // !!security check if every element in tmp buff is a number with ascii code
        for(size_t i = 0 ; i < strlen(tmpbuff) ; i++) {

            // it must be between char "0" and '9'
            // we check if there is at least one char in string
            // which is not a number then we can't enter in if(valid)
            if((tmpbuff[i] < '0' || tmpbuff[i] > '9')){
                valid = 0;
            }
        } //close for

        if(!valid) {
            fprintf(stderr, "Trovato carattere non compatibile\n");
            fclose(fp);
            return -1;
        } else {
            // assign the converted value
            cpu_cur_freq = atol(tmpbuff); // unit measure Khz
            // conversion form Khz to Ghz of current frequency
            cpu_cur_freq = cpu_cur_freq / 10000;
        } // end of if else
    }

    fclose(fp);
    return cpu_cur_freq;
}

struct cpu * cpu_info() {
    struct  cpu * cpu ; //tempporary ptr to the struct we want info
    char *cpu_model ="model name"; // string we take a sreference to find the string we want
    char *cpu_core_numb = "cpu cores";
    char *cpu_find_field; //string we want  to find
    char tmpbuff[200]; // temporary buffer where we save the string we are reading
    FILE *fp; // ptr to the file we read to  take the string

    // Open /proc/cpuinfo, which contains information about the CPU.
    // If the file cannot be opened, we cannot continue.
    fp = fopen("/proc/cpuinfo","r");
    if(fp == NULL){
        perror("!! Errore nell'apertura del file\n");
        return NULL;
    }

    // Allocate memory for our CPU structure. We will fill it with data.
    cpu = malloc( sizeof( struct cpu));
    // check if struct  is allocated
    if(!cpu) {
        perror("La struttura non è stata allocata corretamente casua: ");
        return NULL;
    }

    // Read the file line by line. Each call to fgets() reads a single line
    // and stores it into 'tmpbuff'.
    while(fgets( tmpbuff, sizeof(tmpbuff), fp)) {
        // Debug: Print the current line
        // printf("Line: %s\n", tmpbuff);  // This will print every line being read
        // Check if this line contains the text "model name".
        // ----------------------------
        // PARSE CPU MODEL
        // ----------------------------
        if(strstr (tmpbuff, cpu_model)) {
            // Move a pointer to the position right after "model name".
            // After that text, there are usually spaces and a colon.
            // base addr of tmpbuff + offset of the dirty string  we want
            cpu_find_field = tmpbuff + strlen(cpu_model);
            //use while to remove useless symbols frm the string such as : or space example " : intel-i7" became "intel-i7"
            // we read one single char at the time in the while and remove the useless char
            while( *cpu_find_field == ' ' || *cpu_find_field == ':' || *cpu_find_field == '\t') {
                //go throught the string avoid char
                cpu_find_field++;
            }
            //after the elimination off useless char we remove the final special char of nw line '\n'
            // address + offset with strcspn return the posistion of the char it's lilke arr[i]
            cpu_find_field[strcspn( cpu_find_field, "\n")] = '\0';

            // now we have the "clean" string we can allocate memory for the field of cpu cpu_name
            cpu->core_model = malloc(strlen(cpu_find_field) + 1); // always leave psace for char '\0'
            // after allocation, now we can copy in the field of the struct cpu with strcp
            strcpy(cpu->core_model, cpu_find_field);
        } // close first if after while

        // ----------------------------
        // PARSE CORE COUNT
        // ----------------------------
        if(strstr( tmpbuff, cpu_core_numb)) {
            cpu_find_field = tmpbuff + strlen(cpu_core_numb);

            while( *cpu_find_field == ' ' || *cpu_find_field == ':' || *cpu_find_field == '\t') {
                //go throught the string avoid useless char
                cpu_find_field++;
            }
            cpu_find_field[strcspn( cpu_find_field, "\n")] = '\0';

            //check if 2 is in the var's value between 0 e 9
            if(*cpu_find_field >= '0' && *cpu_find_field <= '9') {
                // save the string of number core in core_numb as an int with a casting ascii value of char
                cpu->core_numb = (int)(*cpu_find_field - '0');
            } else {
                perror("Errore nell assegnazione del campo");
                return NULL;
            }
        } // close second if
    } // close while

    // call function that gives cpu speed and fill the field of cpu struct
    cpu->max_freq = cpu_speed();
    // don't need anymore the file /proc/cpuinfo
    fclose(fp);
    //all finished corectly, we can close the file
    return cpu;
}

struct disk_space* disk_usage(void) {
    struct disk_space *disk; // struct that bring our inf on usage of the disk
    struct statvfs fs_info; // strcut include with the header
    const double GB = 1024.0 * 1024.0 * 1024.0; // var for conversion of the operation from byte to GB 1024^3
    char *path_user =  getenv("HOME");  // Retrieve the current user's home directory

    // if var not find print the error
    if (path_user == NULL) {
        fprintf(stderr, "Error: HOME environment variable is not defined.\n");
        return NULL;
    }

    // Call statvfs() to retrieve filesystem statistics.
    // This function fills 'fs_info' with information such as block size,
    // total blocks, free blocks, and blocks available to the unprivileged user.
    //
    if(statvfs( path_user, &fs_info) != 0) {
        // If statvfs() fails, it sets errno.
        // perror() prints a human-readable error message based on errno.
        perror("stasfv() failed");
        return NULL;
    }

    // allocate space for the struct
    disk = malloc( sizeof(struct disk_space));

    // the unit measure of the following operation si in byte

    // Calculate the total size of the filesystem in bytes.
    // f_blocks  = total number of blocks in the filesystem
    // f_frsize  = fundamental block size (the size of each block in bytes)
    // Total bytes = number of blocks * size of each block
    disk->total_space = (double )(fs_info.f_blocks * fs_info.f_frsize) / GB;

    // Calculate the total free space in bytes (including root-reserved blocks).
    // f_bfree = all free blocks in the filesystem (even those reserved for root)
    // f_frsize Dimension of a "block unit" of filesystem.
    // Free bytes = free blocks * size of each block
    disk->free_space = (double) (fs_info.f_bfree * fs_info.f_frsize) / GB;

    // Calculate the free space available to a regular (non-root) user in bytes.
    // f_bavail = free blocks available to unprivileged users (excludes root-only blocks)
    // Usable bytes = user-available blocks * size of each block
    disk->available_space = (double) (fs_info.f_bavail * fs_info.f_frsize) / GB;

    return disk;
}

struct mem_info * m_info(void) {
    struct mem_info *info;
    FILE *fp = NULL;
    char tmpbuff[104];
    char * find_mem;
    char * f_mem_total = "MemTotal";
    char * f_mem_free = "MemFree";
    char * f_mem_available = "MemAvailable";

    fp = fopen("/proc/meminfo", "r");
    if(!fp) {
        perror("File non aperto correttamente dio cane ");
        return NULL;
    }

    info = malloc(sizeof( struct mem_info));
    while(fgets(tmpbuff, sizeof(tmpbuff), fp)) {
        if(strstr(tmpbuff, f_mem_free)) {
            // jump to the string we are interested about
            find_mem = &tmpbuff[strlen(f_mem_free)] ;

            // if find anyoneof this symbols jump
            while( *find_mem == '\t' ||  *find_mem == ':' || *find_mem == ' ') {
                find_mem++;
            }
            // offset of char \n in the string to override it
            find_mem[strcspn(find_mem, "\n")] ='\0';
            // use function strod to make a casting from string to double
            info->mem_free = strtod(find_mem,NULL);
            //first conversion from KB to GB
            info->mem_free = info->mem_free / (1024 * 1024);
        } //close first if to find mem free

        if(strstr(tmpbuff, f_mem_available)) {
            // jump to the string we are interested about
            find_mem = &tmpbuff[strlen(f_mem_available)] ;
            // if find anyoneof this symbols jump
            while( *find_mem == '\t' ||  *find_mem == ':' || *find_mem == ' ') {
                find_mem++;
            }

            // offset of char \n in the string to override it
            find_mem[strcspn(find_mem, "\n")] ='\0';
            info->mem_avi = strtod(find_mem,NULL);

            // second conversion from KB to GB
            info->mem_avi = info->mem_avi / (1024 * 1024);
        } //close second if to find mem Available

        if(strstr(tmpbuff, f_mem_total)) {
            // jump to the string we are interested about
            find_mem = &tmpbuff[strlen(f_mem_total)] ;
            // if find anyoneof this symbols jump
            while( *find_mem == '\t' ||  *find_mem == ':' || *find_mem == ' ') {
                find_mem++;
            }
            // offset of char \n in the string to override it
            find_mem[strcspn(find_mem, "\n")] ='\0';
            info->mem_tot = strtod(find_mem,NULL);

            //third conversion from KB to G from KB to GB
            info->mem_tot = info->mem_tot / (1024 * 1024);
        } //close third  if to find mem Available
    }
    // calculating through formula Memory used (RAM) = Memmory total - Memory available (GB)
    info->mem_used = info->mem_tot - info->mem_avi;
    fclose(fp);
    return info;
}

// series of funtion to read and get packages installed and package manager nad other (flatpak, snap)
struct packages * get_packages(void){
    struct packages *p;

    p = malloc(sizeof(struct packages));
    // control if p was correctly allocated
    if(!p){
        return NULL;
    }

    //initialize structure package to 0
    p->pkg_flatpak = 0;
    p->pkg_manager = 0;
    p->pkg_snap = 0;

    // start to check how many packages there are for any manager
    //pkg_manager_field(p);
    pkg_flatpak_field(p);
    //pkg_snap_field(p);

    return p;
}

/*void pkg_manager_field(struct packages *pkg){

    //pkg->pkg_manager = count_pkg(PKG_MANAGER_PATH);
}*/

void pkg_flatpak_field(struct packages *pkg){

    // count system flatpak applications
    pkg->pkg_flatpak = count_pkg_dir(FLATPAK_SYS_DIR);
    // get the HOME directory of the current user
    char *home = getenv("HOME");

    // if home exist build the absolute path starting from HOME
    if(home){

        char path[512];
        // build: /home/user/.local/share/flatpak/app
        snprintf(path, sizeof(path), "%s/%s", home, FLATPAK_USER_DIR);
        // sum to know all the package of FLATPAK_SYS_DIR + FLATPAK_USER_DIR
        pkg->pkg_flatpak += count_pkg_dir(path);
    }
}

/*void pkg_snap_field(struct packages *pkg){

    //pkg->pkg_snap = count_pkg(SNAP_PATH);
}*/

//package counter function
int count_pkg_dir(const char *path){
    // counter for packages found inside the directory
    int i = 0;
    //structure used to store each directory entry (file or folder)
    struct dirent * entry;

    //open the directory specified by "path"
    //opendir() returns a pointer to DIR if successful
    DIR *cur_dir = opendir(path);

    //if the directory cannot be opened (not found or permission denied)
    // return 0 packages
    if(!cur_dir){
        return 0;
    }

    // read directory entries one by one
    // readdir() returns NULL when no more entries exist
    while((entry = readdir(cur_dir)) != NULL){

        // skip "." (current directory) and ".." (parent directory) if found
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){

            continue;
        }

        i++;
    }
    // close the directory stream and free resources
    closedir(cur_dir);

    return i;
}
