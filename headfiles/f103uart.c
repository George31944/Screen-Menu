#include "f103uart.h"
int fputc(int ch, FILE *f)		//	�Զ��崮�ڴ�ӡ����
{
    HAL_UART_Transmit(&huart1 , (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
}
