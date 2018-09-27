/*
Armator - simulateur de jeu d'instruction ARMv5T � but p�dagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique G�n�rale GNU publi�e par la Free Software
Foundation (version 2 ou bien toute autre version ult�rieure choisie par vous).

Ce programme est distribu� car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but sp�cifique. Reportez-vous � la
Licence Publique G�n�rale GNU pour plus de d�tails.

Vous devez avoir re�u une copie de la Licence Publique G�n�rale GNU en m�me
temps que ce programme ; si ce n'est pas le cas, �crivez � la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
�tats-Unis.

Contact: Guillaume.Huard@imag.fr
	 B�timent IMAG
	 700 avenue centrale, domaine universitaire
	 38401 Saint Martin d'H�res
*/
#include "arm_instruction.h"
#include "arm_exception.h"
#include "arm_data_processing.h"
#include "arm_load_store.h"
#include "arm_branch_other.h"
#include "arm_constants.h"
#include "util.h"


//---------------------------------------------------
//Regarde le ZNCV : envoie 1 si le ZNCV respecte les conditions, 0 sinon
int conditionsValides(arm_core p, uint32_t instruction) {
	
	uint32_t cpsr = arm_read_cpsr(p);
	
	switch( get_bits(instruction,31,28) ){
	
		case 0x0 : if (!get_bit(cpsr,30)) {return 0;}
								else {return 1;}
										   break;
		case 0x1 : if (get_bit(cpsr,30)) {return 0;}
								else {return 1;}
										   break;
		case 0x2 : if (!get_bit(cpsr,29)) {return 0;}
								else {return 1;}
										   break;
		case 0x3 : if (get_bit(cpsr,29)) {return 0;}
								else {return 1;}
										   break;
		case 0x4 : if (!get_bit(cpsr,31)) {return 0;}
								else {return 1;}
										   break;
		case 0x5 : if (get_bit(cpsr,31)) {return 0;}
								else {return 1;}
										   break;
		case 0x6 : if (!get_bit(cpsr,28)) {return 0;}
								else {return 1;}		  
		   									break;
		case 0x7 : if (get_bit(cpsr,28)) {return 0;}
								else {return 1;}
										   break;
		case 0x8 : if (!get_bit(cpsr,C) || get_bit(cpsr,Z)) {return 0;}
								else {return 1;}
										   break;
		case 0x9 : if (get_bit(cpsr,C) || !get_bit(cpsr,Z)) {return 0;}
								else {return 1;}
										   break;
		case 0xA : if ( get_bit(cpsr,N) != get_bit(cpsr,V)) {return 0;}
								else {return 1;}
										   break;
		case 0xB : if (get_bit(cpsr,N) == get_bit(cpsr,Z)) {return 0;}
								else {return 1;}
										   break;
		case 0xC : if ((get_bit(cpsr,Z) != 0) || (get_bit(cpsr,N) != get_bit(cpsr,V))) {return 0;}
								else {return 1;}
										   break;
		case 0xD : if ((get_bit(cpsr,Z) == 0) || (get_bit(cpsr,N) == get_bit(cpsr,V))) {return 0;}
								else {return 1;}
										   break;
	}
	
	 return 1;
}

//---------------------------------------------------
//prend en param�tre une instruction
// et proc�de au d�codage (cf le forum pour + d'explications) et retourne 1 si �a s'est bien pass�
int decode(arm_core p, uint32_t instruction){
	
	int exception;//il est l� juste pour r�cup les exceptions
	
	if ((get_bits(instruction,31,28) != 0xF) && (conditionsValides(p, instruction))) { //le cas o� cond = 1111 sera g�r� ult�rieurement
			switch (get_bits(instruction,27,25)){
				case 0x0 : exception = arm_data_processing_shift(p, instruction);
						   break;
				case 0x1 : exception = arm_data_processing_shift(p,instruction);
						   break;
				case 0x3 : exception = arm_coprocessor_load_store(p,instruction);
						   break;
				case 0x2 : exception = arm_load_store(p,instruction);
						   break;
				case 0x4 : exception = arm_load_store_multiple(p,instruction);
						   break;
				case 0x5 : exception = arm_branch(p,instruction);
						   break;
				case 0x6 : exception = arm_coprocessor_others_swi(p,instruction);
						   break;
				case 0x7 : exception = arm_miscellaneous(p,instruction);
						   break;
			}
	}
	return exception;
}

//---------------------------------------------------
static int arm_execute_instruction(arm_core p) {
		
		uint32_t instruction;
		int exception;
		
		//FETCH
		if (arm_fetch(p, &instruction) == -1){

			return 1;
		} 	
		//DECODE + EXECUTE
		exception = decode(p, instruction);

return exception;
}
//---------------------------------------------------
int arm_step(arm_core p) {
    int result;

    result = arm_execute_instruction(p);
    if (result)
        arm_exception(p, result);
    return result;
}
