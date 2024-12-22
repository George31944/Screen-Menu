#include "sh1106.h"
#include "gpio.h"
#include "spi.h"
#include "photo.h"
void sh1106_reset()		//	复位函数
{
	HAL_GPIO_WritePin(GPIOB,OLED_RES_Pin,GPIO_PIN_SET);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(GPIOB,OLED_RES_Pin,GPIO_PIN_RESET);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(GPIOB,OLED_RES_Pin,GPIO_PIN_SET);
	HAL_Delay(1000);
}
void sh1106_write_cmd(uint8_t cmd)			//	发送指令
{
	HAL_GPIO_WritePin(GPIOB,OLED_DC_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi3,&cmd,1,1000);
}
void sh1106_write_data(uint8_t data)		//	发送数据
{
	HAL_GPIO_WritePin(GPIOB,OLED_DC_Pin,GPIO_PIN_SET);
	HAL_SPI_Transmit(&hspi3,&data,1,1000);
}

void sh1106_init(void)						//	初始化函数
{       
        sh1106_reset();
        sh1106_write_cmd(0xAE);//--turn off oled panel
        sh1106_write_cmd(0x00);//---set low column address 00->02
        sh1106_write_cmd(0x10);//---set high column address
        sh1106_write_cmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
        sh1106_write_cmd(0x81);//--set contrast control register
        sh1106_write_cmd(0xCF);// Set SEG Output Current Brightness
        sh1106_write_cmd(0xA1);//--Set SEG/Column Mapping     
        sh1106_write_cmd(0xC0);//Set COM/Row Scan Direction   
        sh1106_write_cmd(0xA6);//--set normal display
        sh1106_write_cmd(0xA8);//--set multiplex ratio(1 to 64)
        sh1106_write_cmd(0x3f);//--1/64 duty
        sh1106_write_cmd(0xD3);//-set display offset Shift Mapping RAM Counter (0x00~0x3F)
        sh1106_write_cmd(0x00);//-not offset
        sh1106_write_cmd(0xd5);//--set display clock divide ratio/oscillator frequency
        sh1106_write_cmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
        sh1106_write_cmd(0xD9);//--set pre-charge period
        sh1106_write_cmd(0xF1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
        sh1106_write_cmd(0xDA);//--set com pins hardware configuration
        sh1106_write_cmd(0x12);
        sh1106_write_cmd(0xDB);//--set vcomh
        sh1106_write_cmd(0x40);//Set VCOM Deselect Level
        sh1106_write_cmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
        sh1106_write_cmd(0x02);//
        sh1106_write_cmd(0x8D);//--set Charge Pump enable/disable
        sh1106_write_cmd(0x14);//--set(0x10) disable
        sh1106_write_cmd(0xA4);// Disable Entire Display On (0xa4/0xa5)
        sh1106_write_cmd(0xA6);// Disable Inverse Display On (0xa6/a7) 
        sh1106_write_cmd(0xAF);//--turn on oled panel
}

void sh1106_clear()							//	清屏函数,将所有led关闭
{
	for(int i=0;i<8;i++)
	{
		sh1106_write_cmd(0xb0+i);		//	页地址，第i页
		sh1106_write_cmd(0x02);			//	列起始地址低4位,0002
		sh1106_write_cmd(0x10);			//	列起始地址高4位,0000
		for(int j=0;j<128;j++)			//	将128字节逐个清零
		{
			display[i][j]=0;
			sh1106_write_data(display[i][j]);			
		}
	}
}

void sh1106_draw_point(uint8_t chXpos, uint8_t chYpos, uint8_t chPoint)		//	画点函数,x、y为横纵坐标（从0开始）
{
        uint8_t chPos, chBx, chTemp = 0;
				uint8_t x_low,x_high=0;			//	列地址的低四位和高四位			
        if (chXpos > 127 || chYpos > 63) {
                return;
        }
        //chYpos坐标转换，因为我们用8个字节管理了64个bit,所以需要把y坐标转换到对应的字节bit位置
        chPos = 7 - chYpos / 8;   //找出那一页
        chBx = chYpos % 8;        //找出哪一位
        chTemp = 1 << (7 - chBx); //把对应位置1
        if (chPoint) {
            display[chPos][chXpos] |= chTemp;
        } else {
            display[chPos][chXpos] &= ~chTemp;		//	display矩阵的对应字节置1或0
        }
      sh1106_write_cmd(0xb0+chPos);			//	页地址
			x_low=0x0f&(chXpos+2);
			x_high=(0xf0&(chXpos+2))>>4;
			sh1106_write_cmd(x_low);						//	列起始地址低4位
			sh1106_write_cmd(x_high|0x10);			//	列起始地址高4位
			sh1106_write_data(display[chPos][chXpos]);
}
//	画bmp图片，chXpos、chYpos为起始位置，pchBmp为生成的点阵数组，chHeight、chWidth为高度和宽度(汉字最好16*8)
void sh1106_draw_bitmap(uint8_t chXpos, uint8_t chYpos, const uint8_t *pchBmp, uint8_t chWidth, uint8_t chHeight)		
{
    uint16_t i, j, byteWidth = (chWidth + 7) / 8;
    //遍历图片的宽高，取出每一点，判断为1的位，为需要点亮的点，通过画点函数绘制到屏幕
    for(j = 0; j < chHeight; j ++){
        for(i = 0; i < chWidth; i ++ ) {
            if(*(pchBmp + j * byteWidth + i / 8) & (128 >> (i & 7))) {
                sh1106_draw_point(chXpos + i, chYpos + j, 1);
            }
        }
    }
}
// 擦除某些点，保持其他点不变
void sh1106_erase_bitmap(uint8_t chXpos, uint8_t chYpos, const uint8_t *pchBmp, uint8_t chWidth, uint8_t chHeight)		
{
    uint16_t i, j, byteWidth = (chWidth + 7) / 8;
    //遍历图片的宽高，取出每一点，判断为1的位，为需要擦除的点，通过画点函数绘制到屏幕
    for(j = 0; j < chHeight; j ++){
        for(i = 0; i < chWidth; i ++ ) {
            if(*(pchBmp + j * byteWidth + i / 8) & (128 >> (i & 7))) {
                sh1106_draw_point(chXpos + i, chYpos + j, 0);
            }
        }
    }
}
