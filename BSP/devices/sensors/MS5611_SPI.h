#pragma once

#include <stdint.h>
#include <HAL/Interface/Interfaces.h>

using namespace HAL;
namespace sensors
{
	class MS5611_SPI
	{
	public:
		MS5611_SPI(SPI *SPI, GPIO *CS);
		~MS5611_SPI();
		
		int init(void);
		int read(int *data);

	protected:
		SPI *spi;
		GPIO *CS;

		int read_regs(uint8_t start_reg, void *out, int count);
		int write_reg(uint8_t reg, uint8_t data);

		uint8_t OSR;// = MS561101BA_OSR_4096;
		int temperature;// = 0;
		int pressure;// = 0;
		int new_temperature;// = 0;
		int64_t last_temperature_time;// = 0;
		int64_t last_pressure_time;// = 0;
		int64_t rawTemperature;// = 0;
		int64_t rawPressure;// = 0;
		int64_t DeltaTemp;// = 0;
		int64_t off;//  = (((int64_t)_C[1]) << 16) + ((_C[3] * dT) >> 7);
		int64_t sens;// = (((int64_t)_C[0]) << 15) + ((_C[2] * dT) >> 8);
		uint16_t refdata[6];
		uint16_t crc;
	};
}