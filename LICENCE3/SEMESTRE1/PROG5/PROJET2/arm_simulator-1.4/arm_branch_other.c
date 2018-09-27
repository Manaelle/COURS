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
#include "arm_branch_other.h"
#include "arm_constants.h"
#include "util.h"
#include <debug.h>
#include <stdlib.h>

//---------------------------------------------------------------
int arm_branch(arm_core p, uint32_t ins) {
    uint8_t L = get_bit(ins,24);
    uint32_t res = get_bits(ins,23,0);
    uint32_t resultat;
    if(L == 0x1){
        arm_write_register(p, 14, arm_read_register(p, 15));
        //printf("debug %d et %d",arm_read_register(p, 14), arm_read_register(p, 15));
    }
    if(get_bit(ins,23) == 1)//On test le 23eme bit (big endian)
        resultat = (res | 0xFF000000); //Nombre negatif : on ajoute des 1 jusqu'au 31eme bit
    else
    	resultat = res ;
    arm_write_register(p, 15, (arm_read_register(p, 15) + (resultat << 2))); // PC = PC + resultat << 2 (voir page 160 de la doc)
    return 1;
}

//---------------------------------------------------------------
int arm_coprocessor_others_swi(arm_core p, uint32_t ins) {
    if (get_bit(ins, 24)) {
        /* Here we implement the end of the simulation as swi 0x123456 */
        if ((ins & 0xFFFFFF) == 0x123456)
            exit(0);
        return SOFTWARE_INTERRUPT;
    } 
    return UNDEFINED_INSTRUCTION;
}
//---------------------------------------------------------------
int arm_miscellaneous(arm_core p, uint32_t ins) {
		
		uint8_t R = get_bit(ins,22);
		uint8_t Rd = get_bits(ins,15,12);
		
		if (R == 1){
		
			uint32_t SPSR_value = arm_read_spsr(p);
			
			arm_write_register(p, Rd, SPSR_value);
			
		}
		else {
		
		uint32_t CPSR_value = arm_read_cpsr(p);
			
		arm_write_register(p, Rd, CPSR_value);
		}

    return 1;
}
