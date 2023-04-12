#ifndef __DELAY_H
#define __DELAY_H

#include "at32f435_437.h"
#include "at32f435_437_conf.h"

#include "gpio.h"
#include "i2c.h"

/* RTT打印组件 */
#define RTT_DBG_ENABLE          (1)
#if RTT_DBG_ENABLE
    #include "SEGGER_RTT.h"
	/* 初始化调试模式 */
    #define DEBUG_INIT()    (SEGGER_RTT_Init(),log_clear())
    /* RTT 终端通道 */
    #define RTT_DBG_PORT        0
	#define LOG_PROTO(type,color,format,...)                        \
			SEGGER_RTT_printf(RTT_DBG_PORT,"%s%s"format"\r\n",    	\
							color,                                  \
							type,                                   \
							##__VA_ARGS__)
    /* 清屏 */
    #define log_clear()     SEGGER_RTT_WriteString(RTT_DBG_PORT, "  "RTT_CTRL_CLEAR)
    /* 无颜色日志输出 */
    #define log_debug(format,...)   LOG_PROTO("DEBUG> ","",format,##__VA_ARGS__)
    /* 有颜色格式日志输出 */
    #define log_info(format,...)    LOG_PROTO("INFO> ", RTT_CTRL_TEXT_BRIGHT_GREEN , format, ##__VA_ARGS__)
    #define log_warn(format,...)    LOG_PROTO("WARN> ", RTT_CTRL_TEXT_BRIGHT_YELLOW, format, ##__VA_ARGS__)
    #define log_error(format,...)   LOG_PROTO("ERROR> ", RTT_CTRL_TEXT_BRIGHT_RED   , format, ##__VA_ARGS__)

#else
    #define DEBUG_INIT()
    #define log_clear()
    #define log_debug
    #define log_info
    #define log_warn
    #define log_error
#endif

/* 延时组件 */
/* 初始化延时组件 */
void AT_Delay_Init(void);

/* 毫秒级延时 */
void AT_Delay(uint16_t nms);

/* 微秒级延时 */
void AT_Delay_us(uint32_t nus);

/* 秒级延时 */
void AT_Delay_ms(uint16_t sec);


#endif /* __DELAY_H */

