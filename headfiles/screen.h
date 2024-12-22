#ifndef __SCREEN_H__
#define __SCREEN_H__
void page0(int);
void page1(float* ,float*);
void page1_changetmp(float* ,float* )	;
void page1_erasetmp(void);	
void page2_init(int);
void page2_speed(int);
void page2_boder_erase(int);
void page2_boder_draw(int);
void page3_init(float);
void page3_changevalue(float);
void page3_erasevalue(float);
#endif
