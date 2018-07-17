#ifndef _MBTCP_H_
#define _MBTCP_H_

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

extern uint8_t mbTCPtid1;
extern uint8_t mbTCPtid2;
	
extern volatile uint8_t *pucRTUBufferCur;
extern volatile uint16_t usRTUBufferPos;

bool MBudp2rtuFrame(void);
bool MBtcp2rtuFrame(void);

#endif


