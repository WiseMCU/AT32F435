/**
  **************************************************************************
  * @file     touch.h
  * @brief    this file contains all the functions prototypes for the
  *           touch firmware driver.
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


#ifndef __TOUCH_H
#define __TOUCH_H

#include "sys.h"

/** @addtogroup AT32F437_periph_examples
  * @{
  */

/** @addtogroup 437_QSPI_lcd
  * @{
  */

/** @defgroup TOUCH_device_type
  * @{
  */

#define TRST_HIGH                        gpio_bits_set(GPIOB,GPIO_PINS_8)
#define TRST_LOW                         gpio_bits_reset(GPIOB,GPIO_PINS_8)


/* I2C address */
#define TP_ADDR       0x2A

/* lcd command and address */
#define MODE_ADDR     0xFA
#define MODE_POINT    0x60
#define MODE_GESTURE  0x11

#define LOWPOWER_ADDR 0xFE
#define LOWPOWER_EN   0x00
#define LOWPOWER_DIS  0x01



/* Touch Interface I2C */
#define I2C_TIMEOUT                      0xFFFFFFF

#define I2Cx_CLKCTRL                     0x30F01C32   //400K

#define I2Cx_PORT                        I2C1
#define I2Cx_CLK                         CRM_I2C1_PERIPH_CLOCK
#define I2Cx_DMA                         DMA1
#define I2Cx_DMA_CLK                     CRM_DMA1_PERIPH_CLOCK

#define I2Cx_SCL_GPIO_CLK                CRM_GPIOB_PERIPH_CLOCK
#define I2Cx_SCL_GPIO_PIN                GPIO_PINS_6
#define I2Cx_SCL_GPIO_PinsSource         GPIO_PINS_SOURCE6
#define I2Cx_SCL_GPIO_PORT               GPIOB
#define I2Cx_SCL_GPIO_MUX                GPIO_MUX_4

#define I2Cx_SDA_GPIO_CLK                CRM_GPIOB_PERIPH_CLOCK
#define I2Cx_SDA_GPIO_PIN                GPIO_PINS_7
#define I2Cx_SDA_GPIO_PinsSource         GPIO_PINS_SOURCE7
#define I2Cx_SDA_GPIO_PORT               GPIOB
#define I2Cx_SDA_GPIO_MUX                GPIO_MUX_4

#define I2Cx_DMA_TX_Channel              DMA1_CHANNEL1
#define I2Cx_DMA_TX_DMAMUX_Channel       DMA1MUX_CHANNEL1
#define I2Cx_DMA_TX_DMAREQ               DMAMUX_DMAREQ_ID_I2C1_TX
#define I2Cx_DMA_TX_IRQn                 DMA1_Channel1_IRQn

#define I2Cx_DMA_RX_Channel              DMA1_CHANNEL2
#define I2Cx_DMA_RX_DMAMUX_Channel       DMA1MUX_CHANNEL2
#define I2Cx_DMA_RX_DMAREQ               DMAMUX_DMAREQ_ID_I2C1_RX
#define I2Cx_DMA_RX_IRQn                 DMA1_Channel2_IRQn

#define I2Cx_EVT_IRQn                    I2C1_EVT_IRQn
#define I2Cx_ERR_IRQn                    I2C1_ERR_IRQn


  
extern uint16_t pos_x;
extern uint16_t pos_y;
extern uint8_t touch_points_num_use;
extern uint8_t touch_flag;


void touch_init(void);
uint8_t cst816d_init(void);
uint8_t cst816d_get_touch_points_num(uint8_t *touch_points_num);
uint8_t cst816d_read_pos(uint8_t *touch_points_num, uint16_t *x, uint16_t *y);
uint8_t cst816d_chipId(void);
uint8_t cst816d_register_write_byte(uint8_t reg_addr, uint8_t data);
uint8_t cst816d_read_len(uint8_t *reg_addr, uint8_t *data, uint8_t len);



/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
#endif





