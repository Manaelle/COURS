/*
Armator - simulateur de jeu d'instruction ARMv5T à but pédagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique Générale GNU publiée par la Free Software
Foundation (version 2 ou bien toute autre version ultérieure choisie par vous).

Ce programme est distribué car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but spécifique. Reportez-vous à la
Licence Publique Générale GNU pour plus de détails.

Vous devez avoir reçu une copie de la Licence Publique Générale GNU en même
temps que ce programme ; si ce n'est pas le cas, écrivez à la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
États-Unis.

Contact: Guillaume.Huard@imag.fr
	 Bâtiment IMAG
	 700 avenue centrale, domaine universitaire
	 38401 Saint Martin d'Hères
*/
#include "registers.h"
#include "arm_constants.h"
#include <stdlib.h>

//--------------------------------------------------------------
//Nos registes :

struct registers_data {
//REGISTRES NON RESERVES
uint32_t R0;
uint32_t R1;
uint32_t R2;
uint32_t R3;
uint32_t R4;
uint32_t R5;
uint32_t R6;
uint32_t R7;

//REGISTRES RESERVES
uint32_t R8_usr;
uint32_t R8_fiq;

uint32_t R9_usr;
uint32_t R9_fiq;

uint32_t R10_usr;
uint32_t R10_fiq;

uint32_t R11_usr;
uint32_t R11_fiq;

uint32_t R12_usr;
uint32_t R12_fiq;

//REGISTRE SP
uint32_t R13_usr;
uint32_t R13_svc;
uint32_t R13_abt;
uint32_t R13_und;
uint32_t R13_irq;
uint32_t R13_fiq;

//REGISTRE LR
uint32_t R14_usr;
uint32_t R14_svc;
uint32_t R14_abt;
uint32_t R14_und;
uint32_t R14_irq;
uint32_t R14_fiq;

//REGISTRE PC
uint32_t R15;

//REGISTRES D'ETAT
uint32_t CPSR;
uint32_t SPSR_svc;
uint32_t SPSR_abt;
uint32_t SPSR_und;
uint32_t SPSR_irq;
uint32_t SPSR_fiq;
};

//--------------------------------------------------------------
//crée un lot de registres ARM (dans register_data)
registers registers_create() {

    registers r = malloc(sizeof(struct registers_data));   
    return r;
}
//--------------------------------------------------------------
//détruit le lot
void registers_destroy(registers r) {
	free(r);
}
//--------------------------------------------------------------
//Le mode se trouve dans le CPSR, aux bits 0 à 4. 
uint8_t get_mode(registers r) {
     
    uint32_t MASK = 0x0000001F; //Position des bits M dans CPSR
    
    uint8_t mode = r->CPSR & MASK;
    
    return mode;
} 
//--------------------------------------------------------------
//Tous les modes, sauf USER, sont dotés d'un CPSR de sauvegarde (=SPSR). 
//Cette fonction envoie 1 si le mode actuel du processeur a un spsr-> 0 sinon.

int current_mode_has_spsr(registers r) {

    uint32_t MASK = 0x0000001F; 
    uint8_t mode = r->CPSR & MASK;
    
    if(mode == USR || mode == SYS) {
    	return 0;
    }
    else {
    	return 1;
    }
    
}
//--------------------------------------------------------------
//USER est un mode classique, les autres modes (Supervisor, Abort etc...) sont les modes privilégiés
int in_a_privileged_mode(registers r) {

    uint32_t MASK = 0x0000001F; 
    uint8_t mode = r->CPSR & MASK;
    
    if(mode == USR) {
    	return 0;
    }
    else {
    	return 1;
    }
}
//--------------------------------------------------------------
//On a 37 registres en tout. La fonction prend un numéro de registre reg et renvoie son contenu.
uint32_t read_register(registers r, uint8_t reg) {
    
    switch(reg){
    
					case 0x00: 
									return r->R0;
									break;
									
					case 0x01: 
									return r->R1;
									break;
									
					case 0x02: 
									return r->R2;
									break;
    		
    				case 0x03: 
									return r->R3;
									break;
									
					case 0x04: 
									return r->R4;
									break;
									
					case 0x05: 
									return r->R5;
									break;
									
					case 0x06: 
									return r->R6;
									break;
									
					case 0x07: 
									return r->R7;
									break;
					
					case 0x08: 
									//R8_usr est le R8 classique. Si seul le mode FIQ a son propre registre R8
									if (get_mode(r) == FIQ) {return r->R8_fiq;}
									else {return r->R8_usr;}
									break;
									
					case 0x09: 
									if (get_mode(r) == FIQ) {return r->R9_fiq;}
									else {return r->R9_usr;}
									break;
									
					case 0x0A: 
									if (get_mode(r) == FIQ) {return r->R10_fiq;}
									else {return r->R10_usr;}
									break;
					
					case 0x0B: 
									if (get_mode(r) == FIQ) {return r->R11_fiq;}
									else {return r->R11_usr;}
									break;
									
					case 0x0C: 
									if (get_mode(r) == FIQ) {return r->R12_fiq;}
									else {return r->R12_usr;}
									break;
									
					case 0x0D: 
									//R13 et R14 dépendent du mode
									switch(get_mode(r)){
    
													case USR: 									
																	return r->R13_usr; //USER est le seul mode dépourvu de sauvegarde du CPSR
																	break;
									
													case FIQ: 
																	return r->R13_fiq;
																	break;
									
													case IRQ: 
																	return r->R13_irq;
																	break;
						
													case SVC: 
																	return r->R13_svc;
																	break;
									
													case ABT: 
																	return r->R13_abt;
																	break;
									
													case UND: 
																	return r->R13_und;
																	break;
									
													case SYS: 
																	return r->R13_usr;
																	break;
									}
									break;
									
					case 0x0E: 
									switch(get_mode(r)){
    
													case USR: 									
																	return r->R14_usr; //USER est le seul mode dépourvu de sauvegarde du CPSR
																	break;
									
													case FIQ: 
																	return r->R14_fiq;
																	break;
									
													case IRQ: 
																	return r->R14_irq;
																	break;
						
													case SVC: 
																	return r->R14_svc;
																	break;
									
													case ABT: 
																	return r->R14_abt;
																	break;
									
													case UND: 
																	return r->R14_und;
																	break;
									
													case SYS: 
																	return r->R14_usr;
																	break;
									}
									break;
					
					case 0x0F: 
									//registre PC
									return r->R15;
									break;
									
					
					default: 
									return 0;
									break;
    
    }
    
    return 0;
}
//--------------------------------------------------------------
//Permet de lire uniquement des registres accessibles en mode USER, soit 15 registres en tout + CPSR
uint32_t read_usr_register(registers r, uint8_t reg) {

    switch(reg){
    
					case 0x00: 
									return r->R0;
									break;
									
					case 0x01: 
									return r->R1;
									break;
									
					case 0x02: 
									return r->R2;
									break;
    		
    			case 0x03: 
									return r->R3;
									break;
									
					case 0x04: 
									return r->R4;
									break;
									
					case 0x05: 
									return r->R5;
									break;
									
					case 0x06: 
									return r->R6;
									break;
									
					case 0x07: 
									return r->R7;
									break;
					
					case 0x08: 
									return r->R8_usr;
									break;
									
					case 0x09: 
									return r->R9_usr;
									break;
									
					case 0x0A: 
									return r->R10_usr;
									break;
									
					case 0x0B: 
									return r->R11_usr;
									break;
									
					case 0x0C: 
									return r->R12_usr;
									break;
									
					case 0x0D: 
									return r->R13_usr;
									break;
					
					case 0x0E: 
									return r->R14_usr;
									break;
									
					case 0x0F: 
									return r->R15;
									break;

					default: 
									return 0;
									break;
    
    }
   
}
//--------------------------------------------------------------
//permet de lire le registre CPSR
uint32_t read_cpsr(registers r) {

    return r->CPSR;
}
//--------------------------------------------------------------
//Permet de lire le SPSr-> Il faut consulter le mode avant. 
uint32_t read_spsr(registers r) {
    
    switch(get_mode(r)){
    
					case USR: 									
									return r->CPSR; //USER est le seul mode, avec SYS, à être dépourvu de sauvegarde du CPSR
									break;
									
					case FIQ: 
									return r->SPSR_fiq;
									break;
									
					case IRQ: 
									return r->SPSR_irq;
									break;
    		
    			case SVC: 
									return r->SPSR_svc;
									break;
									
					case ABT: 
									return r->SPSR_abt;
									break;
									
					case UND: 
									return r->SPSR_und;
									break;
									
					case SYS: 
									return r->CPSR; //SYS est comme un mode USR, mais sans contraintes
									break;
									
					default: 
									return 0;
									break;
    
    }
    
}
//--------------------------------------------------------------
//écrit VALUE dans le registre n°REG (REG allant de 0 à 37)
void write_register(registers r, uint8_t reg, uint32_t value) {
    
    switch(reg){
    
					case 0x00: 
									r->R0 = value;
									break;
									
					case 0x01: 
									r->R1 = value;
									break;
									
					case 0x02: 
									r->R2 = value;
									break;
    		
    				case 0x03: 
									r->R3 = value;
									break;
									
					case 0x04: 
									r->R4 = value;
									break;
									
					case 0x05: 
									r->R5 = value;
									break;
									
					case 0x06: 
									r->R6 = value;
									break;
									
					case 0x07: 
									r->R7 = value;
									break;
					
					//R8_usr est le R8 classique. Si seul le mode FIQ a son propre registre R8
									if (get_mode(r) == FIQ) {r->R8_fiq = value;}
									else {r->R8_usr = value;}
									break;
									
					case 0x09: 
									if (get_mode(r) == FIQ) {r->R9_fiq = value;}
									else {r->R9_usr = value;}
									break;
									
					case 0x0A: 
									if (get_mode(r) == FIQ) {r->R10_fiq = value;}
									else {r->R10_usr = value;}
									break;
					
					case 0x0B: 
									if (get_mode(r) == FIQ) {r->R11_fiq = value;}
									else {r->R11_usr= value;}
									break;
									
					case 0x0C: 
									if (get_mode(r) == FIQ) {r->R12_fiq= value;}
									else {r->R12_usr = value;}
									break;
									
					case 0x0D: 
									//R13 et R14 dépendent du mode
									switch(get_mode(r)){
    
													case USR: 									
																	r->R13_usr = value; //USER est le seul mode dépourvu de sauvegarde du CPSR
																	break;
									
													case FIQ: 
																	r->R13_fiq = value;
																	break;
									
													case IRQ: 
																	r->R13_irq = value;
																	break;
						
													case SVC: 
																	r->R13_svc = value;
																	break;
									
													case ABT: 
																	r->R13_abt = value;
																	break;
									
													case UND: 
																	r->R13_und = value;
																	break;
									
													case SYS: 
																	r->R13_usr = value;
																	break;
									}
									break;
									
					case 0x0E: 
									switch(get_mode(r)){
    
													case USR: 									
																	r->R14_usr = value; //USER est le seul mode dépourvu de sauvegarde du CPSR
																	break;
									
													case FIQ: 
																	r->R14_fiq = value;
																	break;
									
													case IRQ: 
																	r->R14_irq = value;
																	break;
						
													case SVC: 
																	r->R14_svc = value;
																	break;
									
													case ABT: 
																	r->R14_abt = value;
																	break;
									
													case UND: 
																	r->R14_und = value;
																	break;
									
													case SYS: 
																	r->R14_usr = value;
																	break;
									}
									break;
					
					case 0x0F: 
									//registre PC
									r->R15  = value;
									break;
    
    }

}

//--------------------------------------------------------------
//permet d'écrire dans un registre, en mode USER
//Attention, certaines zones du CPSR sont inaccessibles en mode USER
void write_usr_register(registers r, uint8_t reg, uint32_t value) {
	switch(reg){
					case 0x00: 
									r->R0  = value;
									break;
									
					case 0x01: 
									r->R1 = value;
									break;
									
					case 0x02: 
									r->R2  = value;
									break;
    		
    				case 0x03: 
									r->R3  = value;
									break;
									
					case 0x04: 
									r->R4  = value;
									break;
									
					case 0x05: 
									r->R5  = value;
									break;
									
					case 0x06: 
									r->R6  = value;
									break;
									
					case 0x07: 
									r->R7  = value;
									break;
					
					case 0x08: 
									r->R8_usr = value;
									break;
									
					case 0x09: 
									r->R9_usr = value;
									break;
									
					case 0x0A: 
									r->R10_usr = value;
									break;
									
					case 0x0B: 
									r->R11_usr = value;
									break;
									
					case 0x0C: 
									r->R12_usr = value;
									break;
									
					case 0x0D: 
									r->R13_usr = value;
									break;
					
					case 0x0E: 
									r->R14_usr = value;
									break;
									
					case 0x0F: 
									r->R15 = value;
									break;
    
    }
}

//--------------------------------------------------------------
//écrire une valeur dans CPSR
void write_cpsr(registers r, uint32_t value) {
	
	uint32_t MASK_USER = 0xF80F0200; //position des bits modifiables par USER
	uint32_t MASK_PRIVILEGE = 0xF90F03FF; //position des bits modifiables par les modes privilégiés (les autres)
	
	switch(get_mode(r)){
									
					case USR: 
									r->CPSR = value & MASK_USER;//Si VALUE tente d'écrire sur des zones privilégiés, ces bits sont ignorés.							
									break;
									
									
					default: 
									r->CPSR = value & MASK_PRIVILEGE;
									break;
    
    }
}

//--------------------------------------------------------------
//écrit VALUE dans le SPSR du mode correspondant. Si le mode est USER, il faut utiliser write_CPSR()
void write_spsr(registers r, uint32_t value) {
	
	switch(get_mode(r)){
									
					case FIQ: 
									r->SPSR_fiq = value;
									break;
									
					case IRQ: 
									r->SPSR_irq = value;
									break;
    		
    			case SVC: 
									r->SPSR_svc = value;
									break;
									
					case ABT: 
									r->SPSR_abt = value;
									break;
									
					case UND: 
									r->SPSR_und = value;
									break;
																
    
    }

}
//--------------------------------------------------------------
