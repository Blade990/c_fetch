#ifndef PACKAGES_H
#define PACKAGES_H

/* Path to the main dnf binary (package manager used by Fedora, RHEL and derivatives) */
#define DNF_BIN_PATH "/usr/bin/dnf"

/* Path to the main apt binary (APT frontend used by Debian, Ubuntu and derivatives) */
#define APT_BIN_PATH "/usr/bin/apt"

// enum  that help to identify how many packages are on distro and which pacakge manager control them (pacman, apt, dnf) on different distro
typedef enum {
    PKG_PACMAN,
   // PKG_DNF,
   // PKG_APT,
    PKG_UNKNOWN
   } pkg_manager_t;

// struct that identify how many packages are on distro and which pacakge manager control them (pacman, apt, dnf) on different distro
struct packages{
    int pkg_manager; // counter of package for distro's package manager on machine
    int pkg_flatpak; // counter of packae for flatpak package on machine
    int pkg_snap;  // counter of package for snap package on machine
};

// prototype function
struct packages * get_packages(void); // fill the struct packages fields
int count_pkg_dir(const char *path);  // for all the package and sandbox managaer he count the installed package
pkg_manager_t  check_pkg_manager_type(void); //find what kind of pkg manager is check the binary file
#endif
