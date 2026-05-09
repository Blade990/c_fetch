fetch: main.o os_info.o cpu_info.o packages.o mem_info.o disk_info.o flatpak.o apt.o snap.o pacman.o print.o
	gcc main.o os_info.o cpu_info.o disk_info.o mem_info.o packages.o apt.o flatpak.o snap.o pacman.o print.o -o fetch

main.o: main.c os_info.h cpu_info.h packages.h print.h disk_info.h mem_info.h
	gcc -c main.c

os_info.o: os_info.c os_info.h
	gcc -c os_info.c

disk_info.o: disk_info.c disk_info.h
	gcc -c disk_info.c

cpu_info.o: cpu_info.c cpu_info.h
	gcc -c cpu_info.c

packages.o: packages.c packages.h
	gcc -c packages.c

mem_info.o: mem_info.c mem_info.h
	gcc -c mem_info.c

flatpak.o: flatpak.c flatpak.h
	gcc -c flatpak.c

snap.o: snap.c snap.h
	gcc -c snap.c

apt.o: apt.c apt.h
	gcc -c apt.c

print.o: print.c print.h os_info.h packages.h cpu_info.h
	gcc -c print.c

pacman.o: pacman.c pacman.h
	gcc -c pacman.c

clean:
	rm -f *.o fetch
