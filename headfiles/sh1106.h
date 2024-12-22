#ifndef __SH1106_H__
#define __SH1106_H__
typedef unsigned  char uint8_t;
void sh1106_reset(void);
void sh1106_write_cmd(uint8_t);
void sh1106_write_data(uint8_t data);
void sh1106_init(void);
void sh1106_clear(void);
void sh1106_draw_point(uint8_t chXpos, uint8_t chYpos, uint8_t chPoint);
void sh1106_draw_bitmap(uint8_t chXpos, uint8_t chYpos, const uint8_t *pchBmp, uint8_t chWidth, uint8_t chHeight);
void sh1106_erase_bitmap(uint8_t chXpos, uint8_t chYpos, const uint8_t *pchBmp, uint8_t chWidth, uint8_t chHeight);
#endif
