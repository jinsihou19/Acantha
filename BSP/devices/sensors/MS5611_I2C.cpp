#include "MS5611_I2C.h"

// registers of the device
#define MS561101BA_ADDR_CSB_LOW   0xEC
#define MS5611Address (0x77<<1)
#define MS561101BA_D1 0x40
#define MS561101BA_D2 0x50
#define MS561101BA_RESET 0x1E

// OSR (Over Sampling Ratio) constants
#define MS561101BA_OSR_256 0x00
#define MS561101BA_OSR_512 0x02
#define MS561101BA_OSR_1024 0x04
#define MS561101BA_OSR_2048 0x06
#define MS561101BA_OSR_4096 0x08

#define MS561101BA_PROM_BASE_ADDR 0xA2	// by adding ints from 0 to 6 we can read all the prom configuration values. 
										// C1 will be at 0xA2 and all the subsequent are multiples of 2
#define MS561101BA_PROM_REG_COUNT 6 // number of registers in the PROM
#define MS561101BA_PROM_REG_SIZE 2 // size in bytes of a prom registry.
#define EXTRA_PRECISION 5 // trick to add more precision to the pressure and temp readings

#define SAMPLEING_TIME 10000 // 10000us, 10ms

namespace sensors
{

MS5611_I2C::MS5611_I2C(I2C *i2c)
{
	this->i2c = i2c;
}

MS5611_I2C::~MS5611_I2C()
{
}

int MS5611_I2C::init(void)
{
	uint8_t tmp[3];
	int i;
	
	i2c->write_reg(MS5611Address, MS561101BA_RESET, 0x00);	
	systimer->delayms(10);
	for(i=0; i<6; i++)
	{
		i2c->read_regs(MS5611Address, MS561101BA_PROM_BASE_ADDR+i*2, tmp, 2);
		refdata[i] = (tmp[0] << 8) + tmp[1];
	}
	
	if (i2c->read_regs(MS5611Address, MS561101BA_PROM_BASE_ADDR+12, tmp, 2) < 0)
		return -1;
	crc = (tmp[0] << 8) + tmp[1];
	
	// Temperature
	i2c->write_reg(MS5611Address, MS561101BA_D2 + OSR, 0x00);
	systimer->delayms(10);
	i2c->read_regs(MS5611Address, 0x00, tmp, 3);
	
	rawTemperature = ((int)tmp[0] << 16) + ((int)tmp[1] << 8) + (int)tmp[2];
	DeltaTemp = rawTemperature - (((int32_t)refdata[4]) << 8);
	temperature = ((1<<EXTRA_PRECISION)*2000l + ((DeltaTemp * refdata[5]) >> (23-EXTRA_PRECISION))) / ((1<<EXTRA_PRECISION));
		
	// Pressure
	i2c->write_reg(MS5611Address, MS561101BA_D1 + OSR, 0x00);
	systimer->delayms(10);
	i2c->read_regs(MS5611Address, 0x00, tmp, 3);
	
	rawPressure = ((int)tmp[0] << 16) + ((int)tmp[1] << 8) + (int)tmp[2];
	off  = (((int64_t)refdata[1]) << 16) + ((refdata[3] * DeltaTemp) >> 7);
	sens = (((int64_t)refdata[0]) << 15) + ((refdata[2] * DeltaTemp) >> 8);
	pressure = ((((rawPressure * sens) >> 21) - off) >> (15-EXTRA_PRECISION)) / ((1<<EXTRA_PRECISION));
	
	return 0;
}

int MS5611_I2C::read(int *data)
{
	int rtn = 1;
	uint8_t tmp[3];

	do
	{
		if (new_temperature == 0 && last_temperature_time == 0 && last_pressure_time == 0)
		{
			if (i2c->write_reg(MS5611Address, MS561101BA_D2 + OSR, 0x00) < 0)
			{
				rtn = -1;
				break;
			}
			last_temperature_time = systimer->gettime();
		}
		
		if (systimer->gettime() - last_temperature_time >  SAMPLEING_TIME && new_temperature == 0)
		{
			if (i2c->read_regs(MS5611Address, 0x00, tmp, 3) < 0)
			{
				rtn = -1;
				break;
			}
		
			rawTemperature = ((int)tmp[0] << 16) + ((int)tmp[1] << 8) + (int)tmp[2];
			DeltaTemp = rawTemperature - (((int32_t)refdata[4]) << 8);
			new_temperature = ((1<<EXTRA_PRECISION)*2000l + ((DeltaTemp * refdata[5]) >> (23-EXTRA_PRECISION))) / ((1<<EXTRA_PRECISION));
			
			if (i2c->write_reg(MS5611Address, MS561101BA_D1 + OSR, 0x00) < 0)
			{
				rtn = -1;
				break;
			}

			last_pressure_time = systimer->gettime();
		}

		if (systimer->gettime() - last_pressure_time >  SAMPLEING_TIME && last_pressure_time > 0)
		{
			if (i2c->read_regs(MS5611Address, 0x00, tmp, 3) <0)
			{
				rtn = -1;
				break;
			}
			
			rawPressure = ((int)tmp[0] << 16) + ((int)tmp[1] << 8) + (int)tmp[2];
			off  = (((int64_t)refdata[1]) << 16) + ((refdata[3] * DeltaTemp) >> 7);
			sens = (((int64_t)refdata[0]) << 15) + ((refdata[2] * DeltaTemp) >> 8);
			pressure = ((((rawPressure * sens) >> 21) - off) >> (15-EXTRA_PRECISION)) / ((1<<EXTRA_PRECISION));
			temperature = new_temperature;

			new_temperature = 0;
			last_temperature_time = 0;
			last_pressure_time = 0;
			rtn = 0;
		}
	} while (0);
	
	data[0] = pressure;
	data[1] = temperature;
	return rtn;
}

}