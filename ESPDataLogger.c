#include "UartRingbuffer.h"
#include "ESPDataLogger.h"
#include "stdio.h"
#include "string.h"
#include <stdarg.h>


void bufclr (char *buf)
{
	int len = strlen (buf);
	for (int i=0; i<len; i++) buf[i] = '\0';
}


void ESP_Init (char *SSID, char *PASSWD)
{
	char data[80];

	Ringbuf_init();

	Uart_sendstring("AT+RST\r\n");

	HAL_Delay(1000);
	Uart_flush();

	/********* AT **********/
	Uart_sendstring("AT\r\n");
	while(!(Wait_for("OK\r\n")));

	Uart_flush();


	/********* AT+CWMODE=1 **********/
	Uart_sendstring("AT+CWMODE=1\r\n");
	while (!(Wait_for("OK\r\n")));

	Uart_flush();


	/********* AT+CWJAP="SSID","PASSWD" **********/
	sprintf (data, "AT+CWJAP=\"%s\",\"%s\"\r\n", SSID, PASSWD);
	Uart_sendstring(data);
	while (!(Wait_for("GOT IP\r\n")));

	Uart_flush();

	/********* AT+CIPMUX=0 **********/
	Uart_sendstring("AT+CIPMUX=0\r\n");
	while (!(Wait_for("OK\r\n")));

	Uart_flush();

}

void ESP_Send_Data(const char* format, ...)
{
    char local_buf[200] = {0};
    char local_buf2[30] = {0};

    Uart_sendstring("AT+CIPSTART=\"TCP\",\"proven-gaps.000webhostapp.com\",80\r\n");
    while (!(Wait_for("OK\r\n")));

    va_list args;
    va_start(args, format);
    vsnprintf(local_buf, sizeof(local_buf), format, args);
    va_end(args);

    int len = strlen(local_buf);
    sprintf(local_buf2, "AT+CIPSEND=%d\r\n", len);
    Uart_sendstring(local_buf2);
    while (!(Wait_for(">")));

    Uart_sendstring(local_buf);
    while (!(Wait_for("SEND OK\r\n")));

    while (!(Wait_for("CLOSED")));

    bufclr(local_buf);
    bufclr(local_buf2);

    Ringbuf_init();
}
