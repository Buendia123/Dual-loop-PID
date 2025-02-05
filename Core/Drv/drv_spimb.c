/******************************************************************************
 *
 * COPYRIGHT:
 *   Copyright      Volex.      All rights reserved.
 *
 *
 * DESCRIPTION:
 *   SPI Master driver related.
 * HISTORY:
 *   @2022.09.01        AAA          Create
 *
 *****************************************************************************/

#include "drv_spimb.h"
#include "drv_delay.h"

#if DRV_SPIM0_SUPPORT
#define DRV_SPIM0_MODE		0
#define DRV_SPIM0_CS_ACTIVE	0
 /* SPI Mode Definition:
  *
  *  Mode      Clock Polarity    Clock Phase
  *            (CPOL)            (CPHA)
  * -------  ----------------   -------------
  *  MODE0	       0	             0
  *  MODE1	       0	             1
  *  MODE2	       1	             0
  *  MODE3	       1	             1
  *
  * see below link for details:
  *  https://en.wikipedia.org/wiki/Serial_Peripheral_Interface_Bus
  *  http://dlnware.com/theory/SPI-Transfer-Modes
  */

#if (DRV_SPIM0_RATE == 400)
#define DRV_SPIM_FixRate()	drv_delay_1P25us()
#elif (DRV_SPIM0_RATE == 1000)
#define DRV_SPIM_FixRate()	drv_delay_1P25us()
#else
#define DRV_SPIM_FixRate()	 do {                                     \
                                    DRV_DELAY_625ns();                   \
                                    DRV_DELAY_625ns();                   \
                                } while (0)
#endif



uint8_t drv_oled_spim_stream(uint8_t pTxData, uint8_t CmdFlag)
{
	uint16_t j;

	DRV_SPIM_DC_SET(CmdFlag);
	DRV_SPIM_FixRate();

	for (j = 0; j < 8; j++)
	{
		DRV_SPIM_SCK_SET(0);
		if ((pTxData & 0x80) == 0x80)
		{
			DRV_SPIM_SDA_SET(1);
		}
		else
		{
			DRV_SPIM_SDA_SET(0);
		}
		DRV_SPIM_SCK_SET(1);
		DRV_SPIM_FixRate();
		pTxData <<= 1;
	}

	DRV_SPIM_FixRate();

	DRV_SPIM_DC_SET(1);
	return 0;
}


#endif
