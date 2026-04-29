#ifndef SNAP_H
#define SNAP_H

#include "dynarray/dynarray.h"
/* ABSOLUTE PATH */
/* directory: contains .snap package files */
#define SNAP_PATH_DIR "/var/lib/snapd/snaps"

// prototype function
int count_pkg_snap();
int is_snap_file(const char *file_name);
int is_reg_file(const char *full_path_file);
char *absolute_file_name(char *file_name);
//void add_packages_to_list( char *result ,char ***pkg_list, int pkg);

 /*
  * cut the name_verion.snap
  * became name\0version\0snap
  * so beacme separe string
  * leave only the package name of the file
 */
int is_duplicate_file(char *file_name, dynarray_t *list);
#endif
