#include "fans_control.h"
#include "tim.h"
void set_speed(int x)
{
		HAL_GPIO_WritePin(GPIOB,DC_AN2_Pin,GPIO_PIN_RESET); //����DC_AN2Ϊ�ͣ�ֻ����һ������ת��
		HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_3); //����PWM���
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_3, 200+x*100);	//����ռ�ձ�
}
void fans_close()
{
		HAL_TIMEx_PWMN_Stop(&htim1,TIM_CHANNEL_3);
}
