#ifndef ETAPE_3
#define ETAPE_3


int getIdFromName(char *tabNames, Elf32_Shdr *shdrTable, const char *name, int nbName, int *error);

char *getNameFromId(char *tabNames, Elf32_Shdr *shdrTable, const int indSect, int nbName, int *error);

int print_contenuSection(int fd, elf *donnees, int ind, char *name);

#endif
