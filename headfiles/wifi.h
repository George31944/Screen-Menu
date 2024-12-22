#ifndef __WIFI_H__
#define __WIFI_H__
#include <stdbool.h>
#include "usart.h"
#include "f103uart.h"
#include <string.h>
typedef unsigned  char uint8_t;

//	����ָ��:ע��"��\������\r\n
#define AT "AT\r\n"
#define ACK_OK "OK"
#define LIGHT_CON "LightSwitch"
#define AT_WIFI_MODE "AT+CWMODE=3\r\n"
#define AT_SET_PSW	"AT+CWJAP=\"kk\",\"12345678\"\r\n"
#define AT_SET_MQTT_CFG "AT+MQTTUSERCFG=0,1,\"NULL\",\"STM32F103&k1rwarbvKpO\",\"b900ed0ef6e9f630091d150e38b73f0d503bcfa5921f6c6cc9851b6499f02a67\",0,0,\"\"\r\n"
#define AT_SET_MQTT_CLENTID "AT+MQTTCLIENTID=0,\"k1rwarbvKpO.STM32F103|securemode=2\\,signmethod=hmacsha256\\,timestamp=1730208322946|\"\r\n"
#define AT_SET_MQTT_CONN "AT+MQTTCONN=0,\"iot-06z00dhzkn1fed7.mqtt.iothub.aliyuncs.com\",1883,1\r\n"

//	����ָ��
//	�ϱ��¶�
#define AT_PUB_TEM 	"AT+MQTTPUB=0,\"/sys/k1rwarbvKpO/STM32F103/thing/event/property/post\",\"{\\\"params\\\":{\\\"CurrentTemperature\\\":%0.2f}}\",1,0\r\n"
//	�ϱ�ʪ��
#define AT_PUB_HIM 	"AT+MQTTPUB=0,\"/sys/k1rwarbvKpO/STM32F103/thing/event/property/post\",\"{\\\"params\\\":{\\\"CurrentHumidity\\\":%0.2f}}\",1,0\r\n"
	

//	����ָ��
#define AT_MQTT_SUB "AT+MQTTSUB=0,\"/sys/k1rwarbvKpO/STM32F103/thing/service/property/set\",1\r\n"


#define BUFFER_SIZE 256			//	�����������
extern uint8_t dma_rxbuf[BUFFER_SIZE];		//	DMA����buffer
extern uint8_t tx_buf[BUFFER_SIZE];  			//����buffer
extern uint8_t rx_buf[BUFFER_SIZE];  		//	Ӧ�ô���buffer
extern bool rx_status;					//	���ձ�־λ

//��������
void send_cmd(uint8_t *cmd,uint8_t *ack);
void wifi_init(void);
void copy_to_rx_buf(uint8_t *ack,uint8_t len);
void UART_IDLEHandler(void);
#endif
