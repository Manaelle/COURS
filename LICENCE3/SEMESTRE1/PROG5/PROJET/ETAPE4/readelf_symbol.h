#ifndef ETAPE_4
#define ETAPE_4

int read_symbole(int fd, Elf32_Sym *symTable, int nbsymbole, int decalsymtab);

int print_symbole(int fd, char *etape4Names, Elf32_Sym *symTable, int nbsymbole);

#endif
