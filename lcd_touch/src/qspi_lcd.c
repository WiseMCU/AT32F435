/**
  **************************************************************************
  * @file     xmc_lcd.c
  * @brief    xmc_lcd config program
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

#include <string.h>
#include "qspi_lcd.h"
#include "touch.h"

/** @addtogroup AT32F437_periph_examples
  * @{
  */

/** @addtogroup 437_QSPI_lcd
  * @{
  */
extern const uint8_t gimage_3[];
extern const uint8_t gimage_8[];
extern const uint8_t gimage_5[];
extern const uint8_t gimage_0[];
 
uint32_t data_cnt;
qspi_xip_type lcd_xip_config;
qspi_cmd_type lcd_cmd_config;
uint8_t *qspi1_mem_addr;

/**
  * @brief  xip init lcd config
  * @param  qspi_xip_struct: the pointer for qspi_xip_type parameter
  * @retval none
  */
void xip_init_lcd_config(qspi_xip_type *qspi_xip_struct)
{
  qspi_xip_struct->read_instruction_code = 0x00;
  qspi_xip_struct->read_address_length = QSPI_XIP_ADDRLEN_3_BYTE;
  qspi_xip_struct->read_operation_mode = QSPI_OPERATE_MODE_144;
  qspi_xip_struct->read_second_dummy_cycle_num = 0;
  qspi_xip_struct->write_instruction_code = 0x12;
  qspi_xip_struct->write_address_length = QSPI_XIP_ADDRLEN_3_BYTE;
  qspi_xip_struct->write_operation_mode = QSPI_OPERATE_MODE_144;
  qspi_xip_struct->write_second_dummy_cycle_num = 0;
  qspi_xip_struct->write_select_mode = QSPI_XIPW_SEL_MODET;
  qspi_xip_struct->write_time_counter = 0x01;
  qspi_xip_struct->write_data_counter = 0x1F;
  qspi_xip_struct->read_select_mode = QSPI_XIPR_SEL_MODET;
  qspi_xip_struct->read_time_counter = 0x01;
  qspi_xip_struct->read_data_counter = 0x1F;
}

/**
  * @brief  lcd cmd write config
  * @param  qspi_cmd_struct: the pointer for qspi_cmd_type parameter
  * @param  addr: write start address
  * @param  counter: write data counter
  * @retval none
  */
void lcd_cmd_write_config(qspi_cmd_type *qspi_cmd_struct, uint32_t addr, uint32_t counter)
{
  qspi_cmd_struct->pe_mode_enable = FALSE;
  qspi_cmd_struct->pe_mode_operate_code = 0;
  qspi_cmd_struct->instruction_code = 0x12;
  qspi_cmd_struct->instruction_length = QSPI_CMD_INSLEN_1_BYTE;
  qspi_cmd_struct->address_code = addr;
  qspi_cmd_struct->address_length = QSPI_CMD_ADRLEN_3_BYTE;
  qspi_cmd_struct->data_counter = counter;
  qspi_cmd_struct->second_dummy_cycle_num = 0;
  qspi_cmd_struct->operation_mode = QSPI_OPERATE_MODE_144;
  qspi_cmd_struct->read_status_config = QSPI_RSTSC_SW_ONCE;
  qspi_cmd_struct->read_status_enable = FALSE;
  qspi_cmd_struct->write_data_enable = TRUE;
}

/**
  * @brief  qspi config
  * @param  none
  * @retval none
  */
void qspi_config(void)
{
  gpio_init_type gpio_init_struct;
  /* enable the dma clock */
  crm_periph_clock_enable(CRM_DMA2_PERIPH_CLOCK, TRUE);

  /* enable the qspi clock */
  crm_periph_clock_enable(CRM_QSPI1_PERIPH_CLOCK, TRUE);

  /* enable the pin clock */
  crm_periph_clock_enable(CRM_GPIOF_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOG_PERIPH_CLOCK, TRUE);

  /* set default parameter */
  gpio_default_para_init(&gpio_init_struct);

  /* configure the io0 gpio */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_8;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOF, &gpio_init_struct);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE8, GPIO_MUX_10);

  /* configure the io1 gpio */
  gpio_init_struct.gpio_pins = GPIO_PINS_9;
  gpio_init(GPIOF, &gpio_init_struct);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE9, GPIO_MUX_10);

  /* configure the io2 gpio */
  gpio_init_struct.gpio_pins = GPIO_PINS_7;
  gpio_init(GPIOF, &gpio_init_struct);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE7, GPIO_MUX_9);

  /* configure the io3 gpio */
  gpio_init_struct.gpio_pins = GPIO_PINS_6;
  gpio_init(GPIOF, &gpio_init_struct);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE6, GPIO_MUX_9);

  /* configure the sck gpio */
  gpio_init_struct.gpio_pins = GPIO_PINS_10;
  gpio_init(GPIOF, &gpio_init_struct);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE10, GPIO_MUX_9);

  /* configure the cs gpio */
  gpio_init_struct.gpio_pins = GPIO_PINS_6;
  gpio_init(GPIOG, &gpio_init_struct);
  gpio_pin_mux_config(GPIOG, GPIO_PINS_SOURCE6, GPIO_MUX_10);
  
  /* configure the reset gpio */
  gpio_init_struct.gpio_pins = GPIO_PINS_5;
  gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
  gpio_init_struct.gpio_pull = GPIO_PULL_UP;
  gpio_init(GPIOF, &gpio_init_struct);
  
  /* switch to cmd port */
  qspi_xip_enable(QSPI1, FALSE);

  /* set sclk */
  qspi_clk_division_set(QSPI1, QSPI_CLK_DIV_8);
  
  /* set sck idle mode 0 */
  qspi_sck_mode_set(QSPI1, QSPI_SCK_MODE_0);
  
  /* initial xip */
  xip_init_lcd_config(&lcd_xip_config);
  qspi_xip_init(QSPI1, &lcd_xip_config);
  qspi_xip_cache_bypass_set(QSPI1,TRUE);

}

/**
  * @brief  qspi command dma set
  * @param  command: the command used to control lcd
  * @param  buf: the pointer for dma data
  * @param  length: data length
  * @param  inc_en: enable dma memory address increase or not
  * @retval none
  */
void qspi_cmd_dma_set(uint8_t command, uint8_t* buf, uint32_t length, confirm_state inc_en)
{
  dma_init_type dma_init_struct;
  dma_reset(DMA2_CHANNEL1);
  dma_default_para_init(&dma_init_struct);
 
  /* dma stream1 configuration */
  dma_init_struct.direction = DMA_DIR_MEMORY_TO_PERIPHERAL;
  if((length % 4) == 0)
  {
    dma_init_struct.buffer_size = (uint16_t)(length/4);
    dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_WORD;
    dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_WORD;
  }
  else if((length % 2) == 0)
  {
    dma_init_struct.buffer_size = (uint16_t)(length/2);
    dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
    dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_HALFWORD;
  }
  else
  {
    dma_init_struct.buffer_size = (uint16_t)length;
    dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_BYTE;
    dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_BYTE;
  }
  dma_init_struct.peripheral_base_addr = (uint32_t)(&(QSPI1->dt));
  dma_init_struct.peripheral_inc_enable = FALSE;
  dma_init_struct.memory_base_addr = (uint32_t)buf;
  dma_init_struct.memory_inc_enable = inc_en;
  dma_init_struct.priority = DMA_PRIORITY_HIGH;
  dma_init_struct.loop_mode_enable = FALSE;
  dma_init(DMA2_CHANNEL1, &dma_init_struct);
  
  dmamux_enable(DMA2, TRUE);
  dmamux_init(DMA2MUX_CHANNEL1, DMAMUX_DMAREQ_ID_QSPI1);

  dma_channel_enable(DMA2_CHANNEL1, TRUE);
  
  /* kick command */
  lcd_cmd_write_config(&lcd_cmd_config, (uint32_t)(command<<8), length);
  qspi_cmd_operation_kick(QSPI1, &lcd_cmd_config);

  /* wait command completed */
  while(qspi_flag_get(QSPI1, QSPI_CMDSTS_FLAG) == RESET);
  qspi_flag_clear(QSPI1, QSPI_CMDSTS_FLAG);

  /* wait dma completed */
  while(dma_flag_get(DMA2_FDT1_FLAG) == RESET);
  dma_flag_clear(DMA2_FDT1_FLAG);
  dma_channel_enable(DMA2_CHANNEL1, FALSE);
}



/**
  * @brief  qspi xip dma set
  * @param  command: the command used to control lcd
  * @param  buf: the pointer for dma data
  * @param  length: data length
  * @param  inc_en : the dma peripheral(data source buffer) address increase or not .
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void qspi_xip_dma_set(uint8_t command, uint8_t* buf, uint32_t length, confirm_state inc_en)
{
  dma_init_type dma_init_struct;
  dma_reset(DMA2_CHANNEL1);
  dma_default_para_init(&dma_init_struct);
 
  /* dma stream1 configuration */
  dma_init_struct.direction = DMA_DIR_MEMORY_TO_MEMORY;
  if((length % 4) == 0)
  {
    dma_init_struct.buffer_size = (uint16_t)(length/4);
    dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_WORD;
    dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_WORD;
  }
  else if((length % 2) == 0)
  {
    dma_init_struct.buffer_size = (uint16_t)(length/2);
    dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
    dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_HALFWORD;
  }
  else
  {
    dma_init_struct.buffer_size = (uint16_t)length;
    dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_BYTE;
    dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_BYTE;
  }
  dma_init_struct.peripheral_base_addr = (uint32_t)buf;
  dma_init_struct.peripheral_inc_enable = inc_en;
  dma_init_struct.memory_base_addr = (uint32_t)(QSPI1_MEM_BASE + (command<<8));
  dma_init_struct.memory_inc_enable = TRUE;
  dma_init_struct.priority = DMA_PRIORITY_HIGH;
  dma_init_struct.loop_mode_enable = FALSE;
  dma_init(DMA2_CHANNEL1, &dma_init_struct);
  
  dmamux_enable(DMA2, TRUE);
  dmamux_init(DMA2MUX_CHANNEL1, DMAMUX_DMAREQ_ID_QSPI1);

  dma_channel_enable(DMA2_CHANNEL1, TRUE);
  
  /* wait dma completed */
  while(dma_flag_get(DMA2_FDT1_FLAG) == RESET);
  dma_flag_clear(DMA2_FDT1_FLAG);
  qspi_dma_enable(QSPI1, FALSE);
  dma_channel_enable(DMA2_CHANNEL1, FALSE);
}

/**
  * @brief  this function is write data to lcd.
  * @param  data : the data to write.
  * @retval none
  */
void lcd_wr_data(uint8_t data)
{
  while(qspi_flag_get(QSPI1, QSPI_TXFIFORDY_FLAG) == RESET);
  QSPI1->dt_u8 = data;
  data_cnt--;
  if(data_cnt == 0)
  {
    while(qspi_flag_get(QSPI1, QSPI_CMDSTS_FLAG) == RESET);
    qspi_flag_clear(QSPI1, QSPI_CMDSTS_FLAG);
  }
}

/**
  * @brief  this function is write command to lcd.
  * @param  command : the command to write.
  * @retval none
  */
void lcd_wr_command(uint8_t command, uint32_t counter)
{
  data_cnt = counter;
  lcd_cmd_write_config(&lcd_cmd_config, (uint32_t)(command<<8), counter);
  qspi_cmd_operation_kick(QSPI1, &lcd_cmd_config);
  if(counter == 0)
  {
    while(qspi_flag_get(QSPI1, QSPI_CMDSTS_FLAG) == RESET);
    qspi_flag_clear(QSPI1, QSPI_CMDSTS_FLAG);
  }
}

/**
  * @brief  this function is write data to lcd through xip interface by memory copy.
  * @param  command : the command to write.
  * @param  wbuf    : the data buffer to write.
  * @param  counter : the data counter to write.
  * @retval none
  */
void lcd_wr_xip(uint8_t command, uint8_t* wbuf, uint32_t counter)
{
  uint32_t *qspi1_mem_addr;

  qspi1_mem_addr = (uint32_t*)(QSPI1_MEM_BASE + (uint32_t)(command<<8));

  memcpy(qspi1_mem_addr, wbuf, counter);
}


/**
  * @brief  configures the lcd.
  *         this function must be called before any write/read operation
  *         on the lcd.
  * @param  none
  * @retval none
  */
void lcd_init(void)
{
 /* qspi config */
 qspi_config();
 
 /* Reset Sequence */
 LCD_RESET_HIGH;
 AT_Delay(50);
 LCD_RESET_LOW;
 AT_Delay(50);
 LCD_RESET_HIGH;	
 AT_Delay(120);

 /* Init Sequence */
 lcd_wr_command(0xfe, 0);
 lcd_wr_command(0xef, 0);
 lcd_wr_command(0x80, 1);
 lcd_wr_data(0x11);
 lcd_wr_command(0x81, 1);
 lcd_wr_data(0x70);

 lcd_wr_command(0x82, 1);	
 lcd_wr_data(0x09);	
 lcd_wr_command(0x83, 1);	
 lcd_wr_data(0x03);	
 lcd_wr_command(0x84, 1);	
 lcd_wr_data(0x62);
 lcd_wr_command(0x89, 1);	
 lcd_wr_data(0x18);	
 lcd_wr_command(0x8A, 1);	
 lcd_wr_data(0x40);
 lcd_wr_command(0x8B, 1);	
 lcd_wr_data(0x0A);

 lcd_wr_command(0x3a, 1);  	
 lcd_wr_data(0x05);
 lcd_wr_command(0x36, 1);
 lcd_wr_data(0x40);
 lcd_wr_command(0xEC, 1);
 lcd_wr_data(0x07);

 lcd_wr_command(0x74, 6);
 lcd_wr_data(0x01);
 lcd_wr_data(0x80);
 lcd_wr_data(0x00);
 lcd_wr_data(0x00);
 lcd_wr_data(0x00);
 lcd_wr_data(0x00);

 lcd_wr_command(0x98, 1);
 lcd_wr_data(0x3E);
 lcd_wr_command(0x99, 1);
 lcd_wr_data(0x3E);

 lcd_wr_command(0xA1, 2);
 lcd_wr_data(0x01);
 lcd_wr_data(0x04);

 lcd_wr_command(0xA2, 2);
 lcd_wr_data(0x01);
 lcd_wr_data(0x04);

 lcd_wr_command(0xCB, 1);
 lcd_wr_data(0x02);

 lcd_wr_command(0x7C, 2);
 lcd_wr_data(0xB6);
 lcd_wr_data(0x24);

 lcd_wr_command(0xAC, 1);
 lcd_wr_data(0x74);

 lcd_wr_command(0xF6, 1);
 lcd_wr_data(0x80);

 lcd_wr_command(0xB5, 2);
 lcd_wr_data(0x09);
 lcd_wr_data(0x09);

 lcd_wr_command(0xEB, 2);
 lcd_wr_data(0x01); 
 lcd_wr_data(0x81); //81

 lcd_wr_command(0x60, 4);
 lcd_wr_data(0x38); 
 lcd_wr_data(0x06);//0c
 lcd_wr_data(0x13); 
 lcd_wr_data(0x56);

 lcd_wr_command(0x63, 4);
 lcd_wr_data(0x38); 
 lcd_wr_data(0x08); //e
 lcd_wr_data(0x13); 
 lcd_wr_data(0x56);

 lcd_wr_command(0x61, 4);
 lcd_wr_data(0x3B); 
 lcd_wr_data(0x1b); 
 lcd_wr_data(0x58); 
 lcd_wr_data(0x38);

 lcd_wr_command(0x62, 4);
 lcd_wr_data(0x3B); 
 lcd_wr_data(0x1b); 
 lcd_wr_data(0x58); 
 lcd_wr_data(0x38);

 lcd_wr_command(0x64, 6);
 lcd_wr_data(0x38); 
 lcd_wr_data(0x0a); 
 lcd_wr_data(0x73); 
 lcd_wr_data(0x16); 
 lcd_wr_data(0x13); 
 lcd_wr_data(0x56);

 lcd_wr_command(0x66, 6);
 lcd_wr_data(0x38);
 lcd_wr_data(0x0b); 
 lcd_wr_data(0x73); 
 lcd_wr_data(0x17); 
 lcd_wr_data(0x13); 
 lcd_wr_data(0x56); 

 lcd_wr_command(0x68, 7);
 lcd_wr_data(0x00); 
 lcd_wr_data(0x0B);
 lcd_wr_data(0x22);
 lcd_wr_data(0x0B); 
 lcd_wr_data(0x22); 
 lcd_wr_data(0x1C);
 lcd_wr_data(0x1C); 

 lcd_wr_command(0x69, 7);
 lcd_wr_data(0x00); 
 lcd_wr_data(0x0B); 
 lcd_wr_data(0x26);
 lcd_wr_data(0x0B); 
 lcd_wr_data(0x26); 
 lcd_wr_data(0x1C); 
 lcd_wr_data(0x1C); 

 lcd_wr_command(0x6A, 2);
 lcd_wr_data(0x15); 
 lcd_wr_data(0x00); 

 lcd_wr_command(0x6E, 32);
 lcd_wr_data(0x08); 
 lcd_wr_data(0x02); 
 lcd_wr_data(0x1a); 
 lcd_wr_data(0x00); 
 lcd_wr_data(0x12); 
 lcd_wr_data(0x12); 
 lcd_wr_data(0x11); 
 lcd_wr_data(0x11); 
 lcd_wr_data(0x14); 
 lcd_wr_data(0x14); 
 lcd_wr_data(0x13); 
 lcd_wr_data(0x13); 
 lcd_wr_data(0x04); 
 lcd_wr_data(0x19); 
 lcd_wr_data(0x1e); 
 lcd_wr_data(0x1d); 
 lcd_wr_data(0x1d); 
 lcd_wr_data(0x1e); 
 lcd_wr_data(0x19); 
 lcd_wr_data(0x04); 
 lcd_wr_data(0x0b); 
 lcd_wr_data(0x0b); 
 lcd_wr_data(0x0c); 
 lcd_wr_data(0x0c); 
 lcd_wr_data(0x09); 
 lcd_wr_data(0x09); 
 lcd_wr_data(0x0a); 
 lcd_wr_data(0x0a); 
 lcd_wr_data(0x00); 
 lcd_wr_data(0x1a);
 lcd_wr_data(0x01); 
 lcd_wr_data(0x07); 

 lcd_wr_command(0x6C, 7);
 lcd_wr_data(0xCC); 
 lcd_wr_data(0x0C); 
 lcd_wr_data(0xCC); 
 lcd_wr_data(0x84); 
 lcd_wr_data(0xCC); 
 lcd_wr_data(0x04); 
 lcd_wr_data(0x50); 

 lcd_wr_command(0x7D, 1);
 lcd_wr_data(0x72);

 lcd_wr_command(0x70, 10);
 lcd_wr_data(0x02);
 lcd_wr_data(0x03);
 lcd_wr_data(0x09);
 lcd_wr_data(0x07);
 lcd_wr_data(0x09);
 lcd_wr_data(0x03);
 lcd_wr_data(0x09);
 lcd_wr_data(0x07);
 lcd_wr_data(0x09); 
 lcd_wr_data(0x03);

 lcd_wr_command(0x90, 4);
 lcd_wr_data(0x06);
 lcd_wr_data(0x06);
 lcd_wr_data(0x05);
 lcd_wr_data(0x06);

 lcd_wr_command(0x93, 3);
 lcd_wr_data(0x45);
 lcd_wr_data(0xFF);
 lcd_wr_data(0x00);

 lcd_wr_command(0xC3, 1);
 lcd_wr_data(0x15);
 lcd_wr_command(0xC4, 1);
 lcd_wr_data(0x36);
 lcd_wr_command(0xC9, 1);
 lcd_wr_data(0x3d);

 lcd_wr_command(0xF0, 6);   
 lcd_wr_data(0x47);//gVR1_N[5:0]
 lcd_wr_data(0x07);//gVR2_N[5:0]
 lcd_wr_data(0x0A);//gVR4_N[4:0]
 lcd_wr_data(0x0A);//gVR6_N[4:0]
 lcd_wr_data(0x00);//gVR0_N[3:0] gVR13_N[3:0] 7
 lcd_wr_data(0x29);//gVR20_N[6:0]

 lcd_wr_command(0xF2, 6);   
 lcd_wr_data(0x47);//gVR1_P[5:0]
 lcd_wr_data(0x07);//gVR2_P[5:0]
 lcd_wr_data(0x0a);//gVR4_P[4:0]
 lcd_wr_data(0x0A);//gVR6_P[4:0]
 lcd_wr_data(0x00);//gVR0_P[3:0] gVR13_P[3:0]
 lcd_wr_data(0x29);//gVR20_P[6:0]

 lcd_wr_command(0xF1, 6);    
 lcd_wr_data(0x42);//gVR43_N[6:0]
 lcd_wr_data(0x91);//gVR27_N[2:0] gVR57_N[4:0]
 lcd_wr_data(0x10);//gVR36_N[2:0] gVR59_N[4:0]
 lcd_wr_data(0x2D);//gVR61_N[5:0]
 lcd_wr_data(0x2F);// gVR62_N[5:0]
 lcd_wr_data(0x6F);//gVR50_N[3:0] gVR63_N[3:0]

 lcd_wr_command(0xF3, 6);   
 lcd_wr_data(0x42);//gVR43_P[6:0]
 lcd_wr_data(0x91);//gVR27_P[2:0] gVR57_P[4:0]
 lcd_wr_data(0x10);//gVR36_P[2:0] gVR59_P[4:0]
 lcd_wr_data(0x2D);//gVR61_P[5:0]
 lcd_wr_data(0x2F);// gVR62_P[5:0]
 lcd_wr_data(0x6F);//gVR50_P[3:0] gVR63_P[3:0]

 lcd_wr_command(0xF9, 1);   
 lcd_wr_data(0x30);

 lcd_wr_command(0xBE, 1);
 lcd_wr_data(0x11); 

 lcd_wr_command(0xFB, 2);
 lcd_wr_data(0x00); 
 lcd_wr_data(0x00); 

 /*
 //lcd_wr_command(0xB1);   
 //lcd_wr_data(0x08);    // SPI_ 2dat
 */

 lcd_wr_command(0x11, 0);	//turns off sleep mode
 AT_Delay(120);	 
 lcd_wr_command(0x29, 0);//display on
 AT_Delay(120);	
 lcd_wr_command(0x2C, 4);
 lcd_wr_data(0x00);
 lcd_wr_data(0x00);
 lcd_wr_data(0x00);
 lcd_wr_data(0x00);
 lcd_wr_command(0x2C, 4);
 lcd_wr_data(0x00);
 lcd_wr_data(0x00);
 lcd_wr_data(0x00);
 lcd_wr_data(0x00);
 AT_Delay(120);

 /* enable and config qspi's dma mode */
 qspi_dma_enable(QSPI1, TRUE);
 qspi_dma_tx_threshold_set(QSPI1, QSPI_DMA_FIFO_THOD_WORD08);
 
 #ifdef xip_dma
 /* enable qspi xip mode, the following qspi operations all in xip mode */
 qspi_xip_enable(QSPI1, TRUE);
 #endif
}

/**
  * @brief  this function is set the lcd enter sleep.
  * @param  none
  * @retval none
  */
void lcd_enter_sleep(void)
{
	lcd_wr_command(0x28, 0);
	AT_Delay(120);
	lcd_wr_command(0x10, 0);
	AT_Delay(50);
}

/**
  * @brief  this function is set the lcd exit sleep.
  * @param  none
  * @retval none
  */
void lcd_exit_sleep(void)
{
	lcd_wr_command(0x11, 0);
	AT_Delay(120) ;
	lcd_wr_command(0x29, 0);
}


/**
  * @brief  this function is set row&column coordinates for lcd.
  * @param  xstart : row coordinates starting vaule.
  * @param  ystart : column coordinates starting vaule.
  * @param  xend : row coordinates ending vaule.
  * @param  yend : column coordinates ending vaule.
  * @retval none
  */
void lcd_setblock(uint16_t xstart, uint16_t ystart, uint16_t xend, uint16_t yend)
{
  #ifdef command_dma
  /* set row coordinates */
  lcd_wr_command(0x2a, 4);
  lcd_wr_data(xstart >> 8);
  lcd_wr_data(xstart);
  lcd_wr_data(xend >> 8);
  lcd_wr_data(xend);
 
  lcd_wr_command(0x2b, 4);
  lcd_wr_data(ystart >> 8);
  lcd_wr_data(ystart);
  lcd_wr_data(yend >> 8);
  lcd_wr_data(yend);
  #else
  uint8_t buf[4];
 
  /* set row coordinates */
  buf[0] = (uint8_t)(xstart >> 8);
  buf[1] = (uint8_t)xstart;
  buf[2] = (uint8_t)(xend >> 8);
  buf[3] = (uint8_t)xend;
  qspi_xip_dma_set(0x2a, buf, 4, TRUE);
  
  /* set column coordinates */
  buf[0] = (uint8_t)(ystart >> 8);
  buf[1] = (uint8_t)ystart;
  buf[2] = (uint8_t)(yend >> 8);
  buf[3] = (uint8_t)yend;
  qspi_xip_dma_set(0x2b, buf, 4, TRUE);
  #endif
}


/**
  * @brief  this function is draw a point on lcd.
  * @param  x      : the x_axis value of the point
  * @param  y      : the y_axis value of the point
  * @param  color  : the color of the point
  * @retval none
  */
void lcd_drawpoint(uint16_t x, uint16_t y, uint16_t color)
{
  uint8_t buf[4];
  buf[0]=color>>8;
  buf[1]=color;
  buf[2]=color>>8;
  buf[3]=color;
 
  if(x<2)x=2;
  if(y<2)y=2;

 lcd_setblock(x-2, y-2, x+2, y+2);
 #ifdef command_dma
 qspi_cmd_dma_set(0x2C, buf,2*25, FALSE);
 #else
 qspi_xip_dma_set(0x2C, buf, 2*25, FALSE);
 #endif
}

/**
  * @brief  this function is draw a line on lcd.
  * @param  xstart : the x_axis value of the starting point
  * @param  ystart : the y_axis value of the starting point
  * @param  xend   : the x_axis value of the ending point
  * @param  yend   : the y_axis value of the ending point
  * @param  color  : the color of the line
  * @retval none
  */
void lcd_drawline(uint16_t xstart, uint16_t ystart, uint16_t xend, uint16_t yend, uint16_t color)
{
  uint16_t x,y;
  float slope;
  x = xstart;
  y = ystart;

  if((yend-ystart)>(xend-xstart))
  {
    slope = (float)(xend-xstart)/(yend-ystart);
    for(y=ystart;y<yend;y++)
    {
      x = xstart+(uint16_t)((float)(y-ystart))*slope;
      lcd_drawpoint(x, y, color);
    }
  }
  else
  {
    slope = (float)(yend-ystart)/(xend-xstart);
    for(x=xstart;x<xend;x++)
    {
      y = ystart+(uint16_t)((float)(x-xstart))*slope;
      lcd_drawpoint(x, y, color);
    }
  }
}

/**
  * @brief  this function is clear the lcd.
  * @param  color  : the color be used to clear
  * @retval none
  */
void lcd_clear(uint16_t color)
{
  uint8_t buf[4];
  buf[0] = color >> 8;
  buf[1] = color;
  buf[2] = color >> 8;
  buf[3] = color;
 
  lcd_setblock(0, 0, COL-1, ROW-1);
 
  #ifdef command_dma
  qspi_cmd_dma_set(0x2C, buf,ROW*COL*2, FALSE);
  #else
  qspi_xip_dma_set(0x2C, buf,ROW*COL*2, FALSE);
  #endif
}


/**
  * @brief  this function is display number on the lcd.
  * @param  none
  * @retval none
  */
void lcd_display_number(void)
{ 
 lcd_setblock(85, 108, 234, 277);
 #ifdef command_dma
 qspi_cmd_dma_set(0x2C, (uint8_t*)gimage_3, 51000, TRUE);
 #else
 qspi_xip_dma_set(0x2C, (uint8_t*)gimage_3, 51000, TRUE);
 #endif

 AT_Delay(1000);
  
 lcd_setblock(85, 108, 234, 277);
 #ifdef command_dma
 qspi_cmd_dma_set(0x2C, (uint8_t*)gimage_8, 51000, TRUE);
 #else
 qspi_xip_dma_set(0x2C, (uint8_t*)gimage_8, 51000, TRUE);
 #endif

 AT_Delay(1000);

 lcd_setblock(85, 108, 234, 277);
 #ifdef command_dma
 qspi_cmd_dma_set(0x2C, (uint8_t*)gimage_5, 51000, TRUE);
 #else
 qspi_xip_dma_set(0x2C, (uint8_t*)gimage_5, 51000, TRUE);
 #endif

 AT_Delay(1000);
  
 lcd_setblock(85, 108, 234, 277);
 #ifdef command_dma
 qspi_cmd_dma_set(0x2C, (uint8_t*)gimage_0, 51000, TRUE);
 #else
 qspi_xip_dma_set(0x2C, (uint8_t*)gimage_0, 51000, TRUE);
 #endif

 AT_Delay(1000);
}

/**
  * @}
  */

/**
  * @}
  */

