fetch: main.o system.o packages.o flatpak.o pacman.o print.o
	gcc main.o system.o packages.o flatpak.o pacman.o print.o -o fetch

main.o: main.c
	gcc -c main.c

system.o: system.c
	gcc -c system.c

packages.o: packages.c
	gcc -c packages.c

flatpak.o: flatpak.c
	gcc -c flatpak.c

print.o: print.c
	gcc -c print.c

pacman.o: pacman.c
	gcc -c pacman.c

clean:
	rm -f *.o fetch
