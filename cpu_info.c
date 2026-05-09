#include "cpu_info.h"
#include "dynarray/dynarray.h"


 /* Standard C error handling, replaces <cerrno> */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

cpu_info_t* get_cpu_info(void){
    cpu_info_t *info = NULL;
    dynarray_t da_struct = danew(sizeof(cpu_info_t));
    /*
     * allocate
     * space for the strcut and
     * check if it's gone right
     */
    if(darealloc(&da_struct, 1) != 0){
        return NULL;
    }
    /*
     * assign to the cpu_info *info struct
     * the field ptr of the strcut dynarry
     * casting needed ptr in dynarry is void
     * ptr is  the filed which point to
     * fisrt addres of the struct
     */
    info = (cpu_info_t *)da_struct.ptr;

    // initialize the field of info
    info->core_model =get_cpu_model();
    info->core_numb = get_cpu_cores();
    info->max_freq = get_cpu_freq();

    return info;
}

/* Helper function: extracts the CPU model name using dynarray */
char* get_cpu_model(void){
    char line[256];
    char *model_ptr = NULL;
    char *find = NULL;
    size_t str_size = 0;
    size_t len = 0;
    dynarray_t da_model;

    FILE *fp = fopen( CPU_INFO_PATH,"r");
    if(!fp){
        return NULL;
    }

    while(fgets(line, sizeof(line), fp)){
        if(strstr(line, CPU_MODEL_KEY)){
            // Skip the colon character
            find = strchr(line,':');
            if(find){
                find++;
                // Skip leading spaces and tabs after the colon
                while( *find == ' ' || *find == '\t'){
                    find++;
                }

                size_t len = strlen(find);
                if(len > 0 && find[len -1] == '\n'){
                    find[len -1] = '\0';
                    len--;
                }

                str_size = len + 1;
                da_model = danew(S8);
                if (darealloc(&da_model, str_size) == 0) {
                     /* Copy the clean string into the dynarray memory using strcpy */
                    strcpy((char *)da_model.ptr, find);
                    model_ptr = (char *)da_model.ptr;
                }

                break; /* Target string found and stored, exit loop */
            }
        }
    }
    fclose(fp);
    return model_ptr;
}

/* Helper function: retrieves CPU maximum frequency */
size_t get_cpu_freq(void) {
    /* --- VARIABLE DECLARATIONS --- */
    size_t freq_mhz;        /* Final frequency value in MHz */
    char line[256];         /* Buffer to read line for fallback parsing */
    char *find;              /* Pointer used to find the colon in fallback */
    unsigned long freq_khz; /* Temporary variable to read raw value from sysfs */
    FILE *freq_fp;          /* File pointer for the sysfs max frequency file */
    FILE *fp;               /* File pointer for the fallback /proc/cpuinfo */

    /* --- INITIALIZATIONS --- */
    freq_mhz = 0;

    /* Attempt 1: Read the exact hardware max frequency from sysfs */
    freq_fp = fopen(CPU_MAX_FREQ_PATH, "r");

    /* If sysfs file is accessible, parse the frequency in KHz */
    if (freq_fp) {
        freq_khz = 0;

        /* If read succeeds, convert KHz to MHz */
        if (fscanf(freq_fp, "%lu", &freq_khz) == 1) {
            freq_mhz = freq_khz / 1000;
        }
        fclose(freq_fp);
    }

    /* Attempt 2: Fallback to /proc/cpuinfo if sysfs was unavailable */
    if (freq_mhz == 0) {
        fp = fopen(CPU_INFO_PATH, "r");

        /* If fallback file also cannot be opened, return 0 */
        if (!fp) {
            return 0;
        }

        /* Scan fallback file line by line */
        while (fgets(line, sizeof(line), fp)) {

            /* Check if current line contains the frequency key */
            if (strstr(line, CPU_FREQ_KEY)) {
                find = strchr(line, ':');

                /* If colon found, convert the string value to double/size_t */
                if (find) {
                    freq_mhz = (size_t)strtod(find + 1, NULL);
                }
                /* Frequency extracted from fallback, exit the loop */
                break;
            }
        }
        fclose(fp);
    }

    return freq_mhz;
}

/* Helper function: counts the total number of CPU cores */
size_t get_cpu_cores(void) {
    /* --- VARIABLE DECLARATIONS --- */
    char line[256]; /* Buffer to read each line from the file */
    size_t cores;   /* Counter for matching CPU processor lines */
    FILE *fp;       /* File pointer for /proc/cpuinfo */

    /* --- INITIALIZATIONS --- */
    cores = 0;

    /* Open the CPU information file in read-only mode */
    fp = fopen(CPU_INFO_PATH, "r");

    /* If the file fails to open, return a fallback of 1 core */
    if (fp == NULL) {
        return 1;
    }

    /* Read the file line by line until the end is reached */
    while (fgets(line, sizeof(line), fp) != NULL) {

        /* Check if the current line starts exactly with the key "processor" */
        if (strncmp(line, CPU_CORES_KEY, strlen(CPU_CORES_KEY)) == 0) {
            cores = cores + 1; /* Explicitly increment the counter without operators like ++ */
        }
    }

    /* Close the file stream to release resources */
    fclose(fp);

    /* Verify if we found any cores; if not, return the fallback value of 1 */
    if (cores > 0) {
        return cores;
    } else {
        return 1;
    }
}

void free_cpu_info(cpu_info_t *info) {
    /* --- VARIABLE DECLARATIONS --- */
    dynarray_t da;        /* Dynarray to wrap the internal string pointer */
    dynarray_t da_struct; /* Dynarray to wrap the main structure pointer */

    /* If the passed pointer is NULL, there's nothing to free */
    if (!info) {
        return;
    }

    /* Free the internal model string if it was allocated */
    if (info->core_model) {
        da.ptr = info->core_model;
        da.ts = S8;
        dafree(&da);
    }

    /* Free the main structure container itself */
    da_struct.ptr = info;
    da_struct.ts = sizeof(cpu_info_t);
    dafree(&da_struct);
}
