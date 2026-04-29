#define DYNARR_IMPL // Definisce l'implementazione della libreria in questo file
#include "dynarray/dynarray.h"
#include "snap.h"
//#include "packages.h"

#include <stdio.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>


// Funzione principale per contare i pacchetti .snap
int count_pkg_snap() {
    int pkg_count = 0;
    DIR *snap_dir = NULL;
    struct dirent *entry = NULL;
    char full_path_file[1024];
    char *cleaned_name = NULL;

    // 1. Inizializziamo la dynarray per contenere puntatori a stringhe (char *)
    dynarray_t list = danew(sizeof(char *));

    snap_dir = opendir(SNAP_PATH_DIR);
    if (!snap_dir) {
        return 0;
    }

    while ((entry = readdir(snap_dir)) != NULL) {
        // Escludiamo le directory . e ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Costruiamo il percorso completo per il controllo stat
        snprintf(full_path_file, sizeof(full_path_file), "%s/%s", SNAP_PATH_DIR, entry->d_name);

        if (is_reg_file(full_path_file) != 1) {
            continue;
        }

        // Se il file ha estensione .snap
        if (is_snap_file(entry->d_name) == 0) {
            cleaned_name = absolute_file_name(entry->d_name);

            // 2. Controllo duplicati usando la nuova funzione
            if (!is_duplicate_file(cleaned_name, &list)) {

                // 3. Aggiunta alla lista: ridimensionamento e inserimento
                // Qui la logica sostituisce la vecchia 'add_packages_to_list'
                size_t current_size = daobjects(&list);
                if (darealloc(&list, current_size + 1) == 0) {
                    char *name_copy = strdup(cleaned_name);
                    daset(&list, current_size, &name_copy);
                    pkg_count++;
                }
            }
            free(cleaned_name);
        }
    }

    // 4. Cleanup: liberiamo le stringhe allocate con strdup
    for (size_t i = 0; i < daobjects(&list); i++) {
        char **ptr = (char **)daget(&list, i);
        if (ptr && *ptr) {
            free(*ptr);
        }
    }

    // 5. Liberiamo la struttura dynarray e chiudiamo la directory
    dafree(&list);
    closedir(snap_dir);

    return pkg_count;
}

/*
 * Controlla se il nome è già presente nella dynarray
 */
int is_duplicate_file(char *file_name, dynarray_t *list) {
    if (!list || !davalid(list)) return 0;

    for (size_t i = 0; i < daobjects(list); i++) {
        char **stored_name = (char **)daget(list, i);
        if (stored_name && *stored_name) {
            if (strcmp(file_name, *stored_name) == 0) {
                return 1; // Duplicato trovato
            }
        }
    }
    return 0;
}

/*
 * Verifica se è un file regolare
 */
int is_reg_file(const char *full_path_file) {
    struct stat st;
    if (stat(full_path_file, &st) != 0) return -1;
    return S_ISREG(st.st_mode) ? 1 : 0;
}

/*
 * Verifica l'estensione .snap
 */
int is_snap_file(const char *file_name) {
    const char *dot = strrchr(file_name, '.');
    if (dot && strcmp(dot, ".snap") == 0) return 0;
    return 1;
}

/*
 * Estrae il nome pulito (es: vlc_123.snap -> vlc)
 */
char *absolute_file_name(char *file_name) {
    char *copy = strdup(file_name);
    char *underscore = strchr(copy, '_');
    if (underscore) {
        *underscore = '\0';
    } else {
        char *dot = strrchr(copy, '.');
        if (dot) *dot = '\0';
    }
    return copy;
}
