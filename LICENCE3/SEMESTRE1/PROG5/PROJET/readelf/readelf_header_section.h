#ifndef ETAPE_2
#define ETAPE_2


char *getType(Elf32_Shdr shdr);

int read_sectionTable(int fd, elf *donnees);

int print_sectionTable(elf *donnees);

#endif
