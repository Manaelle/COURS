#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>

#include "readelf_header.h"

int main(int argc, char *argv[])
{
	int fd;
	Elf32_Ehdr *ehdr;
	
	fd = open(argv[1], O_RDONLY); // Le dernier argument est le nom du fichier
	if(fd < 0)
	{
		fprintf(stderr, "Impossible d'ouvrir le fichier %s.\n", argv[2]);
		return 2;
	}

	ehdr = malloc(sizeof(Elf32_Ehdr));
	read_header(fd, ehdr);
	print_header(ehdr);

	close(fd);

	free(ehdr);

	return 0;
}