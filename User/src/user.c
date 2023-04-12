#include "user.h"


void LED_Init(void)
{
	gpio_init_type GPIO_Init_Struct;										//定义初始结构体

	GPIO_Init_Struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;	//配置电流驱动能力
	GPIO_Init_Struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;				//输出方式
	GPIO_Init_Struct.gpio_mode = GPIO_MODE_OUTPUT;							//输出模式
	GPIO_Init_Struct.gpio_pins = GPIO_PINS_0;								//引脚序号
	GPIO_Init_Struct.gpio_pull = GPIO_PULL_NONE;							//无上下拉
	
	AT_GPIO_InitPin(GPIOC, &GPIO_Init_Struct);								//初始化
	
	AT_GPIO_WritePin(GPIOC , GPIO_PINS_0, GPIO_PIN_SET);					//设置初始电平
}