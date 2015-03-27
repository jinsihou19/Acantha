#pragma once
#include <stdint.h>
#include <HAL/Interface/Interfaces.h>
#include <HAL/STM32F4/F4Interfaces.h>
#include <BSP\devices\ILED.h>
#include <BSP\boards\dev_v1\LED.h>
using namespace BSP;
using namespace STM32F4;
class Manager
{
	//Manager construct:
	public :
		Manager(){};
		~Manager(){};
	
	//LED Manager:
	#define LED_NUM 5
	typedef struct{
		char name[8];//name 
		uint8_t num; //num
		LED *pLED;   //pointer
	}LED_table;
	private:
		LED_table led_table[LED_NUM];
		int led_num;
	public :
		virtual int  Register_LED(const char *name,LED *pLED);
		virtual int  get_LED_Num();
		virtual LED* getLED(const int num);
		virtual LED* getLED(const char *name);
	
	//Uart Manager:
	#define UART_NUM 4
	typedef struct{
		char name[8];
		uint8_t num;
		IUART *pUart; 
	}UART_table;
	private:
		UART_table uart_table[UART_NUM];
		int uart_num;
	public :
		virtual int  Register_UART(const char *name,IUART *pUart);
		virtual int  get_UART_Num();
		virtual IUART *getUART(const int num);
		virtual IUART *getUART(const char *name);
	
		
};
//Manager manager;