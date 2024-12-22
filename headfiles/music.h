#ifndef __MUSIC_H__
#define __MUSIC_H__
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
extern int imus;
extern const uint16_t tone[];
extern uint8_t music_tone[344];
extern uint8_t music_time[344];
void set_pwm(uint16_t period, uint16_t pulse);
#endif
