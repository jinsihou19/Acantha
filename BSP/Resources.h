#pragma once
#include <stdint.h>
#include <HAL/Interface/Interfaces.h>
#include <HAL/STM32F4/F4Interfaces.h>
#include <BSP/devices/ILED.h>
#include <BSP/devices/IGyro.h>
#include <BSP/boards/dev_v1/LED.h>
#include <BSP/boards/dev_v1/BatteryVoltage.h>
#include <BSP/devices/IAccelerometer.h>
#include <BSP/devices/IMagnetometer.h>
#include <BSP/devices/IBarometer.h>
#include <BSP/devices/IMagnetometer.h>
#include <BSP/devices/IGPS.h>

using namespace BSP;
using namespace STM32F4;
#define MAX_ACCELEROMETER_COUNT 5
#define MAX_GPS_COUNT 3

class Manager
{
	//Manager construct:
	public :
		Manager();
		~Manager(){};
		#define LED_NUM 5
		#define UART_NUM 4
		#define TIMER_NUM 4
		#define BATTERYVOLTAGE_NUM 2
			
			
		typedef struct{
			char name[10];//name 
			LED *pLED;   //pointer
		}LED_table;
		typedef struct{
			char name[10];
			IUART *pUart; 
		}UART_table;
		typedef struct{
			char name[10];
			uint8_t num;
			ITimer *pTimer; 
		}Timer_table;
		typedef struct{
			char name[18];
			uint8_t num;
			IBatteryVoltage *pIBatteryVoltage; 
		}BatteryVoltage_table;
	
	
	private:
		IRCIN *rcin;
		IRCOUT *rcout;
		LED_table led_table[LED_NUM];
		int led_num;
		UART_table uart_table[UART_NUM];
		int uart_num;
		Timer_table timer_table[TIMER_NUM];
		int timer_num;
		BatteryVoltage_table batteryvoltage_table[BATTERYVOLTAGE_NUM];
		int batteryvoltage_num;
		int accelerometer_count;
		devices::IAccelerometer * accelerometers[MAX_ACCELEROMETER_COUNT];
		int gyroscope_count;
		devices::IGyro * gyroscopes[MAX_ACCELEROMETER_COUNT];	
		int barometer_count;
		devices::IBarometer * barometers[MAX_ACCELEROMETER_COUNT];	
		int magnetometer_count;
		devices::IMagnetometer * magnetometers[MAX_ACCELEROMETER_COUNT];
		int gps_count;
		devices::IGPS * GPSs[MAX_GPS_COUNT];
	
	public :
		int get_LED_Num();
		int get_UART_Num();
		int get_Timer_Num();
		int get_gyroscope_count();
		int get_accelerometer_count();
		int get_magnetometer_count();
		int get_barometer_count();
		int get_GPS_count();	
		
		//register function:
		int register_LED(const char *name,LED *pLED);
		int register_gyroscope(devices::IGyro *gyro);
		int register_accelerometer(devices::IAccelerometer *accel);
		int register_barometer(devices::IBarometer *baro);
		int register_magnetometer(devices::IMagnetometer *mag);
		int register_GPS(devices::IGPS *gps);
		int register_UART(const char *name,IUART *pUart);
		int register_Timer(const char *name,ITimer *pTimer);
		int register_BatteryVoltage(const char *name,IBatteryVoltage *pIBatteryVoltage);
		int register_RCIN(IRCIN *rcin);
		int register_RCOUT(IRCOUT *rcout);

		//getDevice function:
		LED* getLED(const char *name);
		IUART *getUART(const char *name);
		ITimer *getTimer(const char *name);
		IBatteryVoltage *getBatteryVoltage(const char *name);
		devices::IAccelerometer * get_accelerometer(int index);
		devices::IGyro * get_gyroscope(int index);
		devices::IMagnetometer * get_magnetometer(int index);
		devices::IBarometer * get_barometer(int index);
		devices::IGPS * get_GPS(int index);
		IRCIN * get_RCIN();
		IRCOUT * get_RCOUT();
};

//Declear manager as global:
extern Manager manager;

// board bsp implements and starts from this function
extern int bsp_init_all();

// board bsp implements this
extern void reset_system();