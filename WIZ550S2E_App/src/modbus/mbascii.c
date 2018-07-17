#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include "mbascii.h"
#include "mbtcp.h"
#include "timerHandler.h"
#include "uartHandler.h"

volatile uint8_t mb_state_ascii_finish;
extern uint8_t plus_count;

#define MB_SER_PDU_SIZE_MIN     3       /*!< Minimum size of a Modbus ASCII frame. */
#define MB_SER_PDU_SIZE_MAX     256     /*!< Maximum size of a Modbus ASCII frame. */
#define MB_SER_PDU_SIZE_LRC     1       /*!< Size of LRC field in PDU. */
#define MB_SER_PDU_ADDR_OFF     0       /*!< Offset of slave address in Ser-PDU. */
#define MB_SER_PDU_PDU_OFF      1       /*!< Offset of Modbus-PDU in Ser-PDU. */

typedef enum
{
	STATE_RX_IDLE,              /*!< Receiver is in idle state. */
	STATE_RX_RCV,               /*!< Frame is beeing received. */
	STATE_RX_END,
	STATE_RX_ERROR              /*!< If the frame is invalid. */
} eMBRcvState;



volatile uint8_t* pucASCIIBufferCur;
volatile uint16_t usASCIIBufferPos;

extern volatile uint8_t *pucTCPBufferCur;
extern volatile uint16_t usTCPBufferPos;

extern volatile uint8_t ucRTUBuf[];
volatile uint8_t *ucASCIIBuf = ucRTUBuf;

static eMBRcvState eRcvState;

uint8_t prvucMBCHAR2BIN( uint8_t ucCharacter )
{
	if( ( ucCharacter >= '0' ) && ( ucCharacter <= '9' ) ) {
		return ( uint8_t )( ucCharacter - '0' );
	}
	else if( ( ucCharacter >= 'A' ) && ( ucCharacter <= 'F' ) ) {
		return ( uint8_t )( ucCharacter - 'A' + 0x0A );
	}
	else {
		return 0xFF;
	}
}


uint8_t prvucMBBIN2CHAR( uint8_t ucByte )
{
    if( ucByte <= 0x09 ) {
        return ( uint8_t )( '0' + ucByte );
    }
    else if( ( ucByte >= 0x0A ) && ( ucByte <= 0x0F ) ) {
        return ( uint8_t )( ucByte - 0x0A + 'A' );
    }
    else {
        /* Programming error. */
//        assert( 0 );
    }
    return '0';
}


uint8_t prvucMBLRC( uint8_t * pucFrame, uint16_t usLen )
{
	uint8_t           ucLRC = 0;  /* LRC char initialized */

    while( usLen-- ) {
        ucLRC += *pucFrame++;   /* Add buffer byte without carry */
    }

    /* Return twos complement */
    ucLRC = ( uint8_t ) ( -( ( int8_t ) ucLRC ) );
		
    return ucLRC;
}

static bool mbASCIIPackage( uint8_t * pucRcvAddress, uint8_t ** pucFrame, uint16_t * pusLength )
{
	uint16_t count;
	for(count=0; count < usASCIIBufferPos; count++) {
		if(count%2) {
			pucASCIIBufferCur[(uint16_t)(count/2)] |= prvucMBCHAR2BIN(ucASCIIBuf[count]);
		}
		else {
			pucASCIIBufferCur[count/2] = (uint8_t)(prvucMBCHAR2BIN(ucASCIIBuf[count])<<4);
		}
	}
	if(prvucMBLRC( ( uint8_t * ) pucASCIIBufferCur, usASCIIBufferPos/2 ) == 0) {
	  *pucRcvAddress = pucASCIIBufferCur[MB_SER_PDU_ADDR_OFF];
	  *pusLength = ( uint16_t )( usASCIIBufferPos/2 - MB_SER_PDU_PDU_OFF - MB_SER_PDU_SIZE_LRC );
	  *pucFrame = ( uint8_t * ) & pucASCIIBufferCur[MB_SER_PDU_PDU_OFF];

	  return TRUE;
	}
	
	return FALSE;
}


bool MBascii2tcpFrame(void)
{
	uint8_t pucRcvAddress;
	uint16_t pusLength;
	uint8_t* ppucFrame;
	
	if(mbASCIIPackage( &pucRcvAddress, &ppucFrame, &pusLength ) != FALSE) {
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


void ASCII_Uart_RX(void)
{
	uint8_t ucByte = 0;

	/* Always read the character. */
	if(UART_read(&ucByte, 1) <= 0) return;
	
	switch ( eRcvState )
	{
		case STATE_RX_IDLE:
			usASCIIBufferPos = 0;
			if(ucByte == MB_ASCII_START) {
				eRcvState = STATE_RX_RCV;
			}
			break;
		case STATE_RX_RCV:
			if( usASCIIBufferPos < MB_SER_PDU_SIZE_MAX ) {
				if(ucByte == MB_ASCII_DEFAULT_CR) {
					eRcvState = STATE_RX_END;
				}
				else {
					ucASCIIBuf[usASCIIBufferPos++] = ucByte;
				}
			}
			else {
				eRcvState = STATE_RX_ERROR;
			}
			//IWDG_ReloadCounter();
			break;
		case STATE_RX_END:
			if(ucByte == MB_ASCII_DEFAULT_LF) {
				mb_state_ascii_finish = TRUE;
				eRcvState = STATE_RX_IDLE;
			}
			break;
		case STATE_RX_ERROR:
			break;
		default:
			break;
	}
}

