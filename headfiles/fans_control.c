#include "fans_control.h"
#include "tim.h"
void set_speed(int x)
{
		HAL_GPIO_WritePin(GPIOB,DC_AN2_Pin,GPIO_PIN_RESET); //设置DC_AN2为低，只保持一个方向转动
		HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_3); //开启PWM输出
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_3, 200+x*100);	//设置占空比
}
void fans_close()
{
		HAL_TIMEx_PWMN_Stop(&htim1,TIM_CHANNEL_3);
}
