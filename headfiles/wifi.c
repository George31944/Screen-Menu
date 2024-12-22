#include "wifi.h"
uint8_t dma_rxbuf[BUFFER_SIZE];		//	DMA接收buffer
uint8_t rx_buf[BUFFER_SIZE];  		//	应用处理buffer
uint8_t tx_buf[BUFFER_SIZE];  		//发送buffer
bool rx_status = false;						//	接收标志位
extern DMA_HandleTypeDef hdma_uart4_rx;
//	发送命令并等待应答，若应答成功则printf：ack ok
void send_cmd(uint8_t *cmd,uint8_t *ack){
    int time_out = 500;
    memset(rx_buf,0,sizeof(rx_buf));
    printf("send ----> %s\n",cmd);
    HAL_UART_Transmit_DMA(&huart4 , (uint8_t *)cmd, strlen((const char *)cmd));
    while (time_out --)
    {
       if(rx_status){
         printf("read ----> %s\n",rx_buf);
        if(strstr((const char*)rx_buf,(const char*) ack) != NULL){		//	判断接收到的数据中有无“OK”
            printf("应答成功\n");				
            rx_status = false;
            return ;
        }
        memset(rx_buf,0,sizeof(rx_buf));
        rx_status = false;
      }
      HAL_Delay(1);
    }
    printf("应答失败\n");		
		return ;
}
//	wifi初始化
void wifi_init(){
		__HAL_UART_ENABLE_IT(&huart4,UART_IT_IDLE);
		HAL_UART_Receive_DMA(&huart4,dma_rxbuf,sizeof(dma_rxbuf));
    send_cmd((uint8_t *)AT,(uint8_t *)ACK_OK);
    HAL_Delay(1000);
    send_cmd((uint8_t *)AT_WIFI_MODE,(uint8_t *)ACK_OK);
    HAL_Delay(1000);
    send_cmd((uint8_t *)AT_SET_PSW,(uint8_t *)ACK_OK);
    HAL_Delay(1000);
    send_cmd((uint8_t *)AT_SET_MQTT_CFG,(uint8_t *)ACK_OK);
    HAL_Delay(1000);
    send_cmd((uint8_t *)AT_SET_MQTT_CLENTID,(uint8_t *)ACK_OK);
    HAL_Delay(1000);
    send_cmd((uint8_t *)AT_SET_MQTT_CONN,(uint8_t *)ACK_OK);
    HAL_Delay(1000);
		send_cmd((uint8_t *)AT_MQTT_SUB,(uint8_t *)ACK_OK);
    HAL_Delay(1000);
}
void copy_to_rx_buf(uint8_t *ack,uint8_t len){
    memcpy(rx_buf,ack,len);
    rx_status = true;
}
//	idle空闲中断函数
void UART_IDLEHandler(){
        if(__HAL_UART_GET_FLAG(&huart4, UART_FLAG_IDLE) == SET) //如果串口处于空闲状态
        {
                __HAL_UART_CLEAR_FLAG(&huart4, UART_FLAG_IDLE);//清空空闲状态标志
                HAL_UART_DMAStop(&huart4); //关闭DMA传输
                //计算接收到的数据长度 ，已接收长度=需要接收总长度-剩余待接收长度
                uint8_t rlen = BUFFER_SIZE - __HAL_DMA_GET_COUNTER(&hdma_uart4_rx);
                //进行数据处理     
                copy_to_rx_buf(dma_rxbuf,rlen);
								//	判断是否接收到相关字段
								if(strstr((const char*)rx_buf,(const char*)LIGHT_CON)!=NULL)
								{
									//	相关操作
									HAL_GPIO_TogglePin(GPIOA,led2_Pin);		//	开关灯
								}
                //重新打开DMA接收
                HAL_UART_Receive_DMA(&huart4,dma_rxbuf,sizeof(dma_rxbuf));                
        }
}
