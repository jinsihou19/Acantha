#include "F4Storage.h"
#include "stm32f4xx_flash.h"
#include <stdint.h>
#include "string.h"
using namespace HAL;
namespace STM32F4
{
	F4Storage::F4Storage(uint32_t page__size,uint32_t buffer_size,uint32_t start_address)
	{
		this->page__size=page__size;
		this->buffer_size=buffer_size;
		this->start_address=start_address;
	}
	int F4Storage::init()
	{
		FLASH_Unlock();
		return 0;
	}
	int F4Storage::min(int a, int b)
	{
		if (a>b)
			return b;
		return a;
	}
	int F4Storage::erase(int address)
	{
		FLASH_Status res = FLASH_EraseSector(address >= 0x080C0000 + page__size ? FLASH_Sector_11 : FLASH_Sector_10, VoltageRange_3);
		return res;
	}
	int F4Storage::total_size()
	{
		return  buffer_size;
	}
	int F4Storage::page_size()
	{
		return  page__size;
	}
	int F4Storage::write(int address, const void *data, int size)
	{
		int count = min(size, buffer_size - address);
		char *p = (char*)data;

		for(int i=0; i<count/4*4; i+=4)
		FLASH_ProgramWord(start_address+address+i, *(uint32_t*)(p+i));

		if (count %4)
		{
			uint32_t pending = 0xffffffff;
			memcpy(&pending, p+count/4*4, count%4);
			FLASH_ProgramWord(start_address+address+count/4*4, pending);
		}

		FLASH_WaitForLastOperation();
		return count;
	}
	int F4Storage::read(int address, void *data, int maxsize)
	{
		int count = min(maxsize, buffer_size - address);
		memcpy(data, (char*)start_address+address, count);
		return count;
	}
}

HAL::IStorage *get_default_storage()
{
	static STM32F4::F4Storage theDefaultStorage;
	return &theDefaultStorage;
}
