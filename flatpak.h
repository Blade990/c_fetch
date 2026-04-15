#ifndef FLATPAK_H
#define FLATPAK_H

#include "packages.h" //  implement count_pkg_dir function  and  packages structure

/* sandbox package managers */
/* ABSOLUTE PATH */
/* directory: each directory = one flatpak application */
#define FLATPAK_SYS_DIR "/var/lib/flatpak/app"
/* RELATIVE PATH (relative to $HOME) */
/* directory: $HOME/.local/share/flatpak/app */
#define FLATPAK_USER_DIR ".local/share/flatpak/app"

void pkg_flatpak_field(struct packages *p);

#endif
