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
//�û��Զ���������
int icon_pos=0;			//	�����湦��ѡ��	
int icon_back=0;		//	�Ƿ����ν���
int icon_fans=0;		//	���Ƚ��湦��ѡ��
int fans_state=0;		//	����ת�٣�0��ʾ��
int icon_light=0;		//	�ƹ�״̬
int icon_music=0;		//	����״̬��0��1��

uint8_t commond[2]={0x21,0x30};		//	ת���������ظ���
uint8_t recieve[6]={0};						//	����ת���õ�������
float temperature=0;
float humidity=0;
uint8_t bur[2]={0xe0,0x00};				//		������дָ��

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
	printf("�����ƶ�...");
	wifi_init();
	sh1106_init();
	page0(icon_light);
	sh1106_draw_bitmap(7+icon_pos*30,22,bianjie,24,26);
	HAL_I2C_Master_Transmit(&hi2c2,0x88,commond,2,1000);		//	�����¶�ת��ת������	
//	HAL_ADCEx_Calibration_Start(&hadc1); //ADCУ׼
//  HAL_ADC_Start(&hadc1);     //����ADCת��������Ϊ����������ֻ�迪��һ��
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		//��ѭ����������ʪ��
		HAL_I2C_Master_Transmit(&hi2c2,0x88,bur,2,1000);
		if(HAL_I2C_Master_Receive(&hi2c2,0x89,recieve,6,1000)==HAL_OK)		
		{
			if(GXHT3L_Dat_To_Float(recieve,&temperature,&humidity)==0)		//	ת��Ϊʵ��ֵ
			{
				sprintf((char *)tx_buf,AT_PUB_TEM,temperature);							//	װ����ʪ������
				send_cmd(tx_buf,(uint8_t *)ACK_OK);													//	�ϱ���ʪ��
				sprintf((char *)tx_buf,AT_PUB_HIM,humidity);	
				send_cmd(tx_buf,(uint8_t *)ACK_OK);	
			}
		}
//		HAL_ADC_PollForConversion(&hadc1,50);		//��ѯ��������ѯת����ɱ�־λ��ÿ�β�����CUP�����ﶼҪ 
//																						//�ȴ�������ɲ��ܽ�����һ������ʱʱ������ô��ڲ���ʱ��
//    ADC_Value = HAL_ADC_GetValue(&hadc1);   //��ȡADֵ
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
#define DEBOUNCE_TIME 50 //   ȥ��ʱ�䣬��λΪms
uint32_t falling_tick = 0;
uint32_t rising_tick = 0;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){        
    if(GPIO_Pin == key3_Pin)  //�ж�key3�Ƿ���
    {	
        if(HAL_GPIO_ReadPin(key3_GPIO_Port,key3_Pin) == 0){
            //�����жϺ������ȡ��IO��ƽ�ǵͣ������½���
            falling_tick = HAL_GetTick();
        }else if(HAL_GPIO_ReadPin(key3_GPIO_Port,key3_Pin) == 1 && falling_tick  != 0){
            //�����жϺ������ȡ��IO��ƽ�Ǹߣ�����������
            rising_tick = HAL_GetTick();
            if (rising_tick - falling_tick > DEBOUNCE_TIME)
            {
                // ����������Ҫִ�еĴ���
							if(icon_back==0)				//	δ����ν���
							{	//	����ԭ���Ŀ�
							sh1106_erase_bitmap(7+icon_pos*30,22,bianjie,24,26);
							//	���¿�
							icon_pos++;		
							icon_pos%=4;
               sh1106_draw_bitmap(7+icon_pos*30,22,bianjie,24,26);	
							}
							else if(icon_back==1)	//	����ν���
							{
								if(icon_pos==1)			//	���Ƚ���
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
		else if(GPIO_Pin == key1_Pin)	//�ж�key1�Ƿ���
			{
        if(HAL_GPIO_ReadPin(key1_GPIO_Port,key1_Pin) == 0){
            //�����жϺ������ȡ��IO��ƽ�ǵͣ������½���
            falling_tick = HAL_GetTick();
        }else if(HAL_GPIO_ReadPin(key1_GPIO_Port,key1_Pin) == 1 && falling_tick  != 0){
            //�����жϺ������ȡ��IO��ƽ�Ǹߣ�����������
            rising_tick = HAL_GetTick();
            if (rising_tick - falling_tick > DEBOUNCE_TIME)
            {
                // ����������Ҫִ�еĴ���
							if(icon_back==0)				//	δ����ν���
							{	//	����ԭ���Ŀ�
							sh1106_erase_bitmap(7+icon_pos*30,22,bianjie,24,26);
							//	���¿�
							icon_pos--;		
							icon_pos=icon_pos<0?3:icon_pos;
               sh1106_draw_bitmap(7+icon_pos*30,22,bianjie,24,26);		//	ָʾ������
							}
							else if(icon_back==1)		//	����ν���	
							{
									if(icon_pos==1)			//	���Ƚ���
									{
										//	���Ƚ���ָʾ���л�
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
		else if(GPIO_Pin == key2_Pin)	//�ж�key2(ȷ�ϰ���)�Ƿ���
		{
			if(HAL_GPIO_ReadPin(key2_GPIO_Port,key2_Pin) == 0){
            //�����жϺ������ȡ��IO��ƽ�ǵͣ������½���
            falling_tick = HAL_GetTick();
        }else if(HAL_GPIO_ReadPin(key2_GPIO_Port,key2_Pin) == 1 && falling_tick  != 0){
            //�����жϺ������ȡ��IO��ƽ�Ǹߣ�����������
            rising_tick = HAL_GetTick();
            if (rising_tick - falling_tick > DEBOUNCE_TIME)
            {	
								if (icon_back == 0) //	δ����ν���
								{
										if (icon_pos == 0) //	������½���
									{
										// ��ת����ʪ�Ƚ��棬��ʱ������ÿ2��ˢ����ʾ
										page1(&temperature, &humidity);
										// tim6 open
										HAL_TIM_Base_Start_IT(&htim6);
										icon_back = 1;
									}
										else if (icon_pos == 1) //  ������Ƚ���
									{
										// �򿪷��Ƚ���
										page2_init(fans_state);
										sh1106_draw_bitmap(8,4,bianjie2,24,26);
										icon_back=1;
									}
										else if(icon_pos==3)		//	���ص�
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
											else			//	������ͣ����
											{
//												icon_back=1;
//												voltage=ADC_Value*3.3/4096.0;
//												page3_init(voltage);
//												// tim6 open
//												HAL_TIM_Base_Start_IT(&htim6);
													if(icon_music==0)
													{
														icon_music=1;
														//timer7��
														HAL_TIM_Base_Start_IT(&htim7);
														//�л�����ͼ��
													}
													else
													{
														icon_music=0;
														//timer7��
														HAL_TIM_Base_Stop_IT(&htim7);
														HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_4);		//	��������
														//�л�����ͼ��
													}
												
											}
											
								}
								else if (icon_back == 1) //  �����˴ν���
								{
										if (icon_pos == 0) // ���½���
										{
											// tim6 close
											HAL_TIM_Base_Stop_IT(&htim6);
											icon_back = 0;
											// �˳��ù��ܣ��ص�������
											page0(icon_light);
											sh1106_draw_bitmap(7+icon_pos*30,22,bianjie,24,26);
										}
										else if(icon_pos == 1) // ���Ƚ���
										{
												if (icon_fans == 0) // ����ͼ��
												{	
													if (fans_state == 0)
													{
															fans_state=1;	
															set_speed(fans_state);		//������
															sh1106_erase_bitmap(8,4,gImage_switch_off,24,26);
															sh1106_draw_bitmap(8,4,gImage_switch_on,24,26);
													}
													else
													{
															fans_close();				//�ط���
															fans_state=0;
															page2_speed(fans_state);
															sh1106_erase_bitmap(8,4,gImage_switch_on,24,26);
															sh1106_draw_bitmap(8,4,gImage_switch_off,24,26);
													}
												}
												else if (icon_fans == 1) //  ����ͼ��
														{
															icon_back = 0;
															icon_fans=0;
															// �˳��ù���,�ص�������,����״̬����
															page0(icon_light);
															sh1106_draw_bitmap(7+icon_pos*30,22,bianjie,24,26);
														}
												else if (icon_fans == 2) //  ����
														{
																if(fans_state<3&&fans_state)
																{fans_state++;
																	set_speed(fans_state);
																	page2_speed(fans_state);}
														}
												else	//  ����
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
//	��ʱ���жϻص�����
int res=0;
int m=0;	
int mes=10;			//	tim7��ʼ��ʱʱ��(ms)
int dist=1;			//	ͣ��0or����1
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
		if(htim == &htim6)  //�ж��ж��Ƿ������ڶ�ʱ��6
   {
		 if(icon_pos==0)
		 {
       if(res<100)		//	��ʱ1��
			 {res++;}
			 else
			 {
				 res=0;
				 page1_erasetmp();	
				 page1_changetmp(&temperature, &humidity);	//	ˢ����ʪ��
			 }
		 }
   }
	 else if(htim == &htim7)  //�ж��ж��Ƿ������ڶ�ʱ��7
	 {
		 if(m<mes)
			m++;
		 else
		 {
			m=0;
			imus%=344;		//	ѭ������
			if(imus<70)		//	ǰ�ಿ��
			{
				mes=music_time[imus]*250;		//	������������ʱ��
				HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);	//	����pwm
				set_pwm(tone[music_tone[imus]],tone[music_tone[imus]]/2); 	//	��������Ƶ��
				imus++;		//	ָ����һ������	
			}
			else				//	 ��ʲ���
			{
				if(music_tone[imus]==21)		//	ֹͣ��
				{
					mes=music_time[imus]*60;
					HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_4);		//	�ر�pwm
					imus++;		//	ָ����һ������
				}
				else			//	����
				{
					if(dist==0)		//	�ϸ����ڲ��ŵ���ͣ��
					{
						dist=1;
						mes=music_time[imus]*60*4;
						HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);
						set_pwm(tone[music_tone[imus]],tone[music_tone[imus]]/2);
					}
					else					//	�ϸ����ڲ��ŵ�������
					{
						dist=0;
						mes=music_time[imus]*10;
						HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_4);		
						imus++;		//	ָ����һ������
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

