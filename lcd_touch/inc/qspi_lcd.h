/**
  **************************************************************************
  * @file     xmc_lcd.h
  * @brief    xmc_lcd header file
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */

#ifndef __XMC_LCD_H
#define __XMC_LCD_H

#include "sys.h"

/** @addtogroup 437_QSPI_lcd
  * @{
  */

/*used in command dma, do not delete it*/
extern uint32_t data_cnt;

/* the lcd reset line */
#define LCD_RESET_HIGH                   gpio_bits_set(GPIOF,GPIO_PINS_5)
#define LCD_RESET_LOW                    gpio_bits_reset(GPIOF,GPIO_PINS_5)

/* the lcd color */
#define RED                              0xF800	
#define GREEN                            0x07E0
#define BLUE                             0x001F
#define WHITE                            0xFFFF
#define BLACK                            0x0000
#define GRAY                             0xEF5D	     //0x2410
#define GRAY75                           0x39E7 
#define GRAY50                           0x7BEF	
#define GRAY25                           0xADB5	

#define ROW                              386
#define COL                              320

/*user can chose one of the two way to write data to lcd*/
//#define command_dma 
#define xip_dma

/** @defgroup QSPI_lcd_exported_functions
  * @{
  */

void qspi_config(void);
void lcd_init(void);
void lcd_wr_command(uint8_t command, uint32_t counter);
void lcd_wr_data(uint8_t data);
void lcd_setblock(uint16_t xstart,uint16_t ystart,uint16_t xend,uint16_t yend);
void lcd_drawpoint(uint16_t x,uint16_t y,uint16_t color);
void lcd_drawline(uint16_t xstart, uint16_t ystart, uint16_t xend, uint16_t yend, uint16_t color);
void lcd_clear(uint16_t color);
void lcd_display_number(void);
void lcd_wr_xip(uint8_t command, uint8_t* wbuf, uint32_t counter);



/**
  * @}
  */

/**
  * @}
  */

#endif
