#include "f103uart.h"
int fputc(int ch, FILE *f)		//	自定义串口打印函数
{
    HAL_UART_Transmit(&huart1 , (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
}
