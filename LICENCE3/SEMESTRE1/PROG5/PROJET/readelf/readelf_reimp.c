#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>
#include <elf.h>


#include "readelf_type.h"
#include "readelf_reimp.h"
#include "readelf_section.h"

//-------------------------------------------------------------------------------
    //PRINT_REIMP : Lecture + Affichage des tables de réimplantations
int print_reimp(int fd, elf *donnees){
    
    int i=0, nbentree=0, j=0, index;

	//LECTURE
	while(i<donnees->ehdr->e_shnum){
     //se déplacer au début de la section .rel.text
     
    if(donnees->shdrTable[i].sh_type == SHT_REL){ //NE PAS FAIRE LES RELA
        lseek(fd, donnees->shdrTable[i].sh_offset, SEEK_SET);
        nbentree = donnees->shdrTable[i].sh_size / sizeof(Elf32_Rel); //pour afficher le nombre d'entree

        donnees->rel = malloc(sizeof(Elf32_Rel) * nbentree);

        j=0;
        
        while(j<nbentree){
            read(fd, &donnees->rel[j], sizeof(donnees->rel));
            j++;
        }
				
				//AFFICHAGE
				printf("*************************************************\n");
        printf("Section de relocalisation '%s' a l'adresse de decalage 0x%x contient %d entrees: \n \n", donnees->tabNames + donnees->shdrTable[i].sh_name, donnees->shdrTable[i].sh_offset, nbentree);
        
        printf("Decalage \t");
         printf("%-8s \t","Info");
         printf("%-15s \t","Type");
         printf("%-8s \t","Val.-sym");
         printf("%-20s \n","Nom.-sym");
        
        j=0;
        while(j<nbentree){
            
            printf("%8.8x \t", donnees->rel[j].r_offset); //Decalage
            printf("%8.8x \t", donnees->rel[j].r_info); //Info
						
						//Type
            switch ((unsigned char) donnees->rel[j].r_info){ 
		          case R_ARM_NONE: printf("%-15s \t","R_ARM_NONE"); break;
		          case R_ARM_PC24: printf("%-15s \t","R_ARM_PC24"); break;
		          case R_ARM_ABS32: printf("%-15s \t","R_ARM_ABS32"); break;
		          case R_ARM_REL32: printf("%-15s \t","R_ARM_REL32"); break;
		          //case R_ARM_LDR_PC_G0: printf("%-15s \t","R_ARM_LDR_PC_G0"); break;
		          case R_ARM_ABS16: printf("%-15s \t","R_ARM_ABS16"); break;
		          case R_ARM_ABS12: printf("%-15s \t","R_ARM_ABS12"); break;
		          case R_ARM_THM_ABS5: printf("%-15s \t","R_ARM_THM_ABS5"); break;
		          case R_ARM_ABS8: printf("%-15s \t","R_ARM_ABS8"); break;
		          case R_ARM_SBREL32: printf("%-15s \t","R_ARM_SBREL32"); break;
		          //case R_ARM_THM_CALL: printf("%-15s \t","R_ARM_THM_CALL"); break;
		          case R_ARM_THM_PC8: printf("%-15s \t","R_ARM_THM_PC8");break;
		          //case R_ARM_BREL_ADJ: printf("%-15s \t","R_ARM_BREL_ADJ");break;
		          case R_ARM_TLS_DESC: printf("%-15s \t","R_ARM_TLS_DESC");break;
		          case R_ARM_THM_SWI8: printf("%-15s \t","R_ARM_THM_SWI8");break;
		          case R_ARM_XPC25: printf("%-15s \t","R_ARM_XPC25");break;
		          case R_ARM_THM_XPC22: printf("%-15s \t","R_ARM_THM_XPC22");break;
		          case R_ARM_TLS_DTPOFF32: printf("%-15s \t","R_ARM_TLS_DTPOFF32");break;
		          case R_ARM_TLS_TPOFF32: printf("%-15s \t","R_ARM_TLS_TPOFF32");break;
		          case R_ARM_COPY: printf("%-15s \t","R_ARM_COPY");break;
		          case R_ARM_GLOB_DAT: printf("%-15s \t","R_ARM_GLOB_DAT");break;
		          case R_ARM_JUMP_SLOT: printf("%-15s \t","R_ARM_JUMP_SLOT");break;
		          case R_ARM_RELATIVE: printf("%-15s \t","R_ARM_RELATIVE");break;
		          //case R_ARM_GOTOFF32: printf("%-15s \t","R_ARM_GOTOFF32");break;
		          //case R_ARM_BASE_PREL: printf("%-15s \t","R_ARM_BASE_PREL");break;
		          //case R_ARM_GOT_BREL: printf("%-15s \t","R_ARM_GOT_BREL");break;
		          case R_ARM_PLT32: printf("%-15s \t","R_ARM_PLT32");break;
		          case R_ARM_CALL: printf("%-15s \t","R_ARM_CALL");break;
		          case R_ARM_JUMP24: printf("%-15s \t","R_ARM_JUMP24");break;
		          case R_ARM_THM_JUMP24: printf("%-15s \t","R_ARM_THM_JUMP24");break;
		          case R_ARM_BASE_ABS: printf("%-15s \t","R_ARM_BASE_ABS");break;
		          case R_ARM_ALU_PCREL_7_0: printf("%-15s \t","R_ARM_ALU_PCREL_7_0");break;
		          case R_ARM_ALU_PCREL_15_8: printf("%-15s \t","R_ARM_ALU_PCREL_15_8");break;
		          case R_ARM_ALU_PCREL_23_15: printf("%-15s \t","R_ARM_ALU_PCREL_23_15");break;
		          //case R_ARM_LDR_SBREL_11_0_NC: printf("%-15s \t","R_ARM_LDR_SBREL_11_0_NC");break;
		          //case R_ARM_ALU_SBREL_19_12_NC: printf("%-15s \t","R_ARM_ALU_SBREL_19_12_NC");break;
		          //case R_ARM_ALU_SBREL_27_20_CK: printf("%-15s \t","R_ARM_ALU_SBREL_27_20_CK");break;
		          case R_ARM_TARGET1: printf("%-15s \t","R_ARM_TARGET1");break;
		          case R_ARM_SBREL31: printf("%-15s \t","R_ARM_SBREL31");break;
		          case R_ARM_V4BX: printf("%-15s \t","R_ARM_V4BX");break;
		          case R_ARM_TARGET2: printf("%-15s \t","R_ARM_TARGET2");break;
		          case R_ARM_PREL31: printf("%-15s \t","R_ARM_PREL31");break;
		          case R_ARM_MOVW_ABS_NC: printf("%-15s \t","R_ARM_MOVW_ABS_NC");break;
		          case R_ARM_MOVT_ABS: printf("%-15s \t","R_ARM_MOVT_ABS");break;
		          case R_ARM_MOVW_PREL_NC: printf("%-15s \t","R_ARM_MOVW_PREL_NC");break;
		          case R_ARM_MOVT_PREL: printf("%-15s \t","R_ARM_MOVT_PREL");break;
		          case R_ARM_THM_MOVW_ABS_NC: printf("%-15s \t","R_ARM_THM_MOVW_ABS_NC");break;
		          case R_ARM_THM_MOVT_ABS: printf("%-15s \t","R_ARM_THM_MOVT_ABS");break;
		          case R_ARM_THM_MOVW_PREL_NC: printf("%-15s \t","R_ARM_THM_MOVW_PREL_NC");break;
		          case R_ARM_THM_MOVT_PREL: printf("%-15s \t","R_ARM_THM_MOVT_PREL");break;
		          case R_ARM_THM_JUMP19: printf("%-15s \t","R_ARM_THM_JUMP19");break;
		          case R_ARM_THM_JUMP6: printf("%-15s \t","R_ARM_THM_JUMP6");break;
		          case R_ARM_THM_ALU_PREL_11_0: printf("%-15s \t","R_ARM_THM_ALU_PREL_11_0");break;
		          case R_ARM_THM_PC12: printf("%-15s \t","R_ARM_THM_PC12");break;
		          case R_ARM_ABS32_NOI: printf("%-15s \t","R_ARM_ABS32_NOI");break;
		          case R_ARM_REL32_NOI: printf("%-15s \t","R_ARM_REL32_NOI");break;
		          case R_ARM_ALU_PC_G0_NC: printf("%-15s \t","R_ARM_ALU_PC_G0_NC");break;
		          case R_ARM_ALU_PC_G0: printf("%-15s \t","R_ARM_ALU_PC_G0");break;
		          case R_ARM_ALU_PC_G1_NC: printf("%-15s \t","R_ARM_ALU_PC_G1_NC");break;
		          case R_ARM_ALU_PC_G1: printf("%-15s \t","R_ARM_ALU_PC_G1");break;
		          case R_ARM_ALU_PC_G2: printf("%-15s \t","R_ARM_ALU_PC_G2");break;
		          case R_ARM_LDR_PC_G1: printf("%-15s \t","R_ARM_LDR_PC_G1");break;
		          case R_ARM_LDR_PC_G2: printf("%-15s \t","R_ARM_LDR_PC_G2");break;
		          case R_ARM_LDRS_PC_G0: printf("%-15s \t","R_ARM_LDRS_PC_G0");break;
		          case R_ARM_LDRS_PC_G1: printf("%-15s \t","R_ARM_LDRS_PC_G1");break;
		          case R_ARM_LDRS_PC_G2: printf("%-15s \t","R_ARM_LDRS_PC_G2");break;
		          case R_ARM_LDC_PC_G0: printf("%-15s \t","R_ARM_LDC_PC_G0");break;
		          case R_ARM_LDC_PC_G1: printf("%-15s \t","R_ARM_LDC_PC_G1");break;
		          case R_ARM_LDC_PC_G2: printf("%-15s \t","R_ARM_LDC_PC_G2");break;
		          case R_ARM_ALU_SB_G0_NC: printf("%-15s \t","R_ARM_ALU_SB_G0_NC");break;
		          case R_ARM_ALU_SB_G0: printf("%-15s \t","R_ARM_ALU_SB_G0");break;
		          case R_ARM_ALU_SB_G1_NC: printf("%-15s \t","R_ARM_ALU_SB_G1_NC");break;
		          case R_ARM_ALU_SB_G1: printf("%-15s \t","R_ARM_ALU_SB_G1");break;
		          case R_ARM_ALU_SB_G2: printf("%-15s \t","R_ARM_ALU_SB_G2");break;
		          case R_ARM_LDR_SB_G0: printf("%-15s \t","R_ARM_LDR_SB_G0");break;
		          case R_ARM_LDR_SB_G1: printf("%-15s \t","R_ARM_LDR_SB_G1");break;
		          case R_ARM_LDR_SB_G2: printf("%-15s \t","R_ARM_LDR_SB_G2");break;
		          case R_ARM_LDRS_SB_G0: printf("%-15s \t","R_ARM_LDRS_SB_G0");break;
		          case R_ARM_LDRS_SB_G1: printf("%-15s \t","R_ARM_LDRS_SB_G1");break;
		          case R_ARM_LDRS_SB_G2: printf("%-15s \t","R_ARM_LDRS_SB_G2");break;
		          case R_ARM_LDC_SB_G0: printf("%-15s \t","R_ARM_LDC_SB_G0");break;
		          case R_ARM_LDC_SB_G1: printf("%-15s \t","R_ARM_LDC_SB_G1");break;
		          case R_ARM_LDC_SB_G2: printf("%-15s \t","R_ARM_LDC_SB_G2");break;
		          case R_ARM_MOVW_BREL_NC: printf("%-15s \t","R_ARM_MOVW_BREL_NC");break;
		          case R_ARM_MOVT_BREL: printf("%-15s \t","R_ARM_MOVT_BREL");break;
		          case R_ARM_MOVW_BREL: printf("%-15s \t","R_ARM_MOVW_BREL");break;
		          case R_ARM_THM_MOVW_BREL_NC: printf("%-15s \t","R_ARM_THM_MOVW_BREL_NC");break;
		          case R_ARM_THM_MOVT_BREL: printf("%-15s \t","R_ARM_THM_MOVT_BREL");break;
		          case R_ARM_THM_MOVW_BREL: printf("%-15s \t","R_ARM_THM_MOVW_BREL");break;
		          case R_ARM_TLS_GOTDESC: printf("%-15s \t","R_ARM_TLS_GOTDESC");break;
		          case R_ARM_TLS_CALL: printf("%-15s \t","R_ARM_TLS_CALL");break;
		          case R_ARM_TLS_DESCSEQ: printf("%-15s \t","R_ARM_TLS_DESCSEQ");break;
		          case R_ARM_THM_TLS_CALL: printf("%-15s \t","R_ARM_THM_TLS_CALL");break;
		          case R_ARM_PLT32_ABS: printf("%-15s \t","R_ARM_PLT32_ABS");break;
		          case R_ARM_GOT_ABS: printf("%-15s \t","R_ARM_GOT_ABS");break;
		          case R_ARM_GOT_PREL: printf("%-15s \t","R_ARM_GOT_PREL");break;
		          case R_ARM_GOT_BREL12: printf("%-15s \t","R_ARM_GOT_BREL12");break;
		          case R_ARM_GOTOFF12: printf("%-15s \t","R_ARM_GOTOFF12");break;
		          case R_ARM_GOTRELAX: printf("%-15s \t","R_ARM_GOTRELAX");break;
		          case R_ARM_GNU_VTENTRY: printf("%-15s \t","R_ARM_GNU_VTENTRY");break;
		          case R_ARM_GNU_VTINHERIT: printf("%-15s \t","R_ARM_GNU_VTINHERIT");break;
		          //case R_ARM_THM_JUMP11: printf("%-15s \t","R_ARM_THM_JUMP11");break;
		          //case R_ARM_THM_JUMP8: printf("%-15s \t","R_ARM_THM_JUMP8");break;
		          case R_ARM_TLS_GD32: printf("%-15s \t","R_ARM_TLS_GD32");break;
		          case R_ARM_TLS_LDM32: printf("%-15s \t","R_ARM_TLS_LDM32");break;
		          case R_ARM_TLS_LDO32: printf("%-15s \t","R_ARM_TLS_LDO32");break;
		          case R_ARM_TLS_IE32: printf("%-15s \t","R_ARM_TLS_IE32");break;
		          case R_ARM_TLS_LE32: printf("%-15s \t","R_ARM_TLS_LE32");break;
		          case R_ARM_TLS_LDO12: printf("%-15s \t","R_ARM_TLS_LDO12");break;
		          case R_ARM_TLS_LE12: printf("%-15s \t","R_ARM_TLS_LE12");break;
		          case R_ARM_TLS_IE12GP: printf("%-15s \t","R_ARM_TLS_IE12GP");break;
		          case R_ARM_ME_TOO: printf("%-15s \t","R_ARM_ME_TOO");break;
		          case R_ARM_THM_TLS_DESCSEQ16: printf("%-15s \t","R_ARM_THM_TLS_DESCSEQ16");break;
		          case R_ARM_THM_TLS_DESCSEQ32: printf("%-15s \t","R_ARM_THM_TLS_DESCSEQ32");break;
		          case R_ARM_THM_GOT_BREL12: printf("%-15s \t","R_ARM_THM_GOT_BREL12");break;
		          case R_ARM_IRELATIVE: printf("%-15s \t","R_ARM_IRELATIVE");break;
		          default: printf("%-15s \t","ERREUR");break;
            }


            printf("%8.8x \t", ((ELF32_R_SYM(donnees->rel[j].r_info))>>8)); //Val.-sym
            
		index = (int) (donnees->rel[j].r_info>>8);
	    if(donnees->symTable[index].st_info == STT_SECTION){
		index = donnees->symTable[index].st_shndx;
            	printf("%s", donnees->tabNames + donnees->shdrTable[index].sh_name);
	    }
	    else{
		
		printf("%s", donnees->etape4Names + donnees->symTable[index].st_name);
	    }
            printf("\n");
            j++;
        }
		
    }
    i++;
}
    return 0;
}
