#include "ADC.h"
#include "stm32F4xx_gpio.h"
#include "stm32F4xx_adc.h"
using namespace HAL;
namespace STM32F4
{
	class F4ADC:public AnologInput
	{
	private:
		ADC_TypeDef* ADCx;
		uint8_t ADC_Channel;
	public:
		F4ADC(ADC_TypeDef* ADCx,uint8_t ADC_Channe);
		~F4ADC(){};
		virtual int read();
	};
}