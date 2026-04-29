#include "packages.h"
#include "flatpak.h"
#include "pacman.h"
#include "snap.h"

#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>


struct packages * get_packages(void){
    struct packages *p;
    pkg_manager_t pkg;
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
    pkg = check_pkg_manager_type();

    switch (pkg){
        /*case PKG_APT:
            p->pkg_manager = count_pkg_file(DPKG_PATH_FILE);
            break;*/
        /*case PKG_DNF:
            pkg_count_manager_field(DNF_COUNT_CMD,p);
            //break;*/
        case PKG_PACMAN:
            p->pkg_manager = pacman_pkg_count(PACMAN_PATH);
            break;
        case PKG_UNKNOWN:
            break;
    }

    pkg_flatpak_field(p);
    p->pkg_snap = count_pkg_snap();

    return p;
}

pkg_manager_t check_pkg_manager_type(void){

    // series of if to check whiwh binary path exist
    // access return 0 if file exists and is have exectutable permission
    // check if pacman exist
    if(access(PACMAN_BIN_PATH, X_OK) == 0){
        return PKG_PACMAN;
    }

    // check if apt exist
    /*if(access(APT_BIN_PATH,X_OK) == 0){
        return PKG_APT;
        }*/

    // check if dnf exist
    /*if(access(DNF_BIN_PATH,X_OK) == 0){
        return PKG_DNF;
        }*/

    return PKG_UNKNOWN;
}

//package counter function for directory
int count_pkg_dir(const char *path) {
    // counter for packages found inside the directory
    int i = 0;
    //structure used to store each directory entry (file or folder)
    struct dirent *entry;

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
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        i++;
    }
    // close the directory stream and free resources
    closedir(cur_dir);
    return i;
}
