#include "apt.h"

#include <stdio.h>
#include <string.h>

int get_apt_pkg(){
    char tmpbuff[256];
    int pkg_cnt = 0;
    FILE *fp = NULL;

    fp = fopen(DPKG_STATUS_PATH, "r");

    if(!fp){
        return 0;
    }

    while(fgets(tmpbuff,sizeof(tmpbuff),fp)){

        if(strncmp(tmpbuff, APT_INSTALLED_MAGIC,strlen(APT_INSTALLED_MAGIC)) == 0){
            pkg_cnt++;
        }
    }

    fclose(fp);
    return pkg_cnt;
}
