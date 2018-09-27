#ifndef ETAPE_1
#define ETAPE_1


int read_header(int fd, Elf32_Ehdr *ehdr);

int print_header(Elf32_Ehdr *ehdr);

#endif
