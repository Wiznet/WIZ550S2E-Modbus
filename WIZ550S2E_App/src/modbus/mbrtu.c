#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include "timerHandler.h"
#include "uartHandler.h"
#include "mbcrc.h"
#include "mbrtu.h"
#include "mbtcp.h"
#include "mbtimer.h"
#include "ConfigData.h"

volatile uint8_t ucRTUBuf[MB_SER_PDU_SIZE_MAX];
static volatile uint16_t usRcvBufferPos;

volatile uint8_t *pucTCPBufferCur;
volatile uint16_t usTCPBufferPos;

extern uint8_t plus_count;

void eMBRTUInit( uint32_t ulBaudRate )
{
	uint32_t usTimerT35_50us;

	/* Modbus RTU uses 8 Databits. */

	/* If baudrate > 19200 then we should use the fixed timer values
	* t35 = 1750us. Otherwise t35 must be 3.5 times the character time.
	*/
	switch(ulBaudRate) {
		case baud_300:
			usTimerT35_50us = 10;
			break;
		case baud_600:
			usTimerT35_50us = 21;
			break;
		case baud_1200:
			usTimerT35_50us = 43;
			break;
		case baud_2400:
			usTimerT35_50us = 86;
			break;
		case baud_4800:
			usTimerT35_50us = 171;
			break;
		case baud_9600:
			usTimerT35_50us = 342;
			break;
		case baud_19200:
			usTimerT35_50us = 686;
			break;
		case baud_38400:
		case baud_57600:
		case baud_115200:
		case baud_230400:
			usTimerT35_50us = 1370;
			break;
	}
	xMBPortTimersInit(usTimerT35_50us);
}

static bool mbRTUPackage( uint8_t * pucRcvAddress, uint8_t ** pucFrame, uint16_t * pusLength )
{
	//	uint8_t i;
	//	for(i=0; i<usRcvBufferPos; i++){printf("%d ",ucRTUBuf[i]);}

	/* Save the address field. All frames are passed to the upper layed
	* and the decision if a frame is used is done there.
	*/
	*pucRcvAddress = ucRTUBuf[MB_SER_PDU_ADDR_OFF];

	/* Total length of Modbus-PDU is Modbus-Serial-Line-PDU minus
	* size of address field and CRC checksum.
	*/
	*pusLength = ( uint16_t )( usRcvBufferPos - MB_SER_PDU_PDU_OFF - MB_SER_PDU_SIZE_CRC );

	/* Return the start of the Modbus PDU to the caller. */
	*pucFrame = ( uint8_t * ) & ucRTUBuf[MB_SER_PDU_PDU_OFF];
	
	return TRUE;
}


bool MBrtu2tcpFrame(void)
{
	uint8_t pucRcvAddress;
	uint16_t pusLength;
	uint8_t* ppucFrame;
	
	if(mbRTUPackage( &pucRcvAddress, &ppucFrame, &pusLength ) != FALSE)
	{
		pucTCPBufferCur = ppucFrame-7;		
		
		pucTCPBufferCur[0] = mbTCPtid1;
		pucTCPBufferCur[1] = mbTCPtid2;
		
		pucTCPBufferCur[2] = 0;
		pucTCPBufferCur[3] = 0;
		
		pucTCPBufferCur[4] = ( pusLength + 1 ) >> 8U;
		pucTCPBufferCur[5] = ( pusLength + 1 ) & 0xFF;
		
		pucTCPBufferCur[6] = pucRcvAddress;	
		
		usTCPBufferPos = pusLength + 7;
		return TRUE;
	}
	return FALSE;
}


void RTU_Uart_RX(void)
{
	uint8_t ucByte;
	
	/* Always read the character. */
	if(UART_read(&ucByte, 1) <= 0) return;
 
	switch ( eRcvState ) {
		/* If we have received a character in the init state we have to
		* wait until the frame is finished.
		*/
		case STATE_RX_INIT:
			vMBPortTimersEnable(  );
			break;

		/* In the error state we wait until all characters in the
		* damaged frame are transmitted.
		*/
		case STATE_RX_ERROR:
			vMBPortTimersEnable(  );
			break;

		/* In the idle state we wait for a new character. If a character
		* is received the t1.5 and t3.5 timers are started and the
		* receiver is in the state STATE_RX_RECEIVCE.
		*/
		case STATE_RX_IDLE:
			usRcvBufferPos = 0;
			ucRTUBuf[usRcvBufferPos++] = ucByte;
			eRcvState = STATE_RX_RCV;

			//printf("%d ",ucByte);

			/* Enable t3.5 timers. */
			vMBPortTimersEnable(  );
			break;

		/* We are currently receiving a frame. Reset the timer after
		* every character received. If more than the maximum possible
		* number of bytes in a modbus frame is received the frame is
		* ignored.
		*/
		case STATE_RX_RCV:
			if( usRcvBufferPos < MB_SER_PDU_SIZE_MAX ) {
				ucRTUBuf[usRcvBufferPos++] = ucByte;
				//printf("%d ",ucByte);
			}
			else {
				eRcvState = STATE_RX_ERROR;
			}
			vMBPortTimersEnable();
			//IWDG_ReloadCounter();
			break;
	}
}

