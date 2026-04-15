#include "pacman.h"

#include <dirent.h>
#include <string.h>

// count dir in pacman path but avoid sys file
int pacman_pkg_count(const char *path){
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
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
            continue;
        }

        // count only the dirs which have at least one - between such as bash-2.5.0-1
        if(strchr(entry->d_name,'-') != NULL){
            i++;
        }
    }
    // close the directory stream and free resources
    closedir(cur_dir);
    return i;
}
