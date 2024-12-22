#ifndef __GXHT3L_H__
#define __GXHT3L_H__

#define CRC8_POLYNOMIAL 0x31
typedef unsigned short int uint16_t;
typedef unsigned  char uint8_t;
extern uint8_t CheckCrc8(uint8_t* const , uint8_t );		
extern uint8_t GXHT3L_Dat_To_Float(uint8_t* const , float* , float* );
#endif
