#include "drv_i2cm.h"
#include "drv_delay.h"
#include <stdio.h>
#include <string.h>

#if DRV_I2CM_SUPPORT  || 1

#define DRV_I2CM_FixRate()      do {                                     \
                                    DRV_DELAY_625ns();                   \
                                    DRV_DELAY_625ns();                   \
                                    DRV_DELAY_625ns();                   \
                                } while (0)
/* TODO:Must modify delay times */
#define DRV_I2CM_SET_SCL(_v)    do {                                        \
                                    DRV_I2CM_FixRate();                     \
									HAL_GPIO_WritePin(GPIOB,MPU_SCL, _v);		        \
                                    /* t_r/t_f: 300ns */                    \
                                    DRV_DELAY_125ns();                      \
                                } while (0)

/* porting part */
#define DRV_I2CM_SET_SDA(_v)    HAL_GPIO_WritePin(GPIOB,MPU_SDA, _v)
#define DRV_I2CM_GET_SCL()		HAL_GPIO_ReadPin(GPIOB,MPU_SCL)
#define DRV_I2CM_GET_SDA()      HAL_GPIO_ReadPin(GPIOB,MPU_SDA)


//default I2C setting
void drv_i2cm_Start()
{
    /* I2C Start/ReStart:
     *   SDA changed from HIGH(1) to LOW(0), while SCL is HIGH(1).
     */
    DRV_I2CM_SET_SDA(HIGH);
    DRV_I2CM_SET_SCL(HIGH);
    DRV_I2CM_FixRate();     /* t_BUF */
    DRV_I2CM_SET_SDA(LOW);  /* t_HD;STA */
    DRV_I2CM_FixRate();
    DRV_I2CM_SET_SCL(LOW);
}

//void drv_i2cm_Stop(void)
//{
//    /* SDA changed from LOW to HIGH, while SCL is HIGH */
//    DRV_I2CM_SET_SDA(LOW);
//    DRV_I2CM_SET_SCL(HIGH);
//    DRV_I2CM_FixRate(); /* t_SU;STO */
//    DRV_I2CM_SET_SDA(HIGH);
//}


void drv_i2cm_Init(void)
{

}
/* return:
 *  =TRUE,  an ACK received;
 *  =FALSE, no ACK received.
 */
static BOOL drv_i2cm_TxByte(uint8_t vData)
{
    uint8_t   vLoop;
    BOOL    vAck;
    for (vLoop = 8; vLoop != 0; vLoop--)
    {
        /* transmitting data, MSB first, LSB last */
        DRV_I2CM_SET_SDA((vData>>(vLoop-1))&0x1);
        DRV_I2CM_SET_SCL(HIGH);
        DRV_I2CM_SET_SCL(LOW);
    }

    /* release SDA */
    DRV_I2CM_SET_SDA(HIGH);

    /* check ACK */
    DRV_I2CM_SET_SCL(HIGH);
    vAck = DRV_I2CM_GET_SDA();
     DRV_I2CM_SET_SCL(LOW);

    return !vAck;
}

static uint8_t drv_i2cm_RxByte(BOOL bSendNAK)
{
    uint8_t   vLoop;
    uint8_t   vData = 0;
    for (vLoop = 8; vLoop != 0; vLoop--)
    {
        DRV_I2CM_SET_SCL(HIGH);
        vData <<= 1;
        vData |= DRV_I2CM_GET_SDA();
        DRV_I2CM_SET_SCL(LOW);
    }
    /* send ACK/NAK */
    DRV_I2CM_SET_SDA(bSendNAK);
    DRV_I2CM_SET_SCL(HIGH);
    DRV_I2CM_SET_SCL(LOW);
    DRV_I2CM_SET_SDA(HIGH);

    return vData;
}


/******************************************************************************
 * FUNCTION NAME:
 *   DRV_I2CM_Stream
 * DESCRIPTION:
 *   I2C master send write/read combined stream.
 * PARAMETERS:
 *   vI2cAddr  : I2C slave chip address.
 *   vWriteLen : Write length.
 *   aWriteBuf : Write buffer.
 *   vReadLen  : Read length.
 *   aReadBuf  : Read buffer.
 * RETURN:
 *   TRUE   : Send stream successfully.
 *   FALSE  : Send stream failed.
 * NOTES:
 *   N/A
 * HISTORY:
 *   @2021.6.1        Fremon          Create
 *****************************************************************************/
BOOL DRV_I2CM_Stream(uint8_t vI2cAddr,uint8_t vWriteLen, uint8_t *aWriteBuf, uint8_t vReadLen, uint8_t *aReadBuf)
{
    BOOL    bDone = FALSE;
    uint8_t   vLoop = 0;

    //ENTER_CS_L3();
    do
    {
        /* send write stream */
        if ((vWriteLen != 0) || ((vWriteLen == 0) && (vReadLen == 0)))
        {
            /* send I2C start */
            drv_i2cm_Start();

            /* send I2C slave address + write */
            if (!drv_i2cm_TxByte(vI2cAddr&0xFE))
            {
                break;
            }

            /* send write buffer */
            for (vLoop = 0; vLoop < vWriteLen; vLoop++)
            {
                if (!drv_i2cm_TxByte(aWriteBuf[vLoop]))
                {
                    break;
                }
            }
        }

        /* send read stream */
        if (vReadLen != 0)
        {
            /* send I2C start/restart */
            drv_i2cm_Start();
            DRV_I2CM_FixRate();
            /* send I2C slave address + read */
            if (!drv_i2cm_TxByte(vI2cAddr | 0x01))
            {
                break;
            }
            DRV_DELAY_625ns();
            /* read data */
            for (vLoop = 0; vLoop < vReadLen; vLoop++)
            {
                aReadBuf[vLoop] = drv_i2cm_RxByte(vLoop==(vReadLen-1));
            }
        }

        /* I2C stream done */
        bDone = TRUE;

    }while(0);

    /* send I2C stop */
//    drv_i2cm_Stop();

    //EXIT_CS_L3();
    return bDone;
}


/******************************************************************************
 * FUNCTION NAME:
 *   DRV_I2CM_Probe
 * DESCRIPTION:
 *   I2C master probe chip.
 * PARAMETERS:
 *   vI2cAddr : I2C chip address.
 * RETURN:
 *   TRUE   : Probe successfully.
 *   FALSE  : Probe failed.
 * NOTES:
 *   N/A
 * HISTORY:
 *   @2021.6.1        Fremon          Create
 *****************************************************************************/
BOOL DRV_I2CM_Probe(uint8_t vI2cAddr)
{
    return DRV_I2CM_Stream(vI2cAddr & 0xFF, 0, NULL, 0, NULL);

}

/******************************************************************************
 * FUNCTION NAME:
 *   DRV_I2CM_Init
 * DESCRIPTION:
 *   I2C master driver init.
 * PARAMETERS:
 *   N/A
 * RETURN:
 *   N/A
 * NOTES:
 *   N/A
 * HISTORY:
 *   @2021.6.1        Fremon          Create
 *****************************************************************************/
void DRV_I2CM_Init(void)
{
    // DRV_IO_SetMode(SCL_M1,LL_GPIO_MODE_OUTPUT);
    // DRV_IO_SetOutType(SCL_M1,LL_GPIO_OUTPUT_OPENDRAIN);
    // DRV_IO_SetMode(SDA_M1,LL_GPIO_MODE_OUTPUT);
    // DRV_IO_SetOutType(SDA_M1,LL_GPIO_OUTPUT_OPENDRAIN);
    /* make sure both SCL/SDA lines are idle state */
    DRV_I2CM_SET_SCL(HIGH);
	DRV_I2CM_SET_SDA(HIGH);
}
#endif /**/

int i2cWrite(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data)
{
	uint8_t WriteBuf[1 + len];
	WriteBuf[0] = reg;
	memcpy(&WriteBuf[1], data, len);
	if (!DRV_I2CM_Stream(addr, len+1,WriteBuf,0,NULL))
	{
		uint8_t* Err = "IIC Write Err";
		OLED_Clear();
		OLED_ShowString(60,0,Err,8,1);
		OLED_ShowString(0,10,reg,8,1);
		return 1;
	}
	return 0;
}

int i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data)
{
	if (!DRV_I2CM_Stream(addr, 1,reg,len,data))
	{
		uint8_t* Err = "IIC Write Err";
		OLED_Clear();
		OLED_ShowString(60,0,Err,8,1);
		OLED_ShowString(0,10,reg,8,1);
		return 1;
	}
	return 0;
}

BOOL IICwriteBytes(uint8_t i2c_addr, uint8_t offset, uint8_t write_len, uint8_t* write_buf)
{
	uint8_t WriteBuf[1 + write_len];
	WriteBuf[0] = offset;
	memcpy(&WriteBuf[1], write_buf, write_len);
	if (!DRV_I2CM_Stream(i2c_addr, write_len+1,WriteBuf,0,NULL))
	{
		char Err[30]; // 确保有足够的空间容纳格式化后的字符串
		snprintf(Err, sizeof(Err), "IIC Write Err at 0x%02X", offset);
		OLED_Clear();
		OLED_ShowString(60, 0, Err, 8, 1);
		OLED_Refresh();
		return FALSE;
	}
	return TRUE;
}

BOOL IICreadBytes(uint8_t i2c_addr, uint8_t offset, uint8_t read_len, uint8_t* read_buf)
{

	if (!DRV_I2CM_Stream(i2c_addr, 1,&offset,read_len,read_buf))
	{
		char Err[30]; // 确保有足够的空间容纳格式化后的字符串
		snprintf(Err, sizeof(Err), "IIC Read Err at 0x%02X", offset);
		OLED_Clear();
		OLED_ShowString(60, 0, Err, 8, 1);
		OLED_Refresh();
		return FALSE;
	}
	return TRUE;
}

BOOL IICwriteBits(uint8_t I2cAdd, uint8_t OffSet, uint8_t bitstart, uint8_t len, uint8_t Data)
{
	uint8_t WriteBuf[2] = {0};
	WriteBuf[0] = OffSet;

	uint8_t mask = 0;
	mask = ((0xFF)<<(bitstart+1));
	mask |= 0xFF>>(8-bitstart+len-1);
	Data = Data<<(8-len);
	Data>>=(7-bitstart);
	WriteBuf[1]&=mask;
	WriteBuf[1]|=Data;
	if (!DRV_I2CM_Stream(I2cAdd & 0xFF, 2, WriteBuf, 0, NULL))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL IICwriteBit(uint8_t I2cAdd, uint8_t OffSet, uint8_t bitadd, uint8_t Data)
{
	uint8_t WriteBuf[2] = {0};

	WriteBuf[0] = OffSet;
	WriteBuf[1] = (Data != 0)?(WriteBuf[1] |= (1<<bitadd)) : (WriteBuf[1] |= ~(1<<bitadd));
	if (!DRV_I2CM_Stream(I2cAdd & 0xFF, 2, WriteBuf, 0, NULL))
	{
		return FALSE;
	}
	return TRUE;
}
