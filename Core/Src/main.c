/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "photo.h"
#include "sh1106.h"
#include "screen.h"
#include "gxht3l.h"
#include "fans_control.h"
#include "music.h"
#include "f103uart.h"
#include "wifi.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
extern void page1_changetmp(float* ,float* )	;
extern void page1_erasetmp(void);	
extern DMA_HandleTypeDef hdma_uart4_rx;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define CRC8_POLYNOMIAL 0x31
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
//用户自定义数据区
int icon_pos=0;			//	主界面功能选择	
int icon_back=0;		//	是否进入次界面
int icon_fans=0;		//	风扇界面功能选择
int fans_state=0;		//	风扇转速，0表示关
int icon_light=0;		//	灯光状态
int icon_music=0;		//	音乐状态，0关1开

uint8_t commond[2]={0x21,0x30};		//	转换速率与重复率
uint8_t recieve[6]={0};						//	单次转换得到的数据
float temperature=0;
float humidity=0;
uint8_t bur[2]={0xe0,0x00};				//		连续读写指令

uint16_t ADC_Value;
float voltage=0;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
	
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
	
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_I2C2_Init();
  MX_SPI3_Init();
  MX_TIM1_Init();
  MX_TIM6_Init();
  MX_TIM2_Init();
  MX_DMA_Init();
  MX_UART4_Init();
  MX_TIM7_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	printf("连接云端...");
	wifi_init();
	sh1106_init();
	page0(icon_light);
	sh1106_draw_bitmap(7+icon_pos*30,22,bianjie,24,26);
	HAL_I2C_Master_Transmit(&hi2c2,0x88,commond,2,1000);		//	设置温度转换转换速率	
//	HAL_ADCEx_Calibration_Start(&hadc1); //ADC校准
//  HAL_ADC_Start(&hadc1);     //启动ADC转换，设置为连续采样后，只需开启一次
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		//主循环内连续温湿度
		HAL_I2C_Master_Transmit(&hi2c2,0x88,bur,2,1000);
		if(HAL_I2C_Master_Receive(&hi2c2,0x89,recieve,6,1000)==HAL_OK)		
		{
			if(GXHT3L_Dat_To_Float(recieve,&temperature,&humidity)==0)		//	转换为实际值
			{
				sprintf((char *)tx_buf,AT_PUB_TEM,temperature);							//	装载温湿度数据
				send_cmd(tx_buf,(uint8_t *)ACK_OK);													//	上报温湿度
				sprintf((char *)tx_buf,AT_PUB_HIM,humidity);	
				send_cmd(tx_buf,(uint8_t *)ACK_OK);	
			}
		}
//		HAL_ADC_PollForConversion(&hadc1,50);		//查询函数，查询转换完成标志位。每次采样，CUP在这里都要 
//																						//等待采样完成才能进行下一步，超时时间可设置大于采样时间
//    ADC_Value = HAL_ADC_GetValue(&hadc1);   //获取AD值
		HAL_Delay(2000);
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
#define DEBOUNCE_TIME 50 //   去抖时间，单位为ms
uint32_t falling_tick = 0;
uint32_t rising_tick = 0;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){        
    if(GPIO_Pin == key3_Pin)  //判断key3是否按下
    {	
        if(HAL_GPIO_ReadPin(key3_GPIO_Port,key3_Pin) == 0){
            //触发中断后，如果读取到IO电平是低，则是下降沿
            falling_tick = HAL_GetTick();
        }else if(HAL_GPIO_ReadPin(key3_GPIO_Port,key3_Pin) == 1 && falling_tick  != 0){
            //触发中断后，如果读取到IO电平是高，则是上升沿
            rising_tick = HAL_GetTick();
            if (rising_tick - falling_tick > DEBOUNCE_TIME)
            {
                // 按键按下需要执行的代码
							if(icon_back==0)				//	未进入次界面
							{	//	消除原来的框
							sh1106_erase_bitmap(7+icon_pos*30,22,bianjie,24,26);
							//	画新框
							icon_pos++;		
							icon_pos%=4;
               sh1106_draw_bitmap(7+icon_pos*30,22,bianjie,24,26);	
							}
							else if(icon_back==1)	//	进入次界面
							{
								if(icon_pos==1)			//	风扇界面
									{
										page2_boder_erase(icon_fans);
										icon_fans++;
										icon_fans%=4;
										page2_boder_draw(icon_fans);
									}
							}
            }
            falling_tick = rising_tick = 0;
        }
    }
		else if(GPIO_Pin == key1_Pin)	//判断key1是否按下
			{
        if(HAL_GPIO_ReadPin(key1_GPIO_Port,key1_Pin) == 0){
            //触发中断后，如果读取到IO电平是低，则是下降沿
            falling_tick = HAL_GetTick();
        }else if(HAL_GPIO_ReadPin(key1_GPIO_Port,key1_Pin) == 1 && falling_tick  != 0){
            //触发中断后，如果读取到IO电平是高，则是上升沿
            rising_tick = HAL_GetTick();
            if (rising_tick - falling_tick > DEBOUNCE_TIME)
            {
                // 按键按下需要执行的代码
							if(icon_back==0)				//	未进入次界面
							{	//	消除原来的框
							sh1106_erase_bitmap(7+icon_pos*30,22,bianjie,24,26);
							//	画新框
							icon_pos--;		
							icon_pos=icon_pos<0?3:icon_pos;
               sh1106_draw_bitmap(7+icon_pos*30,22,bianjie,24,26);		//	指示框左移
							}
							else if(icon_back==1)		//	进入次界面	
							{
									if(icon_pos==1)			//	风扇界面
									{
										//	风扇界面指示框切换
										page2_boder_erase(icon_fans);
										icon_fans--;		
										icon_fans=icon_fans<0?3:icon_fans;
										page2_boder_draw(icon_fans);
									}
							}
            }
            falling_tick = rising_tick = 0;
        }
    }
		else if(GPIO_Pin == key2_Pin)	//判断key2(确认按键)是否按下
		{
			if(HAL_GPIO_ReadPin(key2_GPIO_Port,key2_Pin) == 0){
            //触发中断后，如果读取到IO电平是低，则是下降沿
            falling_tick = HAL_GetTick();
        }else if(HAL_GPIO_ReadPin(key2_GPIO_Port,key2_Pin) == 1 && falling_tick  != 0){
            //触发中断后，如果读取到IO电平是高，则是上升沿
            rising_tick = HAL_GetTick();
            if (rising_tick - falling_tick > DEBOUNCE_TIME)
            {	
								if (icon_back == 0) //	未进入次界面
								{
										if (icon_pos == 0) //	进入测温界面
									{
										// 跳转到温湿度界面，定时器开，每2秒刷新显示
										page1(&temperature, &humidity);
										// tim6 open
										HAL_TIM_Base_Start_IT(&htim6);
										icon_back = 1;
									}
										else if (icon_pos == 1) //  进入风扇界面
									{
										// 打开风扇界面
										page2_init(fans_state);
										sh1106_draw_bitmap(8,4,bianjie2,24,26);
										icon_back=1;
									}
										else if(icon_pos==3)		//	开关灯
										{
											HAL_GPIO_TogglePin(GPIOA,led_Pin);
											if(icon_light==0)
											{
												icon_light=1;
												sh1106_erase_bitmap(97,22,gImage_light_off,24,26);
												sh1106_draw_bitmap(97,22,gImage_light_on,24,26);
											}
											else
											{
												icon_light=0;
												sh1106_erase_bitmap(97,22,gImage_light_on,24,26);
												sh1106_draw_bitmap(97,22,gImage_light_off,24,26);
											}	
										}
											else			//	播放暂停音乐
											{
//												icon_back=1;
//												voltage=ADC_Value*3.3/4096.0;
//												page3_init(voltage);
//												// tim6 open
//												HAL_TIM_Base_Start_IT(&htim6);
													if(icon_music==0)
													{
														icon_music=1;
														//timer7开
														HAL_TIM_Base_Start_IT(&htim7);
														//切换音乐图标
													}
													else
													{
														icon_music=0;
														//timer7关
														HAL_TIM_Base_Stop_IT(&htim7);
														HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_4);		//	蜂鸣器关
														//切换音乐图标
													}
												
											}
											
								}
								else if (icon_back == 1) //  进入了次界面
								{
										if (icon_pos == 0) // 测温界面
										{
											// tim6 close
											HAL_TIM_Base_Stop_IT(&htim6);
											icon_back = 0;
											// 退出该功能，回到主界面
											page0(icon_light);
											sh1106_draw_bitmap(7+icon_pos*30,22,bianjie,24,26);
										}
										else if(icon_pos == 1) // 风扇界面
										{
												if (icon_fans == 0) // 开关图标
												{	
													if (fans_state == 0)
													{
															fans_state=1;	
															set_speed(fans_state);		//开风扇
															sh1106_erase_bitmap(8,4,gImage_switch_off,24,26);
															sh1106_draw_bitmap(8,4,gImage_switch_on,24,26);
													}
													else
													{
															fans_close();				//关风扇
															fans_state=0;
															page2_speed(fans_state);
															sh1106_erase_bitmap(8,4,gImage_switch_on,24,26);
															sh1106_draw_bitmap(8,4,gImage_switch_off,24,26);
													}
												}
												else if (icon_fans == 1) //  返回图标
														{
															icon_back = 0;
															icon_fans=0;
															// 退出该功能,回到主界面,风扇状态不变
															page0(icon_light);
															sh1106_draw_bitmap(7+icon_pos*30,22,bianjie,24,26);
														}
												else if (icon_fans == 2) //  升速
														{
																if(fans_state<3&&fans_state)
																{fans_state++;
																	set_speed(fans_state);
																	page2_speed(fans_state);}
														}
												else	//  降速
														{
																if(fans_state>1&&fans_state)
																{fans_state--;
																	set_speed(fans_state);
																	page2_speed(fans_state);}
														}
										}
							}			
           }
            falling_tick = rising_tick = 0;
        }
    }
}
//	定时器中断回调函数
int res=0;
int m=0;	
int mes=10;			//	tim7初始定时时间(ms)
int dist=1;			//	停顿0or音符1
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
		if(htim == &htim6)  //判断中断是否来自于定时器6
   {
		 if(icon_pos==0)
		 {
       if(res<100)		//	定时1秒
			 {res++;}
			 else
			 {
				 res=0;
				 page1_erasetmp();	
				 page1_changetmp(&temperature, &humidity);	//	刷新温湿度
			 }
		 }
   }
	 else if(htim == &htim7)  //判断中断是否来自于定时器7
	 {
		 if(m<mes)
			m++;
		 else
		 {
			m=0;
			imus%=344;		//	循环播放
			if(imus<70)		//	前奏部分
			{
				mes=music_time[imus]*250;		//	设置音符持续时间
				HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);	//	开启pwm
				set_pwm(tone[music_tone[imus]],tone[music_tone[imus]]/2); 	//	设置音符频率
				imus++;		//	指向下一个音符	
			}
			else				//	 歌词部分
			{
				if(music_tone[imus]==21)		//	停止符
				{
					mes=music_time[imus]*60;
					HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_4);		//	关闭pwm
					imus++;		//	指向下一个音符
				}
				else			//	音符
				{
					if(dist==0)		//	上个周期播放的是停顿
					{
						dist=1;
						mes=music_time[imus]*60*4;
						HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);
						set_pwm(tone[music_tone[imus]],tone[music_tone[imus]]/2);
					}
					else					//	上个周期播放的是音符
					{
						dist=0;
						mes=music_time[imus]*10;
						HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_4);		
						imus++;		//	指向下一个音符
					}
				}
			}
		 }
	 }	 
}	

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

