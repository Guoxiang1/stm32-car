
#include "mpu6050.h"
#include "i2c.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "usart.h"
#include "math.h"

static signed char gyro_orientation[9] = {-1, 0, 0,
                                           0,-1, 0,
                                           0, 0, 1};
float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;
extern float Pitch, Roll, Yaw;
unsigned long sensor_timestamp;
short gyro[3], accel[3], sensors;
unsigned char more;
long quat[4];


 
void MPU6050_Init(void)
{
	int result=0;
	//IIC_Init();
	result=mpu_init();
	if(!result)
	{	 		 
	
		printf("mpu initialization complete......\n ");		//mpu initialization complete	 	  

		if(!mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL))		//mpu_set_sensor
			printf("mpu_set_sensor complete ......\n");
		else
			printf("mpu_set_sensor come across error ......\n");

		if(!mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL))	//mpu_configure_fifo
			printf("mpu_configure_fifo complete ......\n");
		else
			printf("mpu_configure_fifo come across error ......\n");

		if(!mpu_set_sample_rate(DEFAULT_MPU_HZ))	   	  		//mpu_set_sample_rate
		 printf("mpu_set_sample_rate complete ......\n");
		else
		 	printf("mpu_set_sample_rate error ......\n");

		if(!dmp_load_motion_driver_firmware())   	  			//dmp_load_motion_driver_firmvare
			printf("dmp_load_motion_driver_firmware complete ......\n");
		else
			printf("dmp_load_motion_driver_firmware come across error ......\n");

		if(!dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation))) 	  //dmp_set_orientation
		 	printf("dmp_set_orientation complete ......\n");
		else
		 	printf("dmp_set_orientation come across error ......\n");

		if(!dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
		    DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
		    DMP_FEATURE_GYRO_CAL))		   	 					 //dmp_enable_feature
		 	printf("dmp_enable_feature complete ......\n");
		else
		 	printf("dmp_enable_feature come across error ......\n");

		if(!dmp_set_fifo_rate(DEFAULT_MPU_HZ))   	 			 //dmp_set_fifo_rate
		 	printf("dmp_set_fifo_rate complete ......\n");
		else
		 	printf("dmp_set_fifo_rate come across error ......\n");

		run_self_test();		//自检

		if(!mpu_set_dmp_state(1))
		 	printf("mpu_set_dmp_state complete ......\n");
		else
		 	printf("mpu_set_dmp_state come across error ......\n");
	}
	else												 //MPU6050状态指示灯 STM32核心板 PC13 绿色灯亮起为不正常
	 {
	 GPIO_ResetBits(GPIOC, GPIO_Pin_13);				//MPU6050状态指示灯 STM32核心板 PC13 绿色灯亮起为不正常
	 while(1);
	 }
	 
}

/*
 *  @param[out] gyro        Gyro data in hardware units.    							陀螺仪数据的硬件单位。
 *  @param[out] accel       Accel data in hardware units.    							硬件单位的加速数据。
 *  @param[out] quat        3-axis quaternion data in hardware units.    硬件单元中的三轴四元数数据
 *  @param[out] timestamp   Timestamp in milliseconds.							     时间戳以毫秒为单位。
 *  @param[out] sensors     Mask of sensors read from FIFO.								从FIFO读取传感器的掩模。
 *  @param[out] more        Number of remaining packets.								剩余报文数。
*/
void MPU6050_Pose(void)
{
	
	dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors,&more);	 //gyro, accel全局变量  陀螺仪数据存放在这里面
	/* Gyro and accel data are written to the FIFO by the DMP in chip frame and hardware units.
	 * This behavior is convenient because it keeps the gyro and accel outputs of dmp_read_fifo and mpu_read_fifo consistent.
	**/
	/*if (sensors & INV_XYZ_GYRO )
	send_packet(PACKET_TYPE_GYRO, gyro);
	if (sensors & INV_XYZ_ACCEL)
	send_packet(PACKET_TYPE_ACCEL, accel); */
	/* Unlike gyro and accel, quaternions are written to the FIFO in the body frame, q30.
	 * The orientation is set by the scalar passed to dmp_set_orientation during initialization. 
	**/
	
	
	if(sensors & INV_WXYZ_QUAT )    //Pitch,Roll,Yaw;全局变量
	{
		q0 = quat[0] / q30;	
		q1 = quat[1] / q30;
		q2 = quat[2] / q30;
		q3 = quat[3] / q30;

		Pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3;	// pitch
		Roll  = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)*57.3;	// roll
		Yaw   = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;	//yaw
    
		
	}
}
