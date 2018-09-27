/*
Armator - simulateur de jeu d'instruction ARMv5T ? but p?dagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique G?n?rale GNU publi?e par la Free Software
Foundation (version 2 ou bien toute autre version ult?rieure choisie par vous).
Ce programme est distribu? car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but sp?cifique. Reportez-vous ? la
Licence Publique G?n?rale GNU pour plus de d?tails.
Vous devez avoir re?u une copie de la Licence Publique G?n?rale GNU en m?me
temps que ce programme ; si ce n'est pas le cas, ?crivez ? la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
?tats-Unis.
Contact: Guillaume.Huard@imag.fr
	 B?timent IMAG
	 700 avenue centrale, domaine universitaire
	 38401 Saint Martin d'H?res
*/
#include "arm_load_store.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "util.h"
#include "debug.h"
#include <stdlib.h>
#include "arm_instruction.h"

int arm_load_store(arm_core p, uint32_t ins) {
	uint32_t index;
	uint32_t Rn;
	uint32_t Rm;
	uint32_t Rd;
	uint32_t *value32=malloc(sizeof(uint32_t));
	uint8_t *value8=malloc(sizeof(uint8_t));
	uint32_t offset;
	uint8_t flag_register;
	uint32_t adress;
	Rn=arm_read_register(p,get_bits(ins,19,16));
	Rm=arm_read_register(p,get_bits(ins,3,0));
	Rd=arm_read_register(p,get_bits(ins,15,12));
	flag_register=get_bits(ins,11,4);
	offset=get_bits(ins,11,0);
	switch(get_bit(ins,25)){//register offset or not
		case 0://immediate
      switch (get_bit(ins,24)) {//post-indexed or not
        case 0://post-indexd
					switch (get_bit(ins,21)) {
						case 0://LDR STR LDRB STRB
							adress=Rn;
							if (conditionsValides(p,ins)) {
								switch (get_bit(ins,23)) {//add or subtract
									case 0://subtract
										Rn=Rn-offset;
										break;
									case 1://add
										Rn=Rn+offset;
										break;
								}
								arm_write_register(p,get_bits(ins,19,16),Rn);
							}
							break;
						case 1://LDRBT LDRT STRBT STRT
							break;//we ignore this stuation firstly
					}
					break;
        case 1://offset or pre-indexed
          switch (get_bit(ins,21)) {
          	case 0://offset
							switch (get_bit(ins,23)) {//add or subtract
								case 0://subtract
									Rn=Rn-offset;
									break;
								case 1://add
									Rn=Rn+offset;
									break;
							}
							adress=Rn;
							break;
						case 1://pre-indexed
							break;//we ignore this stuation firstly
          }
					break;
				}
				break;
    case 1://register or scaled
      switch (flag_register) {
        case 0://register
					switch (get_bit(ins,24)) {//post-indexed or not
						case 0://post-indexed
							switch (get_bit(ins,21)) {
								case 0://LDR STR LDRB STRB registre
									adress=Rn;
									if (conditionsValides(p,ins)) {
										switch (get_bit(ins,23)) {//add or subtract
											case 0://subtract
												Rn=Rn-Rm;
												break;
											case 1://add
												Rn=Rn+Rm;
												break;
										}
										arm_write_register(p,get_bits(ins,19,16),Rn);
									}
									break;
								case 1:
									break;//we ignore this stuation firstly
							}
							break;
						case 1://offset or pre-indexed
							switch (get_bit(ins,21)) {
								case 0://offset
									switch (get_bit(ins,23)) {//add or subtract
										case 0://subtract
											Rn=Rn-Rm;
											break;
										case 1://add
											Rn=Rn+Rm;
											break;
									}
									adress=Rn;
									break;
								case 1://pre-indexed
									break;//we ignore this stuation firstly
							}
							break;
					}
        default://scaled register
					switch (get_bits(ins,6,5)) {
						case 0:
							index=(Rm<<get_bits(ins,11,7));
							break;
					}
					switch (get_bit(ins,24)) {//post-indexed or not
						case 0://post-indexed
							switch (get_bit(ins,21)) {
								case 0://LDR STR LDRB STRB registre
									if (conditionsValides(p,ins)) {
										switch (get_bit(ins,23)) {//add or subtract
											case 0://subtract
												Rn=Rn-index;
												break;
											case 1://add
												Rn=Rn+index;
												break;
										}
										adress=Rn;
										arm_write_register(p,get_bits(ins,19,16),Rn);
									}
									break;
								case 1:
									break;//we ignore this stuation firstly
							}
							break;
						case 1://offset or pre-indexed
							switch (get_bit(ins,21)) {
								case 0://offset
									switch (get_bit(ins,23)) {//add or subtract
										case 0://subtract
											Rn=Rn-index;
											break;
										case 1://add
											Rn=Rn+index;
											break;
									}
									adress=Rn;
									break;
								case 1://pre-indexed
									break;//we ignore this stuation firstly
							}
							break;
					}
					break;
      }
			break;
 }
	switch (get_bit(ins,22)) {//byte or not
		case 0://word
			switch (get_bit(ins,20)) {//load or store
				case 0://store
					if(1){//success in taking the value
						*value32=Rd;
						arm_write_word(p,adress,*value32);
						return 0;
					}
					else return -1;
					break;
				case 1://load
					if(arm_read_word(p,adress,value32)==0){//success in taking the value
						arm_write_register(p,get_bits(ins,15,12),*value32);
						return 0;
					}
					else return -1;
					break;
			}
			break;
		case 1://byte
			switch (get_bit(ins,20)) {//load or store
				case 0://store
					if(1){//success in taking the value
						*value8=Rd;
						arm_write_byte(p,adress,*value8);
						return 0;
					}
					else return -1;
					break;
				case 1://load
					if(arm_read_byte(p,adress+3,value8)==0){//success in taking the value
						arm_write_register(p,get_bits(ins,15,12),*value8);
						return 0;
					}
					else return -1;
					break;
			}
			break;
	}
 return 0;
}

int arm_load_store_multiple(arm_core p, uint32_t ins) {//note: we just form a ranger of addresses here,no actions for store or load 
    uint32_t *value32=malloc(sizeof(uint32_t));
	uint32_t Rn;
	uint32_t start_address;
	//uint32_t end_address;
	Rn=arm_read_register(p,get_bits(ins,19,16));
	int i=0,j=0;
	int num_of_set_bits=0;
	for(i=0;i<=15;i++){
		if(get_bit(ins,i)) num_of_set_bits++;
	}
	switch(get_bit(ins,24)){//after or before
		case 0://after
			switch(get_bit(ins,23)){//increment or decrement
				case 0://decrement
					start_address=Rn-num_of_set_bits*4+4;
					//end_address=Rn; 
					break;
				case 1://increment
					start_address=Rn;
					//end_address=Rn+num_of_set_bits*4-4;
					break;
			}
			break;
		case 1://before
			switch(get_bit(ins,23)){//increment or decrement
				case 0://decrement
					start_address=Rn-num_of_set_bits*4;
					//end_address=Rn-4;
					break;
				case 1://increment
					start_address=Rn+4;
					//end_address=Rn+num_of_set_bits*4;
					break;
			}
    		break;
    }
    switch(get_bit(ins,23)){//we will juge the cond here
    	case 0://decrement
    		if(conditionsValides(p,ins) && get_bit(ins,21)) 
    			Rn-=(num_of_set_bits*4);
    		break;
    	case 1://increment
    		if(conditionsValides(p,ins) && get_bit(ins,21)) 
    			Rn+=(num_of_set_bits*4);
    		break;
	}
	arm_write_register(p,get_bits(ins,19,16),Rn);//change the Rn
	switch(get_bit(ins,23)){//we do the loads and stores now
		case 0://decrement
			switch(get_bit(ins,20)){
				case 1://load
					for(i=0;i<=15;i++){
						if(get_bit(ins,i)){
							if(arm_read_word(p,start_address-j*4,value32)==0){//success in taking the value
								arm_write_register(p,i,*value32);
								j++;
							}
						}
						
					}
					break;
				case 0://store
					for(i=0;i<=15;i++){
						if(get_bit(ins,i)){
							*value32=arm_read_register(p,i);
							arm_write_word(p,start_address-4*j,*value32);
							j++;
						}
					}
					break;		
			}
			break;
		case 1://increment
			switch(get_bit(ins,20)){
				case 1://load
					for(i=0;i<=15;i++){
						if(get_bit(ins,i)){
							if(arm_read_word(p,start_address+j*4,value32)==0){//success in taking the value
								arm_write_register(p,i,*value32);
								j++;
							}
						}
						
					}
					break;
				case 0://store
					for(i=0;i<=15;i++){
						if(get_bit(ins,i)){
							*value32=arm_read_register(p,i);
							arm_write_word(p,start_address+4*j,*value32);
							j++;
						}
					}
					break;		
			}
			break;
	}
	return 0;
}

int arm_coprocessor_load_store(arm_core p, uint32_t ins) {
    /* Not implemented */
    return UNDEFINED_INSTRUCTION;
}
