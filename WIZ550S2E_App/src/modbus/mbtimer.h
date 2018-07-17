#ifndef _MBTIMER_H_
#define _MBTIMER_H_

typedef enum
{
    STATE_RX_INIT,              /*!< Receiver is in initial state. */
    STATE_RX_IDLE,              /*!< Receiver is in idle state. */
    STATE_RX_RCV,               /*!< Frame is beeing received. */
    STATE_RX_ERROR              /*!< If the frame is invalid. */
} eMBRcvState;


extern eMBRcvState eRcvState;
extern volatile uint8_t mb_state_rtu_finish;

void xMBPortTimersInit( uint32_t usTim1Timerout50us );
void vMBPortTimersEnable( void );
void vMBPortTimersDisable( void );
void xMBRTUTimerT35Expired( void );

#endif

