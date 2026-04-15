#ifndef PACMAN_H
#define PACMAN_H

/*CONST PATH BINARY FILE PAKCAGE MANAGERS */
/*to check every path to find the right one for every distro */
/* Path to the main pacman binary (package manager used by Arch Linux and derivatives) */
#define PACMAN_BIN_PATH "/usr/bin/pacman"

/* package managers */
/* ABSOLUTE PATH PACMAN*/
/* directory: each subdirectory = one installed package */
#define PACMAN_PATH "/var/lib/pacman/local"


//prototype function
int pacman_pkg_count(const char * path);
#endif
