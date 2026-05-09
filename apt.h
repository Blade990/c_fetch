#ifndef APT_H
#define APT_H

#define DPKG_STATUS_PATH "/var/lib/dpkg/status"

#define APT_INSTALLED_MAGIC "Status: install ok installed"



/* Prototype */
int get_apt_pkg(void);
#endif
