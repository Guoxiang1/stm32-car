#include "stm32f10x.h" // 标准库头文件
#include "motor.h"
#include "usart.h"
#include "bluetooth.h"

extern u8 *cmd;

void process_bluetooth_cmd(void);