#include "gpio.h"

/* 引脚状态翻转 */
void AT_GPIO_TogglePin(gpio_type *GPIOx, uint16_t Pins)
{
	GPIOx->odt ^= Pins;
}

/* 读引脚状态 */
GPIO_PinState AT_GPIO_ReadPin(gpio_type *GPIOx, uint16_t Pins)
{
	if(Pins != (Pins & GPIOx->idt))
	{
		return GPIO_PIN_RESET;
	}
	
	return GPIO_PIN_SET;
}

/* 写引脚状态 */
void AT_GPIO_WritePin(gpio_type *GPIOx, uint16_t Pins, GPIO_PinState PinState)
{
	switch(PinState)
	{
		case GPIO_PIN_RESET:
			GPIOx->clr = Pins;
			break;
		
		case GPIO_PIN_SET:
			GPIOx->scr = Pins;
			break;
	}
}

/* 初始化引脚 */
void AT_GPIO_InitPin(gpio_type *GPIOx, gpio_init_type *GPIO_Init_Struct)
{
	crm_periph_clock_enable(MAKE_VALUE(0x30, (GPIOx - GPIOA) / 16), TRUE);
	gpio_init(GPIOx, GPIO_Init_Struct);
}







