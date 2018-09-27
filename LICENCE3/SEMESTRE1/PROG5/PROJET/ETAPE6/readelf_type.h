#ifndef TYPE_ELF
#define TYPE_ELF

typedef struct elf {

	//Variables Etape 1
   Elf32_Ehdr *ehdr;

   //Variables Etape 2
		Elf32_Shdr *shdrTable;
		char *tabNames;
		Elf32_Shdr stringTab;

		//Variables Etape 3
		char* strSec;
		
		//Variables Etape 4
		Elf32_Sym *symTable;
		char *etape4Names;
		Elf32_Shdr stringTab4;
		int nbsymbole, decalsymtab ;

		//Variables Etape 5
		Elf32_Rel *rel;
		int indSecRel;
		int nbentreeSecRel;

} elf;

elf read_all(int fd, int *error);

#endif
