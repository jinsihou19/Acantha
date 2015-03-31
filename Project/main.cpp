#include <string.h>

#include <BSP/Resources.h>
#include <BSP/boards/dev_v1/init.h>
#include <stdio.h>
#include <Library/param.h>


using namespace STM32F4;
using namespace HAL;
using namespace devices;

uint8_t recv_buffer[5];
void delay()
{
	int64_t t = systimer->gettime();
	while(systimer->gettime() - t < 20)
		;
}

extern "C" int64_t getus()
{
	return systimer->gettime();
}

extern "C" void delayms(int ms)
{
	systimer->delayms(ms);
}

param test("test", 0);
int main(void)
{
	/*
	init_led();
	init_uart4();
	init_timer1();
	init_BatteryVoltage();
	manager.getLED("LED_RED")->off();
	manager.getLED("LED_GREEN")->off();
	while(1)
	{
		//manager.getLED("LED_RED")->off();
		//manager.getLED("LED_GREEN")->off();
		
		//manager.getUART("UART4")->write("12345\n",6);
	}
	*/

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	
	init_all_device();
		
	IAccelerometer * accel = manager.get_accelerometer(0);
	IGyro * gyro = manager.get_gyroscope(0);
	IBarometer * baro = manager.get_barometer(0);
	IMagnetometer * mag = manager.get_magnetometer(0);
	F4GPIO debug(GPIOA,GPIO_Pin_7);
	debug.set_mode(MODE_OUT_PushPull);
	
	/*
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);

	F4GPIO cs(GPIOE,GPIO_Pin_7);
	F4GPIO cs_mpu6000(GPIOE,GPIO_Pin_8);
	F4GPIO cs_5983(GPIOE,GPIO_Pin_10);
	cs.set_mode(MODE_OUT_PushPull);
	cs_mpu6000.set_mode(MODE_OUT_PushPull);
	cs_5983.set_mode(MODE_OUT_PushPull);
	cs.write(true);
	cs_mpu6000.write(true);
	cs_5983.write(true);
	
	F4SPI spi2(SPI2);
	//MS5611_SPI baro(&spi2, &cs);
	//baro.init();
	
	MPU6000 mpu6000;
	mpu6000.init(&spi2, &cs_mpu6000);
	
	*/
	
	//HMC5983 hmc5983;
	//hmc5983.init(&spi2, &cs_5983);
	
	//Test Timer:
	//init_MPU9250spi();
	while(1)
	{
		short data[7] = {0};
		
		accelerometer_data adata;
		gyro_data gdata;
		baro_data bdata;
		mag_data mdata;
		int res = accel->read(&adata);
		res = gyro->read(&gdata);
		res = baro->read(&bdata);
		res = mag->read(&mdata);
		
		float scale = 180/3.1415926f;
		//printf("\r%d, %f,%f,%f,%f, angular rate: %f,%f,%f deg/s", res, adata.x, adata.y, adata.z, adata.temperature, gdata.x * scale, gdata.y * scale, gdata.z * scale);
		
		//printf("%f, %f", bdata.pressure, bdata.temperature);
		//systimer->delayms(10);
		debug.toggle();
	}
	
	/*
	
	while(0)
	{		
		int16_t rcs[8];
		rc.get_channel_data(rcs, 0, 8);

		rcout.write(rcs, 0, 6);
		systimer->delayms(10);
		
		//printf("\r%d,%d,%d,%d,%d,%d", rcs[0], rcs[1], rcs[2], rcs[3], rcs[4], rcs[5]);
	}
	
	
	while(1)
	{
		int data[2];
		int res = baro.read(data);
		//int res = read_MS5611spi(data);
		printf("\r%d, %d, %d", res, data[0], data[1]);
		
 		res = res;
	}
	*/
}


struct __FILE { int handle; /* Add whatever you need here */ };
#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))
#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000

extern "C" int fputc(int ch, FILE *f)
{
	if (DEMCR & TRCENA) 
	{
		while (ITM_Port32(0) == 0);
		ITM_Port8(0) = ch;
	}
	return (ch);
}

