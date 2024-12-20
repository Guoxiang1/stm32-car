#include "control.h"
#include <string.h>
#include "motor.h"
#include "usart.h"	

u8 *cmd;

void process_bluetooth_cmd(void) {
    if (strcmp((char *)cmd, "1") == 0) {
        Car_Forward();   	// 小车前进
		printf("接收到的数据为：%s\r\n", cmd);

    } else if (strcmp((char *)cmd, "2") == 0) {
        Car_Backward();		// 小车后退
		printf("接收到的数据为：%s\r\n", cmd);

    } else if (strcmp((char *)cmd, "3") == 0) {
        Car_Stop();			// 小车停止
		printf("接收到的数据为：%s\r\n", cmd);

    } else if (strcmp((char *)cmd, "4") == 0) {
        Car_TurnLeft();		// 小车左转
		printf("接收到的数据为：%s\r\n", cmd);

    } else if (strcmp((char *)cmd, "5") == 0) {
        Car_SlightLeft();   // 小车小左转
		printf("接收到的数据为：%s\r\n", cmd);

    } else if (strcmp((char *)cmd, "6") == 0) {
        Car_TurnRight();	// 小车右转
		printf("接收到的数据为：%s\r\n", cmd);

    } else if (strcmp((char *)cmd, "7") == 0) {
        Car_SlightRight();	// 小车小右转
		printf("接收到的数据为：%s\r\n", cmd);
    }
}
