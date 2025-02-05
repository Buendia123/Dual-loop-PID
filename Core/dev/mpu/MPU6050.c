#include "MPU6050.h"
#include "drv_i2cm.h"
#include "inv_mpu.h"
#include "math.h"


#define PRINT_ACCEL     (0x01)
#define PRINT_GYRO      (0x02)
#define PRINT_QUAT      (0x04)
#define ACCEL_ON        (0x01)
#define GYRO_ON         (0x02)
#define MOTION          (0)
#define NO_MOTION       (1)
//#define DEFAULT_MPU_HZ  (200)
#define FLASH_SIZE      (512)
#define FLASH_MEM_START ((void*)0x1800)
#define q30  1073741824.0f
short gyro[3], accel[3], sensors;

float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;
uint8_t buffer[14];
uint16_t  MPU6050_FIFO[6][11];




 /* CLK_SEL | Clock Source
 * --------+--------------------------------------
 * 0       | Internal oscillator
 * 1       | PLL with X Gyro reference
 * 2       | PLL with Y Gyro reference
 * 3       | PLL with Z Gyro reference
 * 4       | PLL with external 32.768kHz reference
 * 5       | PLL with external 19.2MHz reference
 * 6       | Reserved
 * 7       | Stops the clock and keeps the timing generator in reset
*******************************************************************************/
BOOL MPU6050_setClockSource(uint8_t source){
   return IICwriteBits(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, source);

}

/** Set full-scale gyroscope range.
 * @param range New full-scale gyroscope range value
 * @see getFullScaleRange()
 * @see MPU6050_GYRO_FS_250
 * @see MPU6050_RA_GYRO_CONFIG
 * @see MPU6050_GCONFIG_FS_SEL_BIT
 * @see MPU6050_GCONFIG_FS_SEL_LENGTH
 */
BOOL MPU6050_setFullScaleGyroRange(uint8_t range) {
    return IICwriteBits(devAddr, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range);
}


BOOL MPU6050_setFullScaleAccelRange(uint8_t range) {
    return IICwriteBits(devAddr, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
}


BOOL MPU6050_setSleepEnabled(uint8_t enabled) {
    return IICwriteBit(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, enabled);
}



BOOL MPU6050_testConnection(void) {
	uint8_t res;
	 IICreadBytes(devAddr, MPU6050_RA_WHO_AM_I, 1, &res);
   if(res == MPU_ADDR)
	   return TRUE;
   else
	   return FALSE;
}


BOOL MPU6050_setI2CMasterModeEnabled(uint8_t enabled) {
    return IICwriteBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_I2C_MST_EN_BIT, enabled);
}


BOOL MPU6050_setI2CBypassEnabled(uint8_t enabled) {
    return IICwriteBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, enabled);
}

BOOL MPU6050_set_sample_rate(uint8_t divider)
{
	return IICwriteBits(devAddr, MPU6050_RA_SMPLRT_DIV,7, 8, divider);
}

BOOL MPU6050_Restart(void)
{
	BOOL bDone = FALSE;
	uint8_t WriteBuf[2] = {MPU6050_RA_PWR_MGMT_1,0x80};
	if (!DRV_I2CM_Stream(devAddr & 0xFF, 2, WriteBuf, 0, NULL))
	{
		return bDone;
	}
	HAL_Delay(100);
	return TRUE;
}

BOOL MPU6050_ClearAllInt()
{
	uint8_t WriteBuf[2] = {MPU6050_RA_PWR_MGMT_2,0x00};
	if (!DRV_I2CM_Stream(devAddr & 0xFF, 2, WriteBuf, 0, NULL))
	{
		return FALSE;
	}
	return TRUE;
}


BOOL MPU6050_ClearFIFO()
{
	uint8_t WriteBuf[2] = {MPU6050_RA_FIFO_EN,0x00};
	if (!DRV_I2CM_Stream(devAddr & 0xFF, 2, WriteBuf, 0, NULL))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL MPU6050_setINTPinLow()
{
	uint8_t WriteBuf[2] = {MPU6050_RA_INT_PIN_CFG,0x80};
	if (!DRV_I2CM_Stream(devAddr & 0xFF, 2, WriteBuf, 0, NULL))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL MPU6050_initialize(void)
{
	BOOL bDone = FALSE;
	if (!MPU6050_testConnection)
		return bDone;
	if (!MPU6050_Restart())
			return bDone;
	if (!MPU6050_setClockSource(MPU6050_CLOCK_PLL_XGYRO))
		return bDone;
	if(!MPU6050_setFullScaleGyroRange(MPU6050_GYRO_FS_2000))
		return bDone;
	if (!MPU6050_setFullScaleAccelRange(MPU6050_ACCEL_FS_2))
		return bDone;
	if (!MPU6050_set_sample_rate((1000 / MPU6050_BASIC_DEFAULT_RATE) - 1))
		return bDone;
	if (!MPU6050_setSleepEnabled(0))
		return bDone;
	if (!MPU6050_setI2CMasterModeEnabled(0))
		return bDone;
	if (!MPU6050_setI2CBypassEnabled(0))
		return bDone;
	if (!MPU6050_ClearAllInt())
		return bDone;
	if (!MPU6050_setINTPinLow(0))
		return bDone;
	if (!MPU6050_ClearFIFO(0))
		return bDone;
	return TRUE;
}

void Read_DMP(struct pose_angle* angle)
{
	unsigned long sensor_timestamp = 100;
	unsigned char more;
	long quat[4];

	dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors, &more);
	if (sensors & INV_WXYZ_QUAT )
	{
	 q0=quat[0] / q30;
	 q1=quat[1] / q30;
	 q2=quat[2] / q30;
	 q3=quat[3] / q30;
	 angle->acc->Roll = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3;
	 angle->acc->Pitch = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3;
	 angle->acc->Yaw = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;
	 angle->gyro->gyro_Pitch=gyro[0];
	 angle->gyro->gyro_Roll=gyro[1];
	 angle->gyro->gyro_Yaw=gyro[2];
//	 Roll  = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3;
//	 Pitch = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3;
//	 Yaw   = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;
//	 gyro_Pitch=gyro[0];
//	 gyro_Roll=gyro[1];
//	 gyro_Yaw=gyro[2];
	}

}


//------------------End of File----------------------------
