#ifndef ETAPE_2
#define ETAPE_2


char *getType(Elf32_Shdr shdr);

int read_sectionTable(int fd, Elf32_Ehdr *ehdr, Elf32_Shdr *shdrTable);

int print_sectionTable(Elf32_Ehdr *ehdr, Elf32_Shdr *shdrTable, char *tabNames);

#endif
