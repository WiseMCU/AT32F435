#ifndef __GPIO_H
#define __GPIO_H

#include "sys.h"

typedef enum
{
  GPIO_PIN_RESET = 0U,
  GPIO_PIN_SET
} GPIO_PinState;

/* 引脚状态翻转 */
void AT_GPIO_TogglePin(gpio_type *GPIOx, uint16_t Pins);

/* 读引脚状态 */
GPIO_PinState AT_GPIO_ReadPin(gpio_type *GPIOx, uint16_t Pins);

/* 写引脚状态 */
void AT_GPIO_WritePin(gpio_type *GPIOx, uint16_t Pins, GPIO_PinState PinState);

/* 初始化引脚 */
void AT_GPIO_InitPin(gpio_type *GPIOx, gpio_init_type *GPIO_Init_Struct);

/*
 初始化实例配置PA0输出模式
void led_init(void)
{
	gpio_init_type GPIO_Init_Struct;										//定义初始结构体

	GPIO_Init_Struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;	//配置电流驱动能力
	GPIO_Init_Struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;				//输出方式
	GPIO_Init_Struct.gpio_mode = GPIO_MODE_OUTPUT;							//输出模式
	GPIO_Init_Struct.gpio_pins = GPIO_PINS_0;								//引脚序号
	GPIO_Init_Struct.gpio_pull = GPIO_PULL_NONE;							//无上下拉
	
	AT_GPIO_InitPin(GPIOA, &GPIO_Init_Struct);								//初始化
	
	AT_GPIO_WritePin(GPIOA , GPIO_PINS_0, GPIO_PIN_SET);					//设置初始电平
}
*/


#endif /* __GPIO_H */


