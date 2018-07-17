#include "mbtcp.h"
#include "mbrtu.h"
#include "mbascii.h"
#include "uartHandler.h"
#include "socket.h"
#include "common.h"

extern volatile uint8_t* pucASCIIBufferCur;
extern volatile uint16_t usASCIIBufferPos;

void mbTCPtoRTU(void)
{
  if(MBtcp2rtuFrame() != FALSE)
	{
		while(usRTUBufferPos)
		{
			UART_write((uint8_t*)pucRTUBufferCur, 1);
			pucRTUBufferCur++;
			usRTUBufferPos--;
		}
	}
}

void mbRTUtoTCP(void)
{
	struct __network_info *net = (struct __network_info *)get_S2E_Packet_pointer()->network_info;
	if(MBrtu2tcpFrame() != FALSE)
	{
		switch(getSn_SR(SOCK_DATA))
		{
			case SOCK_UDP :
				sendto(SOCK_DATA, (uint8_t*)pucTCPBufferCur, usTCPBufferPos, net->remote_ip, net->remote_port);
			break;
			case SOCK_ESTABLISHED:
			case SOCK_CLOSE_WAIT:
				send(SOCK_DATA, (uint8_t*)pucTCPBufferCur, usTCPBufferPos);
			break;
			default:
			break;
	  }
	}
}

void mbTCPtoASCII(void)
{
	uint8_t ucByte;
	
	if(MBtcp2asciiFrame() != FALSE)
	{
		ucByte = MB_ASCII_START;
		UART_write(&ucByte, 1);
		while(usASCIIBufferPos)
		{
			ucByte = prvucMBBIN2CHAR((uint8_t)*pucASCIIBufferCur>>4);
			UART_write(&ucByte, 1);

			ucByte = prvucMBBIN2CHAR((uint8_t)*pucASCIIBufferCur&0x0F);
			UART_write(&ucByte, 1);

			pucASCIIBufferCur++;
			usASCIIBufferPos--;
		  }
		ucByte = MB_ASCII_DEFAULT_CR;
		UART_write(&ucByte, 1);
		ucByte = MB_ASCII_DEFAULT_LF;
		UART_write(&ucByte, 1);
	}
}

void mbASCIItoTCP(void)
{
	struct __network_info *net = (struct __network_info *)get_S2E_Packet_pointer()->network_info;

	if(MBascii2tcpFrame() != FALSE) {
		switch(getSn_SR(SOCK_DATA)) {
			case SOCK_UDP :
				sendto(SOCK_DATA, (uint8_t*)pucTCPBufferCur, usTCPBufferPos, net->remote_ip, net->remote_port);
				break;
			case SOCK_ESTABLISHED:
			case SOCK_CLOSE_WAIT:
				send(SOCK_DATA, (uint8_t*)pucTCPBufferCur, usTCPBufferPos);
				break;
			default:
				break;
		}
	}
}
