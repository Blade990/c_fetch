#include "flatpak.h"

#include <stdio.h>
#include <stdlib.h>

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
