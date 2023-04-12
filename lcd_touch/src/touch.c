/**
  **************************************************************************
  * @file     touch.c
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
#include "touch.h"

/** @addtogroup AT32F437_periph_examples
  * @{
  */

/** @addtogroup 437_QSPI_lcd
  * @{
  */

uint16_t pos_x = 0x0;
uint16_t pos_y = 0x0;
uint8_t touch_points_num_use = 0;
uint8_t touch_flag = 0;
i2c_handle_type hi2cx;

/**
  * @brief  initializes peripherals used by the i2c.
  * @param  none
  * @retval none
  */
void i2c_lowlevel_init(i2c_handle_type* hi2c)
{
  gpio_init_type gpio_init_structure;

  if(hi2c->i2cx == I2Cx_PORT)
  {
    /* i2c periph clock enable */
    crm_periph_clock_enable(I2Cx_CLK, TRUE);
    crm_periph_clock_enable(I2Cx_SCL_GPIO_CLK, TRUE);
    crm_periph_clock_enable(I2Cx_SDA_GPIO_CLK, TRUE);

    /* gpio configuration */
    gpio_pin_mux_config(I2Cx_SCL_GPIO_PORT, I2Cx_SCL_GPIO_PinsSource, I2Cx_SCL_GPIO_MUX);

    gpio_pin_mux_config(I2Cx_SDA_GPIO_PORT, I2Cx_SDA_GPIO_PinsSource, I2Cx_SDA_GPIO_MUX);

    /* configure i2c pins: scl */
    gpio_init_structure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init_structure.gpio_mode           = GPIO_MODE_MUX;
    gpio_init_structure.gpio_out_type       = GPIO_OUTPUT_OPEN_DRAIN;
    gpio_init_structure.gpio_pull           = GPIO_PULL_UP;

    gpio_init_structure.gpio_pins           = I2Cx_SCL_GPIO_PIN;
    gpio_init(I2Cx_SCL_GPIO_PORT, &gpio_init_structure);

    /* configure i2c pins: sda */
    gpio_init_structure.gpio_pins           = I2Cx_SDA_GPIO_PIN;
    gpio_init(I2Cx_SDA_GPIO_PORT, &gpio_init_structure);

    /* configure and enable i2c interrupt */
    nvic_irq_enable(I2Cx_EVT_IRQn, 0, 0);
    nvic_irq_enable(I2Cx_ERR_IRQn, 0, 0);

    /* configure and enable i2c dma channel interrupt */
    nvic_irq_enable(I2Cx_DMA_TX_IRQn, 0, 0);
    nvic_irq_enable(I2Cx_DMA_RX_IRQn, 0, 0);

    /* i2c dma tx and rx channels configuration */
    /* enable the dma clock */
    crm_periph_clock_enable(I2Cx_DMA_CLK, TRUE);

    /* i2c dma channel configuration */
    dma_reset(hi2c->dma_tx_channel);
    dma_reset(hi2c->dma_rx_channel);

    hi2c->dma_tx_channel = I2Cx_DMA_TX_Channel;
    hi2c->dma_rx_channel = I2Cx_DMA_RX_Channel;

    hi2c->dma_init_struct.peripheral_base_addr    = (uint32_t)&hi2c->i2cx->txdt;
    hi2c->dma_init_struct.memory_base_addr        = 0;
    hi2c->dma_init_struct.direction               = DMA_DIR_MEMORY_TO_PERIPHERAL;
    hi2c->dma_init_struct.buffer_size             = 0xFFFF;
    hi2c->dma_init_struct.peripheral_inc_enable   = FALSE;
    hi2c->dma_init_struct.memory_inc_enable       = TRUE;
    hi2c->dma_init_struct.peripheral_data_width   = DMA_PERIPHERAL_DATA_WIDTH_BYTE;
    hi2c->dma_init_struct.memory_data_width       = DMA_MEMORY_DATA_WIDTH_BYTE;
    hi2c->dma_init_struct.loop_mode_enable        = FALSE;
    hi2c->dma_init_struct.priority                = DMA_PRIORITY_LOW;

    dma_init(hi2c->dma_tx_channel, &hi2c->dma_init_struct);
    dma_init(hi2c->dma_rx_channel, &hi2c->dma_init_struct);

    dmamux_init(I2Cx_DMA_TX_DMAMUX_Channel, I2Cx_DMA_TX_DMAREQ);
    dmamux_init(I2Cx_DMA_RX_DMAMUX_Channel, I2Cx_DMA_RX_DMAREQ);

    dmamux_enable(I2Cx_DMA, TRUE);

    /* config i2c */
    i2c_init(hi2c->i2cx, 0x0F, I2Cx_CLKCTRL);
    i2c_own_address1_set(hi2c->i2cx, I2C_ADDRESS_MODE_7BIT, TP_ADDR);
  }
}


/**
  * @brief  this function is init touch module(cst816d).
  * @param  none
  * @retval none
  */
void touch_init(void)
{
  gpio_init_type  gpio_init_struct = {0};
  exint_init_type exint_init_struct;
    
  /* crm config */
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_SCFG_PERIPH_CLOCK, TRUE);

  /* exint pin config */
  gpio_default_para_init(&gpio_init_struct);
  gpio_init_struct.gpio_pins = GPIO_PINS_11;
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOB, &gpio_init_struct);

  /* reset pin config */
  gpio_init_struct.gpio_pins = GPIO_PINS_8;
  gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_UP;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOB, &gpio_init_struct);

  /* i2c config */
  hi2cx.i2cx = I2Cx_PORT; 
  i2c_config(&hi2cx);
  
  /* reset lcd */
  AT_Delay(10);
  TRST_LOW;
  AT_Delay(100);
  TRST_HIGH;
  AT_Delay(120);
 
  /* exint config */
  /* config exint after reset lcd, to skip lcd power_on and reset interrupt */
  scfg_exint_line_config(SCFG_PORT_SOURCE_GPIOB, SCFG_PINS_SOURCE11);
  exint_default_para_init(&exint_init_struct);
  exint_init_struct.line_enable = TRUE;
  exint_init_struct.line_mode = EXINT_LINE_INTERRUPUT;
  exint_init_struct.line_select = EXINT_LINE_11;
  exint_init_struct.line_polarity = EXINT_TRIGGER_RISING_EDGE;
  exint_init(&exint_init_struct);
  
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  /* please make sure exint interrupt priority lower than i2c interrupt */
  nvic_irq_enable(EXINT15_10_IRQn, 1, 0);
  
  /* read lcd id */
  cst816d_chipId();

  /* set to point mode */
  cst816d_register_write_byte(MODE_ADDR, MODE_POINT);
  
  /* forbidden get into lowpower mode */
  cst816d_register_write_byte(LOWPOWER_ADDR, LOWPOWER_DIS);
}

/**
  * @brief  this function is used to write one byte data to lcd register.
  * @param  reg_addr : lcd register address.
  * @param  data     : the data received from lcd.
  * @param  len      : the data length needs to read.
  * @retval status
  */
uint8_t cst816d_read_len(uint8_t *reg_addr, uint8_t *data, uint8_t len)
{
  uint8_t res, ret;
  i2c_master_transmit_dma(&hi2cx, TP_ADDR, reg_addr, 1, I2C_TIMEOUT);
  ret = i2c_wait_end(&hi2cx, I2C_TIMEOUT);
  if(ret != I2C_OK)
    return ret;
  i2c_master_receive_dma(&hi2cx, TP_ADDR, data, len, I2C_TIMEOUT);
  res = i2c_wait_end(&hi2cx, I2C_TIMEOUT);
  return res;
}

/**
  * @brief  this function is used to write one byte data to lcd register.
  * @param  reg_addr : lcd register address.
  * @param  data     : the data needs to be written.
  * @retval status
  */
uint8_t cst816d_register_write_byte(uint8_t reg_addr, uint8_t data)
{
  uint8_t ret;
  uint8_t tx_buf[2];
  tx_buf[0] = reg_addr;
  tx_buf[1] = data;
  ret = i2c_master_transmit_dma(&hi2cx, TP_ADDR, tx_buf, 2, I2C_TIMEOUT);
  i2c_wait_end(&hi2cx, I2C_TIMEOUT);
  return ret;
}

/**
  * @brief  this function is read lcd id(for cst816d,the id is 0xb6).
  * @param  x/y : coordinate vaule.
  * @retval status
  */
uint8_t cst816d_chipId(void)
{
  uint8_t id=0;
  uint8_t res = 0;
  uint8_t reg_addr = 0xA7;
  res = cst816d_read_len(&reg_addr, &id,1);
  return res;
}

/**
  * @brief  this function is read lcd touch point number.
  * @param  touch_points_num : coordinate vaule.
  * @retval status
  */
uint8_t cst816d_get_touch_points_num(uint8_t *touch_points_num)
{
  uint8_t res = 0;
  uint8_t reg_addr = 0x02;
  res = cst816d_read_len(&reg_addr, touch_points_num, 1);
  return res;
}

/**
  * @brief  this function is read position data from touch.
  * @param  x/y : coordinate vaule.
  * @retval status
  */
uint8_t cst816d_read_pos(uint8_t *touch_points_num, uint16_t *x, uint16_t *y)
{
  uint8_t data[4];
  uint8_t reg_addr;
  cst816d_get_touch_points_num(touch_points_num);
  if (0 == *touch_points_num)
  {
    *x = 0;
    *y = 0;
    return 1;
  }
  else
  { 
    reg_addr = 0x03;
    cst816d_read_len(&reg_addr, data, 4);

    *x = ((data[0] & 0x0f) << 8) | data[1];
    *y = ((data[2] & 0x0f) << 8) | data[3];
  }

  return 1;
}

/**
  * @}
  */

/**
  * @}
  */
