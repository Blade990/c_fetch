#include "mem_info.h"
#include "dynarray/dynarray.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

mem_info_t* get_mem_info(void){
    mem_info_t *mem_info = NULL;
    dynarray_t ds_struct = danew(sizeof(mem_info_t));

    if(darealloc(&ds_struct, 1)){
        return NULL;
    }

    mem_info = (mem_info_t*)ds_struct.ptr;

    mem_info->available = get_mem_available();
    mem_info->total= get_mem_total();

    mem_info->used = mem_info->total - mem_info->available;

    if(mem_info->total > 0.0){
        mem_info->used_perc = (mem_info->used / mem_info->total)* 100.0;

        mem_info->free_perc = 100.0 - mem_info->used_perc ;
    }else{
        mem_info->used_perc = 0.0;
        mem_info->free_perc = 0.0;
    }

    return mem_info;
}

double get_mem_available(){
    double avail_mem_info = 0.0;
    char *find = NULL;
    FILE *fp = NULL;
    char tmpbuff[256];

    fp = fopen(MEM_INFO_PATH,"r");

    if(!fp){
        return 0.0;
    }

    while(fgets(tmpbuff, sizeof(tmpbuff), fp)){

        if(strstr(tmpbuff,KEY_MEM_AVAIL)){
            find = strchr(tmpbuff, ':');
            if(find){
                find ++;
                avail_mem_info = atof(find) * CONV_GB;
            }
            break;
        }
    }

    fclose(fp);
    return avail_mem_info;
}

double get_mem_total(void){
    double total_mem_info = 0.0;
    FILE *fp = NULL;
    char tmpbuff[256];
    char *find = NULL;


    fp = fopen(MEM_INFO_PATH, "r");

    if(!fp){
        return total_mem_info;
    }

    while (fgets(tmpbuff, sizeof(tmpbuff), fp)){

        if(strstr(tmpbuff, KEY_MEM_TOT)){
            find = strchr(tmpbuff, ':');
            if(find){
                find++;
                total_mem_info = atof(find) * CONV_GB;
            }
            break;
        }
    }

    fclose(fp);
    return total_mem_info;
}

void free_mem_info(mem_info_t *mem_data){
    dynarray_t ds;
    if(!mem_data){
        return;
    }

    ds.ptr = mem_data;

    dafree(ds.ptr);
}
