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
#include "arm_exception.h"
#include "arm_constants.h"
#include "arm_core.h"
#include "util.h"

// Not supported below ARMv6, should read as 0
#define CP15_reg1_EEbit 0

#define Exception_bit_9 (CP15_reg1_EEbit << 9)
//-------------------------------------------------------

void arm_exception(arm_core p, unsigned char exception) {
    /* We only support RESET initially */
    /* Semantics of reset interrupt (ARM manual A2-18) */
	
	uint32_t cpsr_save = arm_read_cpsr(p); //une sauvegarde du cpsr actuel
	uint32_t new_cpsr;
	uint32_t pc_value;
	
    switch(exception){
		case RESET :
				new_cpsr = 0x1d3 | (cpsr_save & 0xFFFFFC00); 
				
				arm_write_cpsr(p, new_cpsr | Exception_bit_9);
				arm_write_register(p, 15, 0x00000000); //écriture sur PC
				break;
				
		case UNDEFINED_INSTRUCTION : //MODE UND
				pc_value = arm_read_register(p, 15);
				new_cpsr = 0x9b | (cpsr_save & 0xFFFFFD40);
				
				arm_write_cpsr(p, new_cpsr | Exception_bit_9);
				arm_write_register(p, 14, pc_value - 4); //R14 = Registre LR
				arm_write_spsr(p, cpsr_save); //SPSR_und
				arm_write_register(p, 15, 0x00000004); 
				break;
				
		case SOFTWARE_INTERRUPT : //MODE SVC
				pc_value = arm_read_register(p, 15);
				new_cpsr = 0x93 | (cpsr_save & 0xFFFFFD40);
				
				arm_write_cpsr(p, new_cpsr | Exception_bit_9);
				arm_write_register(p, 14, pc_value - 4); //A priori, l'instruction SWI est à l'adresse indiquée par PC. Donc toujours PC - 4 
				arm_write_spsr(p, cpsr_save); //SPSR_svc
				arm_write_register(p, 15, 0x00000008); 
				break;
				
				
		case PREFETCH_ABORT : //MODE ABT
				pc_value = arm_read_register(p, 15);
				new_cpsr = 0x197 | (cpsr_save & 0xFFFFFC40);
				
				arm_write_cpsr(p, new_cpsr | Exception_bit_9);
				arm_write_register(p, 14, pc_value + 4); 
				arm_write_spsr(p, cpsr_save); //SPSR_abt
				arm_write_register(p, 15, 0x0000000C); 
				break;
				
		case DATA_ABORT : //MODE ABT (aussi)
				pc_value = arm_read_register(p, 15);
				new_cpsr = 0x197 | (cpsr_save & 0xFFFFFC40);
				
				arm_write_cpsr(p, new_cpsr | Exception_bit_9);
				arm_write_register(p, 14, pc_value + 8); 
				arm_write_spsr(p, cpsr_save); //SPSR_abt
				arm_write_register(p, 15, 0x00000010); 
				break;
				
		case INTERRUPT : //MODE IRQ
				pc_value = arm_read_register(p, 15);
				new_cpsr = 0x1a2 | (cpsr_save & 0xFFFFFC40);
				
				arm_write_cpsr(p, new_cpsr | Exception_bit_9);
				arm_write_register(p, 14, pc_value + 4); 
				arm_write_spsr(p, cpsr_save); //SPSR_irq
				arm_write_register(p, 15, 0x00000018);
				
				//Je ne prend pas en compte le bit VE, 
				//il se trouve dans le registre CP15 du système du COprocesseur (pas traité ici)
		
		case FAST_INTERRUPT : //MODE FIQ
				pc_value = arm_read_register(p, 15);
				new_cpsr = 0x1d1 | (cpsr_save & 0xFFFFFC00);
				
				arm_write_cpsr(p, new_cpsr | Exception_bit_9);
				arm_write_register(p, 14, pc_value + 4); 
				arm_write_spsr(p, cpsr_save); //SPSR_irq
				arm_write_register(p, 15, 0x0000001C);
    }
}
