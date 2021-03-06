#include <BSP/Resources.h>
#include <stdlib.h>
#include <string.h>
using namespace HAL;
using namespace STM32F4;
//Manager::LED part

Manager::Manager()
{
	accelerometer_count = 0;
	gyroscope_count = 0;
	gps_count = 0;
	rcin = NULL;
	rcout = NULL;
}
int  Manager::register_LED(const char *name,LED *pLED)
{
	strcpy(led_table[led_num].name,name);
	led_table[led_num].pLED=pLED;
	led_num++;
	return 0;
}
int  Manager::get_LED_Num()
{
	return led_num;
}
LED* Manager::getLED(const char *name)
{
	for(int i=0;i<LED_NUM;i++)
	{
			//check if valid,return pointer:
			if(0==strcmp(name,led_table[i].name))
			{
				return led_table[i].pLED;
			}
	}
	//check if not valid,return null pointer:
	return NULL;
}


//Manager::UART part
int Manager::register_UART(const char *name,IUART *pUart)
{
	strcpy(uart_table[uart_num].name,name);
	uart_table[uart_num].pUart=pUart;
	uart_num++;
	return 0;
}
int Manager::get_UART_Num()
{
	return uart_num;
}
IUART *Manager::getUART(const char *name)
{
	for(int i=0;i<UART_NUM;i++)
	{
			//check if valid,return pointer:
			if(0==strcmp(name,uart_table[i].name))
			{
				return uart_table[i].pUart;
			}
	}
	//check if not valid,return null pointer:
	return NULL;
}
	
//Manager::Timer part:
int Manager::register_Timer(const char *name,ITimer *pTimer)
{
	strcpy(timer_table[timer_num].name,name);
	timer_table[timer_num].num=timer_num;
	timer_table[timer_num].pTimer=pTimer;
	timer_num++;
	return 0;
}
int Manager::get_Timer_Num()
{
	return timer_num;
}
ITimer *Manager::getTimer(const char *name)
{
	for(int i=0;i<TIMER_NUM;i++)
	{
			//check if valid,return pointer:
			if(0==strcmp(name,timer_table[i].name))
			{
				return timer_table[i].pTimer;
			}
	}
	//check if not valid,return null pointer:
	return NULL;
}
int Manager::register_BatteryVoltage(const char *name,IBatteryVoltage *pIBatteryVoltage)
{
	strcpy(batteryvoltage_table[batteryvoltage_num].name,name);
	batteryvoltage_table[batteryvoltage_num].pIBatteryVoltage=pIBatteryVoltage;
	batteryvoltage_num++;
	return 0;
}
IBatteryVoltage *Manager::getBatteryVoltage(const char *name)
{
	for(int i=0;i<BATTERYVOLTAGE_NUM;i++)
	{
			//check if valid,return pointer:
			if(0==strcmp(name,batteryvoltage_table[i].name))
			{
				return batteryvoltage_table[i].pIBatteryVoltage;
			}
	}
	//check if not valid,return null pointer:
	return NULL;
}

int Manager::register_accelerometer(devices::IAccelerometer *accel)
{
	if (accelerometer_count >= MAX_ACCELEROMETER_COUNT)
		return -1;
	
	accelerometers[accelerometer_count++] = accel;
	return 0;
}
devices::IAccelerometer * Manager::get_accelerometer(int index)
{
	if (index < 0 || index >= accelerometer_count)
		return NULL;
	return accelerometers[index];
}
int Manager::get_accelerometer_count()
{
	return accelerometer_count;
}

int Manager::register_gyroscope(devices::IGyro *gyro)
{
	if (gyroscope_count >= MAX_ACCELEROMETER_COUNT)
		return -1;
	
	gyroscopes[gyroscope_count++] = gyro;
	return 0;
}
devices::IGyro * Manager::get_gyroscope(int index)
{
	if (index < 0 || index >= gyroscope_count)
		return NULL;
	return gyroscopes[index];
}
int Manager::get_gyroscope_count()
{
	return gyroscope_count;
}

int Manager::register_barometer(devices::IBarometer *baro)
{
	if (barometer_count >= MAX_ACCELEROMETER_COUNT)
		return -1;
	
	barometers[barometer_count++] = baro;
	return 0;
}
devices::IBarometer * Manager::get_barometer(int index)
{
	if (index < 0 || index >= barometer_count)
		return NULL;
	return barometers[index];
}
int Manager::get_barometer_count()
{
	return barometer_count;
}

int Manager::register_magnetometer(devices::IMagnetometer *mag)
{
	if (magnetometer_count >= MAX_ACCELEROMETER_COUNT)
		return -1;
	
	magnetometers[magnetometer_count++] = mag;
	return 0;
}
devices::IMagnetometer * Manager::get_magnetometer(int index)
{
	if (index < 0 || index >= magnetometer_count)
		return NULL;
	return magnetometers[index];
}
int Manager::get_magnetometer_count()
{
	return magnetometer_count;
}

int Manager::register_GPS(devices::IGPS *gps)
{
	if (gps_count >= MAX_GPS_COUNT)
		return -1;
	
	GPSs[gps_count++] = gps;
	return 0;
}
devices::IGPS * Manager::get_GPS(int index)
{
	if (index < 0 || index >= gps_count)
		return NULL;
	return GPSs[index];
}
int Manager::get_GPS_count()
{
	return gps_count;
}



int Manager::register_RCIN(IRCIN* rcin)
{
	this->rcin = rcin;
	return 0;
}

int Manager::register_RCOUT(IRCOUT* rcout)
{
	this->rcout = rcout;
	return 0;
}
IRCIN * Manager::get_RCIN()
{
	return rcin;
}

IRCOUT * Manager::get_RCOUT()
{
	return rcout;
}
// the only manager instance
Manager manager;