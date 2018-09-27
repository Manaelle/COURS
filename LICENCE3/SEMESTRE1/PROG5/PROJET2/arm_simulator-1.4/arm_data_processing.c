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
#include "arm_data_processing.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "arm_branch_other.h"
#include "arm_load_store.h"
#include "util.h"
#include "debug.h"

#define AND 0x00
#define EOR 0x01
#define SUB 0x02
#define RSB 0x03
#define ADD 0x04
#define ADC 0x05
#define SBC 0x06
#define RSC 0x07
#define TST 0x08
#define TEQ 0x09
#define CMP 0x0A
#define CMN 0x0B
#define ORR 0x0C
#define MOV 0x0D
#define BIC 0x0E
#define MVN 0x0F

//Constante du shift dans shifter_operand
#define LSL 0
#define LSR 1
#define ASR 2
#define ROR 3

//---------------------------------------------------
//affecte de nouvelles valeurs du ZNCV au CPSR. 
//Si Les valeurs Z, N, C ou V sont différents de 0 
//ou 1, le flag n'est pas modifié
void set_ZNCV_CPSR(arm_core p, uint32_t Zbit, uint32_t Nbit, uint32_t Cbit, uint32_t Vbit){
	
	uint32_t value = arm_read_cpsr(p);
	
	
	if ( (Zbit == 0) || (Zbit == 1) ){
	
			value = (value & ~(1<<30)) | (Zbit << 30) ;
			
	}
	
	if ( (Nbit == 0) || (Nbit == 1) ){
	
			value = (value & ~(1<<31)) | (Nbit << 31) ;
			
	}
	
	if ( (Cbit == 0) || (Cbit == 1) ){
	
			value = (value & ~(1<<29)) | (Cbit << 29) ;
			
	}
	
	if ( (Vbit == 0) || (Vbit == 1) ){
	
			value = (value & ~(1<<28)) | (Vbit << 28) ;
			
	}
	
	arm_write_cpsr(p, value);	
}
//---------------------------------------------------
//retourne la valeur de la retenue sortante, lors d'une opération
int carry_from(uint32_t op1, uint32_t op2){ 

    uint64_t resultat;
	
    resultat = op1 + op2;
	
    return get_bit(resultat, 32);  
}
//---------------------------------------------------
int borrow_from(uint32_t op1, uint32_t op2){
	   
    if(op1-op2 < 0){return 1;}
    else {return 0;}
	
}
//---------------------------------------------------
/*GESTION DE OVERFLOW, operation = 1 si c'est une addition, 0 si c'est une soustraction
cas d'une addition : overflow si les 2 opérandes sont de même signe, 
mais le resultat de signe différent

cas d'un soustraction : overflow si les 2 opérandes sont de signes 
différents et le résultat de signe différent de l'opérande de gauche */
int overflow_from(uint32_t op1, uint32_t op2, uint8_t operation){
	
    uint32_t resultat;
		
	//CAS ADDITION
    if (operation == 1){
		  
		resultat = op1 + op2;
		 
        if ((get_bit(op1,31) == get_bit(op2,31))&&(get_bit(op1,31) != get_bit(resultat,31))){
			
            return 1;
        }
        else {
			return 0;
        }
    }
	//CAS SOUSTRACTION 
    else {
	
        resultat = op1 - op2;
        
		if ((get_bit(op1,31) != get_bit(op2,31))&&(get_bit(op1,31) != get_bit(resultat,31))){
			
             return 1;
        }
        else {
            return 0;
        }    
      
   }
}
//---------------------------------------------------
// LSL : retourne la valeur du shifter_operand + décalage à gauche
uint32_t lsl_shift(arm_core p, uint32_t ins, int *shifter_carry_out){

	 //Si I_shift = 0, le shift est une valeur immédiate (shift_imm), sinon c'est un registre (Rs)
	uint8_t I_shift = get_bit(ins,4);
	uint32_t value; //valeur retournée
	
	//RECUPERATION DE LA VALEUR DE RM
	uint8_t Rm = get_bits(ins,3,0);
	uint32_t Rm_val = arm_read_register(p,Rm);

	//CAS IMMEDIAT
	if(I_shift == 0){
	
		uint8_t shift_imm = get_bits(ins,11,7); 
		
		value = Rm_val << shift_imm;
		*shifter_carry_out = get_bit(Rm_val,(32 - shift_imm));
		
	}
	
	//CAS REGISTRE
	else {
		
				uint8_t Rs = get_bits(ins,11,8);
				uint32_t Rs_val = arm_read_register(p,Rs);
				Rs_val = get_bits(Rs_val, 7, 0);
				
				uint32_t CPSRflag = arm_read_cpsr(p); // On récupère CPSR
	  		uint8_t Cflag = get_bit(CPSRflag,C); // On récupère le flag C de CPSR
				
				if (Rs_val == 0){
						value = Rm_val;
						*shifter_carry_out = Cflag;
				}
				else if (Rs_val < 32){
							 value = Rm_val << Rs_val;
							 *shifter_carry_out = get_bit(Rm_val,(32 - Rs_val));
				}
				else if (Rs_val == 32){
								value = 0;
								*shifter_carry_out = get_bit(Rm_val,0);
							}
				else if (Rs_val > 32){
								value = 0;
								*shifter_carry_out = 0;
				}
	}
	
	return value;

}
//---------------------------------------------------
// LSR : retourne la valeur du shifter_operand + décalage à droite
uint32_t lsr_shift(arm_core p, uint32_t ins, int *shifter_carry_out){

	 //Si I_shift = 0, le shift est une valeur immédiate (shift_imm), sinon c'est un registre (Rs)
	uint8_t I_shift = get_bit(ins,4);
	uint32_t value; //valeur retournée
	
	//RECUPERATION DE LA VALEUR DE RM
	uint8_t Rm = get_bits(ins,3,0);
	uint32_t Rm_val = arm_read_register(p,Rm);

	//CAS IMMEDIAT
	if(I_shift == 0){
	
		uint8_t shift_imm = get_bits(ins,11,7); 
		
		if (shift_imm == 0){
					   value = 0;
					   *shifter_carry_out = get_bit(Rm_val,31);
		}
		else {
						value = Rm_val >> shift_imm;
						*shifter_carry_out = get_bit(Rm_val,(shift_imm - 1));
		}
		
	}
	
	//CAS REGISTRE
	else {
		
				uint8_t Rs = get_bits(ins,11,8);
				uint32_t Rs_val = arm_read_register(p,Rs);
				Rs_val = get_bits(Rs_val, 7, 0);
				
				uint32_t CPSRflag = arm_read_cpsr(p); // On récupère CPSR
	  		uint8_t Cflag = get_bit(CPSRflag,C); // On récupère le flag C de CPSR
				
				if (Rs_val == 0){
					   value = Rm_val;
					   *shifter_carry_out = Cflag;
				}
				else if (Rs_val < 32){
					   value = Rm_val >> Rs_val;
					   *shifter_carry_out = get_bit(Rm_val,(Rs_val - 1));
					}
					else if (Rs_val == 32){
					   value = 0;
					   *shifter_carry_out = get_bit(Rm_val,31);
					}
					else if (Rs_val > 32){
					   value = 0;
					   *shifter_carry_out = 0;
					}
	}
	
	return value;

}
//---------------------------------------------------
// ASR : retourne la valeur du shifter_operand + décalage à droite
uint32_t asr_shift(arm_core p, uint32_t ins, int *shifter_carry_out){

	 //Si I_shift = 0, le shift est une valeur immédiate (shift_imm), sinon c'est un registre (Rs)
	uint8_t I_shift = get_bit(ins,4);
	uint32_t value; //valeur retournée
	
	//RECUPERATION DE LA VALEUR DE RM
	uint8_t Rm = get_bits(ins,3,0);
	uint32_t Rm_val = arm_read_register(p,Rm);

	//CAS IMMEDIAT
	if(I_shift == 0){
	
		uint8_t shift_imm = get_bits(ins,11,7); 
		
		if (shift_imm == 0){
					   if (get_bit(Rm_val,31) == 0){
								value = 0;
								*shifter_carry_out = get_bit(Rm_val,31);
					   }
					   else if (get_bit(Rm_val,31) == 1){
						  value = 0xFFFFFFFF;
						  *shifter_carry_out = get_bit(Rm_val,31);
					   }
		}
		else if (shift_imm > 0){
					   value = asr(Rm_val, shift_imm);
					   *shifter_carry_out = get_bit(Rm_val,(shift_imm - 1));
		}
		
	}
	
	//CAS REGISTRE
	else {
		
				uint8_t Rs = get_bits(ins,11,8);
				uint32_t Rs_val = arm_read_register(p,Rs);
				Rs_val = get_bits(Rs_val, 7, 0);
				
				uint32_t CPSRflag = arm_read_cpsr(p); // On récupère CPSR
	  		uint8_t Cflag = get_bit(CPSRflag,C); // On récupère le flag C de CPSR
		
				if (Rs_val == 0){
					   value = Rm_val;
					   *shifter_carry_out = Cflag;
					}
					else if (Rs_val < 32){
					   value = asr(Rm_val, Rs_val);
					   *shifter_carry_out = get_bit(Rm_val,(Rs_val - 1));
					}
					else if (Rs_val >= 32){
					   if (get_bit(Rm_val,31) == 0){
									value = 0;
									*shifter_carry_out = get_bit(Rm_val,31);
					   }
					   else if(get_bit(Rm_val,31) == 1){
						  value = 0xFFFFFFFF;
						  *shifter_carry_out = get_bit(Rm_val,31);
					   }
					}
	}
	
	return value;

}
//---------------------------------------------------
// ROR : retourne la valeur du shifter_operand + décalage à droite
uint32_t ror_shift(arm_core p, uint32_t ins, int *shifter_carry_out){

	 //Si I_shift = 0, le shift est une valeur immédiate (shift_imm), sinon c'est un registre (Rs)
	uint8_t I_shift = get_bit(ins,4);
	uint32_t value; //valeur retournée
	
	//RECUPERATION DE LA VALEUR DE RM
	uint8_t Rm = get_bits(ins,3,0);
	uint32_t Rm_val = arm_read_register(p,Rm);
	
	//RECUPERATION DU CFLAG
	uint32_t CPSRflag = arm_read_cpsr(p); // On récupère CPSR
	uint32_t Cflag = get_bit(CPSRflag,C); // On récupère le flag C de CPSR

	//CAS IMMEDIAT
	if(I_shift == 0){
	
		uint8_t shift_imm = get_bits(ins,11,7); 
		
		if (shift_imm == 0){ //RRX
					 value = (Cflag << 31) | (Rm_val >> 1); 
					 *shifter_carry_out = get_bit(Rm_val,0);
		}
		else {
					 value = ror(Rm_val,shift_imm);
					 *shifter_carry_out = get_bit(Rm_val,(shift_imm - 1));
		}
		
	}
	
	//CAS REGISTRE
	else {
		
				uint8_t Rs = get_bits(ins,11,8);
				uint32_t Rs_val = arm_read_register(p,Rs);
				Rs_val = get_bits(Rs_val, 7, 0);
		
					if (Rs_val == 0){
							 value = Rm_val;
							 *shifter_carry_out = Cflag;
						}	
					else {
					
								Rs_val = get_bits(Rs_val, 4, 0);	
								
								if (Rs_val == 0){
									 value = Rm_val;
									 *shifter_carry_out = get_bit(Rm_val,31);
								}
								else if (Rs_val > 0){
									 value = ror(Rm_val, Rs_val);
									 *shifter_carry_out = get_bit(Rm_val,(Rs_val - 1));
								}
					}
	}
	
	return value;

}
//---------------------------------------------------
uint32_t register_alone(arm_core p, uint32_t ins, int *shifter_carry_out){

			uint8_t Rm = get_bits(ins,3,0);
			uint32_t value; //valeur retournée
			
			uint32_t CPSRflag = arm_read_cpsr(p); // On récupère CPSR
			uint32_t Cflag = get_bit(CPSRflag,C); // On récupère le flag C de CPSR
					
			value = arm_read_register(p,Rm);
			*shifter_carry_out = Cflag;
			
			return value;

}

//---------------------------------------------------
//Fonction qui donne la valeur de Shift_op : cas valeur immédiate / cas registre 
uint32_t shiftval(arm_core p, uint32_t ins, int *shifter_carry_out){ 

   uint32_t I = get_bit(ins,25);
   uint32_t value;   
 	
   //VALEUR IMMEDIATE
   if (I == 1){
   
		 	uint32_t rotate_imm = get_bits(ins,11,8); 
		 	uint32_t immed_8 = get_bits(ins,7,0);
		 
		 	uint32_t CPSRflag = arm_read_cpsr(p); // On récupère CPSR
			uint8_t Cflag = get_bit(CPSRflag,C); // On récupère le flag C de CPSR
		 	
		  value = ror(immed_8, (rotate_imm * 2));  

			if (rotate_imm == 0){
					*shifter_carry_out = Cflag;	
			}
			else {
					*shifter_carry_out = get_bit(value,31);   
			}
		
   }
   
   //REGISTRE
   else if (I == 0){

					uint8_t shift = get_bits(ins,6,5);  
						 			
						switch(shift){
				
							case LSL: 
								if (get_bits(ins,11,7) == 0){
									value = register_alone(p,ins, shifter_carry_out);
								}
								else {
									value = lsl_shift(p,ins, shifter_carry_out);
								}
								break;

							case LSR:
								value = lsr_shift(p,ins, shifter_carry_out);
								break;
					
							case ASR:
								value = asr_shift(p,ins, shifter_carry_out);
								break;

							case ROR:
								value = ror_shift(p,ins, shifter_carry_out);
								break; 
						}	
							 
		}		
		
		return value;
    
}
//---------------------------------------------------
// Si S = 1, alors il faut actualiser les FLAGS
void Is_S(arm_core p, uint32_t ins, int Cbit, int Vbit){
	
	uint8_t Rd = get_bits(ins ,15, 12);
	uint8_t S = get_bit(ins, 20);
	uint32_t value_Rd = arm_read_register(p, Rd);
	
	if((S == 1) && (Rd == 15)){
			if(arm_current_mode_has_spsr(p)){
					uint32_t cpsr = arm_read_spsr(p);
					arm_write_cpsr(p, cpsr);
			}
			else {
				return; //UNDPREDICTABLE
			}
		}
		else if (S == 1) {
			set_ZNCV_CPSR(p, (value_Rd == 0) , get_bit(value_Rd ,31) , Cbit, Vbit);
		}
}
//---------------------------------------------------
//And : Effectue le "ET" bit à bit 
void and(arm_core p, uint32_t ins, int *shifter_carry_out){
	
	uint32_t value_OP1, value_OP2, value; 

	//RECUPARATION DES OPERANDES
	uint8_t Rn = get_bits(ins ,19, 16); //La 1ère opérande (un registre)	
	value_OP1 = arm_read_register(p, Rn);
	value_OP2 = shiftval(p, ins, shifter_carry_out);//récupéré depuis le shifter_operand
	
	//TRAITEMENT DE OPERATION
	value = value_OP1 & value_OP2;
	
	//RANGEMENT DU RESULTAT DANS Rd
	uint8_t Rd = get_bits(ins ,15, 12); //Le registre résultat
	arm_write_register(p, Rd, value);
	
}
//---------------------------------------------------
//Exclusive OR : Effectue le "OU" exclusif bit à bit 
void eor(arm_core p, uint32_t ins, int *shifter_carry_out){
	
	uint32_t value_OP1, value_OP2, value; 

	//RECUPARATION DES OPERANDES
	uint8_t Rn = get_bits(ins ,19, 16); //La 1ère opérande (un registre)	
	value_OP1 = arm_read_register(p, Rn);
	value_OP2 = shiftval(p, ins, shifter_carry_out);//récupéré depuis le shifter_operand
	
	//TRAITEMENT DE OPERATION
	value = value_OP1 ^ value_OP2;
	
	//RANGEMENT DU RESULTAT DANS Rd
	uint8_t Rd = get_bits(ins ,15, 12); //Le registre résultat
	arm_write_register(p, Rd, value);
	
}
//---------------------------------------------------
//Substract : Effectue la soustraction entre les 2 opérandes
//ATTENTION, LE FLAGS V CHANGE ici
void sub(arm_core p, uint32_t ins, int *Cbit, int *Vbit ){
	
	uint32_t value_OP1, value_OP2, value; 

	//RECUPARATION DES OPERANDES
	uint8_t Rn = get_bits(ins ,19, 16); //La 1ère opérande (un registre)
	int shifter_carry_out;
	value_OP1 = arm_read_register(p, Rn);
	value_OP2 = shiftval(p, ins, &shifter_carry_out);//récupéré depuis le shifter_operand
	
	//TRAITEMENT DE OPERATION
	value = value_OP1 - value_OP2;
	
	//RANGEMENT DU RESULTAT DANS Rd
	uint8_t Rd = get_bits(ins ,15, 12); //Le registre résultat
	arm_write_register(p, Rd, value);
	
	//CHANGEMENT DU ZNCV
	*Cbit = !borrow_from(value_OP1, value_OP2);
	*Vbit = overflow_from(value_OP1, value_OP2, 0);
}
//---------------------------------------------------
//Reverse Substract : Effectue la soustraction entre les 2 opérandes (inversé)
void rsb(arm_core p, uint32_t ins, int *Cbit, int *Vbit){
	
	uint32_t value_OP1, value_OP2, value; 

	//RECUPARATION DES OPERANDES
	uint8_t Rn = get_bits(ins ,19, 16); //La 1ère opérande (un registre)
	int shifter_carry_out;
	value_OP1 = arm_read_register(p, Rn);
	value_OP2 = shiftval(p, ins, &shifter_carry_out);//récupéré depuis le shifter_operand
	
	//TRAITEMENT DE OPERATION
	value = value_OP2 - value_OP1;
	
	//RANGEMENT DU RESULTAT DANS Rd
	uint8_t Rd = get_bits(ins ,15, 12); //Le registre résultat
	arm_write_register(p, Rd, value);

	//CHANGEMENT DU ZNCV
	*Cbit = !borrow_from(value_OP2, value_OP1);
	*Vbit = overflow_from(value_OP2, value_OP1, 0);
}
//---------------------------------------------------
//Addition : Effectue l'addition entre les 2 opérandes
//ATTENTION, LE FLAGS V CHANGE ici
void add(arm_core p, uint32_t ins, int *Cbit, int *Vbit ){
	
	uint32_t value_OP1, value_OP2, value; 

	//RECUPARATION DES OPERANDES
	uint8_t Rn = get_bits(ins ,19, 16); //La 1ère opérande (un registre)
	int shifter_carry_out;
	value_OP1 = arm_read_register(p, Rn);
	value_OP2 = shiftval(p, ins, &shifter_carry_out);//récupéré depuis le shifter_operand
	
	//TRAITEMENT DE OPERATION
	value = value_OP1 + value_OP2;
	
	//RANGEMENT DU RESULTAT DANS Rd
	uint8_t Rd = get_bits(ins ,15, 12); //Le registre résultat
	arm_write_register(p, Rd, value);
	
	//CHANGEMENT DU ZNCV
	*Cbit = carry_from(value_OP1, value_OP2);
	*Vbit = overflow_from(value_OP1, value_OP2, 1);
}
//---------------------------------------------------
//Addition With Carry : Effectue l'addition entre les 2 opérandes + retenue
//ATTENTION, LE FLAGS V CHANGE ici
void adc(arm_core p, uint32_t ins, int *Cbit, int *Vbit ){
	
	uint32_t value_OP1, value_OP2, value, value_CPSR; 

	//RECUPARATION DES OPERANDES
	uint8_t Rn = get_bits(ins ,19, 16); //La 1ère opérande (un registre)
	int shifter_carry_out;
	value_OP1 = arm_read_register(p, Rn);
	

	value_OP2 = shiftval(p, ins, &shifter_carry_out);//récupéré depuis le shifter_operand
	
	//TRAITEMENT DE OPERATION
	value_CPSR = arm_read_cpsr(p);
	value = value_OP1 + value_OP2 + get_bit(value_CPSR, C);
	
	//RANGEMENT DU RESULTAT DANS Rd
	uint8_t Rd = get_bits(ins ,15, 12); //Le registre résultat
	arm_write_register(p, Rd, value);
	
	//CHANGEMENT DU ZNCV
	*Cbit = carry_from(value_OP1, value_OP2 + get_bit(value_CPSR, C) );
	*Vbit = overflow_from(value_OP1, value_OP2 + get_bit(value_CPSR, C), 1);

}
//---------------------------------------------------
//Substract With Carry : Effectue la soustraction entre les 2 opérandes + retenue
//ATTENTION, LE FLAGS V CHANGE ici
void sbc(arm_core p, uint32_t ins, int *Cbit, int *Vbit ){
	
	uint32_t value_OP1, value_OP2, value, value_CPSR; 

	//RECUPARATION DES OPERANDES
	uint8_t Rn = get_bits(ins ,19, 16); //La 1ère opérande (un registre)
	int shifter_carry_out;
	value_OP1 = arm_read_register(p, Rn);
	value_OP2 = shiftval(p, ins, &shifter_carry_out);//récupéré depuis le shifter_operand
	
	//TRAITEMENT DE OPERATION
	value_CPSR = arm_read_cpsr(p);
	int Cval = get_bit(value_CPSR, C);
	value = value_OP1 - value_OP2 - !Cval ;
	
	//RANGEMENT DU RESULTAT DANS Rd
	uint8_t Rd = get_bits(ins ,15, 12); //Le registre résultat
	arm_write_register(p, Rd, value);
	
	//CHANGEMENT DU ZNCV
	*Cbit = !borrow_from(value_OP1, value_OP2 - !Cval );
	*Vbit = overflow_from(value_OP1, value_OP2 - !Cval, 0);
}
//---------------------------------------------------
//Reverse Substract With Carry : Effectue la soustraction entre les 2 opérandes + retenue
//ATTENTION, LE FLAGS V CHANGE ici
void rsc(arm_core p, uint32_t ins, int *Cbit, int *Vbit ){
	
	uint32_t value_OP1, value_OP2, value, value_CPSR; 

	//RECUPARATION DES OPERANDES
	uint8_t Rn = get_bits(ins ,19, 16); //La 1ère opérande (un registre)
	int shifter_carry_out;
	value_OP1 = arm_read_register(p, Rn);
	value_OP2 = shiftval(p, ins, &shifter_carry_out);//récupéré depuis le shifter_operand
	
	//TRAITEMENT DE OPERATION
	value_CPSR = arm_read_cpsr(p);
	int Cval = get_bit(value_CPSR, C);
	value = value_OP2 - value_OP1 - !Cval ;
	
	//RANGEMENT DU RESULTAT DANS Rd
	uint8_t Rd = get_bits(ins ,15, 12); //Le registre résultat
	arm_write_register(p, Rd, value);
	
	//CHANGEMENT DU ZNCV
	*Cbit = !borrow_from(value_OP2, value_OP1 - !Cval );
	*Vbit = overflow_from(value_OP2, value_OP1 - !Cval, 0);
}
//---------------------------------------------------
//Test : Compare la première oprande avec une valeur 
//ATTENTION : Pas de registre Rd, on modifie direct les flags
void tst(arm_core p, uint32_t ins){
		
		uint32_t value, value_OP1, value_OP2; 
		
		//RECUPARATION DES OPERANDES
		uint8_t Rn = get_bits(ins ,19, 16); //La 1ère opérande (un registre)
		int shifter_carry_out;	
		value_OP1 = arm_read_register(p, Rn);
		value_OP2 = shiftval(p, ins, &shifter_carry_out);//récupéré depuis le shifter_operand
		
		//TRAITEMENT DE OPERATION
		value = value_OP1 & value_OP2;
		
		//MODIFICATION DU CPSR 
		set_ZNCV_CPSR(p,(value == 0), get_bit(value, 31), shifter_carry_out, 9 );
}
//---------------------------------------------------
//Test Equivalence : Compare la première oprande avec une valeur 
//ATTENTION : Pas de registre Rd, on modifie direct les flags
void teq(arm_core p, uint32_t ins){
		
		uint32_t value, value_OP1, value_OP2; 
		
		//RECUPARATION DES OPERANDES
		uint8_t Rn = get_bits(ins ,19, 16); //La 1ère opérande (un registre)
		int shifter_carry_out;	
		value_OP1 = arm_read_register(p, Rn);
		value_OP2 = shiftval(p, ins, &shifter_carry_out);//récupéré depuis le shifter_operand
		
		//TRAITEMENT DE OPERATION
		value = value_OP1 ^ value_OP2;
		
		//MODIFICATION DU CPSR 
		set_ZNCV_CPSR(p,(value == 0), get_bit(value ,31), shifter_carry_out, 9 );
}
//---------------------------------------------------
//Compare : Compare la première oprande avec la 2ème opérande 
//ATTENTION : Pas de registre Rd, on modifie direct les flags
void cmp(arm_core p, uint32_t ins){
		
		uint32_t value, value_OP1, value_OP2; 
		
		//RECUPARATION DES OPERANDES
		uint8_t Rn = get_bits(ins ,19, 16); //La 1ère opérande (un registre)
		int shifter_carry_out;
		value_OP1 = arm_read_register(p, Rn);
		value_OP2 = shiftval(p, ins, &shifter_carry_out);//récupéré depuis le shifter_operand
		
		//TRAITEMENT DE OPERATION
		value = value_OP1 - value_OP2;
		
		//MODIFICATION DU CPSR 
		set_ZNCV_CPSR(p,(value == 0), get_bit(value ,31), !borrow_from(value_OP1, value_OP2), overflow_from(value_OP1, value_OP2, 0) );
}
//---------------------------------------------------
//Compare Negative : Compare la première oprande avec le négatif de la 2ème opérande 
//ATTENTION : Pas de registre Rd, on modifie direct les flags
void cmn(arm_core p, uint32_t ins){
		
		uint32_t value, value_OP1, value_OP2; 
		
		//RECUPARATION DES OPERANDES
		uint8_t Rn = get_bits(ins ,19, 16); //La 1ère opérande (un registre)
		int shifter_carry_out;	//inutile ici
		value_OP1 = arm_read_register(p, Rn);
		value_OP2 = shiftval(p, ins, &shifter_carry_out);//récupéré depuis le shifter_operand
		
		//TRAITEMENT DE OPERATION
		value = value_OP1 + value_OP2;
		
		//MODIFICATION DU CPSR 
		set_ZNCV_CPSR(p,(value == 0), get_bit(value ,31), 9, 9 );
}
//---------------------------------------------------
//OR : Correspond ou OR logique, bit par bit, de deux valeurs
void orr(arm_core p, uint32_t ins, int *shifter_carry_out){
		
		uint32_t value, value_OP1, value_OP2; 
		
		//RECUPARATION DES OPERANDES
		uint8_t Rn = get_bits(ins ,19, 16); //La 1ère opérande (un registre)
		value_OP1 = arm_read_register(p, Rn);
		value_OP2 = shiftval(p, ins, shifter_carry_out);//récupéré depuis le shifter_operand
		
		//TRAITEMENT DE OPERATION
		value = value_OP1 | value_OP2;
		
		//RANGEMENT DU RESULTAT DANS Rd
		uint8_t Rd = get_bits(ins ,15, 12); //Le registre résultat	
		arm_write_register(p, Rd, value);
		
}
//---------------------------------------------------
//Move : envoie la valeur dans un registre donné
void mov(arm_core p, uint32_t ins, int *shifter_carry_out){
		
		uint32_t value_OP; 
		
		//RECUPARATION DES OPERANDES
		value_OP = shiftval(p, ins, shifter_carry_out);
		
		//TRAITEMENT DE OPERATION
		uint8_t Rd = get_bits(ins ,15, 12); //Le registre résultat
		arm_write_register(p, Rd, value_OP);
}
//---------------------------------------------------
//Bit Clear : Met à 0 les bits sélectionnés 
void bic(arm_core p, uint32_t ins, int *shifter_carry_out){
		
		uint32_t value, value_OP1, value_OP2; 
		
		//RECUPARATION DES OPERANDES
		uint8_t Rn = get_bits(ins ,19, 16); //La 1ère opérande (un registre)
		value_OP1 = arm_read_register(p, Rn);
		value_OP2 = shiftval(p, ins, shifter_carry_out);
		
		//TRAITEMENT DE OPERATION
		value = value_OP1 & (~value_OP2);
		
		//RANGEMENT DU RESULTAT DANS Rd
		uint8_t Rd = get_bits(ins ,15, 12); //Le registre résultat
		arm_write_register(p, Rd, value);
		
		
}
//---------------------------------------------------
//donne le complément de la valeur (ou du contenu du registre). Exemple : mvn(1001) = 0110
void mvn(arm_core p, uint32_t ins, int *shifter_carry_out){
		
		uint32_t value, value_OP;
		
		//RECUPARATION DES OPERANDES
		value_OP = shiftval(p, ins, shifter_carry_out);
		
		//TRAITEMENT DE OPERATION
		value = ~(value_OP); 
		
		//RANGEMENT DU RESULTAT DANS Rd
		uint8_t Rd = get_bits(ins ,15, 12); //Le registre résultat
		arm_write_register(p, Rd, value);

		
}
//---------------------------------------------------
void traitement(arm_core p, uint32_t ins){

		uint8_t codeop = get_bits(ins ,24, 21); //le code de l'opérateur	
		
		int Cbit = 9;
		int Vbit = 9;
		
		switch(codeop){
			case AND :
					and(p, ins, &Cbit);
					Is_S(p, ins, Cbit, Vbit);
					break;
			case EOR :
					eor(p, ins, &Cbit);
					Is_S(p, ins, Cbit, Vbit);
					break;
			case SUB :
					sub(p, ins, &Cbit, &Vbit);
					Is_S(p, ins, Cbit, Vbit);
					break;
			case RSB :
					rsb(p, ins, &Cbit, &Vbit);
					Is_S(p, ins, Cbit, Vbit);	
					break;
			case ADD :
					add(p, ins, &Cbit, &Vbit);
					Is_S(p, ins, Cbit, Vbit);
					break;
			case ADC :
					adc(p, ins, &Cbit, &Vbit);
					Is_S(p, ins, Cbit, Vbit);
					break;
			case SBC :
					sbc(p, ins, &Cbit, &Vbit);
					Is_S(p, ins, Cbit, Vbit);
					break;
			case RSC :
					rsc(p, ins, &Cbit, &Vbit);
					Is_S(p, ins, Cbit, Vbit);
					break;
			
			//--------------------
			case TST :
					tst(p, ins);
					break;
			case TEQ :
					teq(p, ins);
					break;
			case CMP :
					cmp(p, ins);
					break;
			case CMN :
					cmn(p, ins);
					break;
			case ORR :
					orr(p, ins, &Cbit);
					Is_S(p, ins, Cbit, Vbit);
					break;
			case MOV :
					mov(p, ins, &Cbit);
					Is_S(p, ins, Cbit, Vbit);
					break;
			case BIC :
					bic(p, ins, &Cbit);
					Is_S(p, ins, Cbit, Vbit);
					break;
			case MVN : 
					mvn(p, ins, &Cbit);
					Is_S(p, ins, Cbit, Vbit);
					break;
		}
		
}
//---------------------------------------------------
/* Decoding functions for different classes of instructions */
int arm_data_processing_shift(arm_core p, uint32_t ins) {

			uint8_t premiersbits_codeop = get_bits(ins,24,23); 
			//Utile car si codeop = 10xx, l'instruction est invalide ou pas à traiter
			uint8_t form = get_bit(ins,4);
			uint8_t bit7 = get_bit(ins,7);
   
   if (premiersbits_codeop == 2) {
		arm_data_processing_immediate_msr(p,ins);
   }
   
   else if ((form == 0) || ((form == 1) && (bit7 == 0)) || (get_bit(ins,25) == 1)){
		traitement(p,ins);
   }
   
   else if((form == 1) && (bit7 == 1)){
		arm_load_store(p,ins); // extra load/store OU Multiplies (figure A3-3 et A3-5)
   }
   
   return 0;
}

//---------------------------------------------------
int arm_data_processing_immediate_msr(arm_core p, uint32_t ins) {
	
	uint8_t R = get_bit(ins,22);
	uint8_t Rd = get_bits(ins,15,12);
	
	if (R == 1){
		arm_write_register(p, Rd, arm_read_spsr(p));		
	}
	else {
		arm_write_register(p, Rd, arm_read_cpsr(p));
	}
		
	return 0;
}

//---------------------------------------------------
