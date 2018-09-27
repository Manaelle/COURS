#include "bit_operations.h"

//-----------------------------------------------------
//FONCTIONS DE BASE

//récupérer le bit à la position p
char get_bit(int32_t value, int position) {
    return (value >> position) & 1;
}

//mettre un '1' au bit de position p
int32_t set_bit(int32_t value, int position) {
    return value | (1 << position);
}

//mettre '0' au bit de position p
int32_t clr_bit(int32_t value, int position) {
    return value & ~(1 << position);
}
//-----------------------------------------------------


