#include "wifi.h"
uint8_t dma_rxbuf[BUFFER_SIZE];		//	DMA����buffer
uint8_t rx_buf[BUFFER_SIZE];  		//	Ӧ�ô���buffer
uint8_t tx_buf[BUFFER_SIZE];  		//����buffer
bool rx_status = false;						//	���ձ�־λ
extern DMA_HandleTypeDef hdma_uart4_rx;
//	��������ȴ�Ӧ����Ӧ��ɹ���printf��ack ok
void send_cmd(uint8_t *cmd,uint8_t *ack){
    int time_out = 500;
    memset(rx_buf,0,sizeof(rx_buf));
    printf("send ----> %s\n",cmd);
    HAL_UART_Transmit_DMA(&huart4 , (uint8_t *)cmd, strlen((const char *)cmd));
    while (time_out --)
    {
       if(rx_status){
         printf("read ----> %s\n",rx_buf);
        if(strstr((const char*)rx_buf,(const char*) ack) != NULL){		//	�жϽ��յ������������ޡ�OK��
            printf("Ӧ��ɹ�\n");				
            rx_status = false;
            return ;
        }
        memset(rx_buf,0,sizeof(rx_buf));
        rx_status = false;
      }
      HAL_Delay(1);
    }
    printf("Ӧ��ʧ��\n");		
		return ;
}
//	wifi��ʼ��
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
//	idle�����жϺ���
void UART_IDLEHandler(){
        if(__HAL_UART_GET_FLAG(&huart4, UART_FLAG_IDLE) == SET) //������ڴ��ڿ���״̬
        {
                __HAL_UART_CLEAR_FLAG(&huart4, UART_FLAG_IDLE);//��տ���״̬��־
                HAL_UART_DMAStop(&huart4); //�ر�DMA����
                //������յ������ݳ��� ���ѽ��ճ���=��Ҫ�����ܳ���-ʣ������ճ���
                uint8_t rlen = BUFFER_SIZE - __HAL_DMA_GET_COUNTER(&hdma_uart4_rx);
                //�������ݴ���     
                copy_to_rx_buf(dma_rxbuf,rlen);
								//	�ж��Ƿ���յ�����ֶ�
								if(strstr((const char*)rx_buf,(const char*)LIGHT_CON)!=NULL)
								{
									//	��ز���
									HAL_GPIO_TogglePin(GPIOA,led2_Pin);		//	���ص�
								}
                //���´�DMA����
                HAL_UART_Receive_DMA(&huart4,dma_rxbuf,sizeof(dma_rxbuf));                
        }
}
