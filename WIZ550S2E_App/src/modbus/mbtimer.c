#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include "ConfigData.h"
#include "mbtimer.h"
#include "timerHandler.h"

eMBRcvState eRcvState;
volatile uint8_t mb_state_rtu_finish;

void xMBPortTimersInit( uint32_t usTim1Timerout50us)
{
	uint32_t timerFreq;

	/* Initialize 32-bit timer 1 clock */
	Chip_TIMER_Init(LPC_TIMER32_1);

	/* Timer rate is system clock rate */
	timerFreq = Chip_Clock_GetSystemClockRate();

	/* Timer setup for match and interrupt at TICKRATE_HZ */
	Chip_TIMER_Reset(LPC_TIMER32_1);

	/* Enable both timers to generate interrupts when time matches */
	Chip_TIMER_MatchEnableInt(LPC_TIMER32_1, 1);

	/* Setup prescale value on 32-bit timer to extend range */
	Chip_TIMER_PrescaleSet(LPC_TIMER32_1, 8);

	/* Setup 32-bit timer's duration (32-bit match time) */
	Chip_TIMER_SetMatch(LPC_TIMER32_1, 1, (timerFreq / usTim1Timerout50us));


	/* Setup both timers to restart when match occurs */
	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER32_1, 1);

	/* Start both timers */
	Chip_TIMER_Enable(LPC_TIMER32_1);

	/* Clear both timers of any pending interrupts */
	NVIC_ClearPendingIRQ(TIMER_32_1_IRQn);

	/* Enable both timer interrupts */
	NVIC_EnableIRQ(TIMER_32_1_IRQn);
}

void vMBPortTimersEnable( void )
{
	/* Timer setup for match and interrupt at TICKRATE_HZ */
	Chip_TIMER_Reset(LPC_TIMER32_1);
	/* Start both timers */
	Chip_TIMER_Enable(LPC_TIMER32_1);
	/* Clear both timers of any pending interrupts */
	NVIC_ClearPendingIRQ(TIMER_32_1_IRQn);
	/* Enable both timer interrupts */
	NVIC_EnableIRQ(TIMER_32_1_IRQn);
}

void vMBPortTimersDisable( void )
{
	/* Start both timers */
	Chip_TIMER_Disable(LPC_TIMER32_1);
	/* Enable both timer interrupts */
	NVIC_DisableIRQ(TIMER_32_1_IRQn);
}

void xMBRTUTimerT35Expired( void )
{
	switch ( eRcvState ) {
		/* Timer t35 expired. Startup phase is finished. */
		case STATE_RX_INIT:
			break;

		/* A frame was received and t35 expired. Notify the listener that
		* a new frame was received. */
		case STATE_RX_RCV:
			mb_state_rtu_finish = TRUE;
			break;
		
		/* An error occured while receiving the frame. */
		case STATE_RX_ERROR:
			break;

		/* Function called in an illegal state. */
		default:
			break;
	}
	vMBPortTimersDisable(  );
	eRcvState = STATE_RX_IDLE;

	//printf("tim3\r\n");
}
