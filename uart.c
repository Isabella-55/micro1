#include "uart.h"

void Uart_Init(unsigned long baud)
{
    unsigned int vx;
    TRISCbits.RC6 = 0;
    TRISCbits.RC7 = 1;
    TXSTA = 0x24;
    RCSTA = 0x90;
    BAUDCON = 0x00;
    BAUDCONbits.BRG16 = 1;
    vx = (unsigned int)(_XTAL_FREQ/(baud*4))-1;
    SPBRG = vx & 0x00FF;
    SPBRGH = vx >> 8;
}

__bit Uart_Available(void)
{
    return (__bit)PIR1bits.RCIF ? 1 : 0;
}

void Uart_Send_Char(char data)
{
    while(TXSTAbits.TRMT == 0);
    TXREG = data;
}

void Uart_Send_String(char *str)
{
    while(*str != '\0')
    {
        Uart_Send_Char(*str++);
    }
}

char Uart_Read(void)
{
    while(PIR1bits.RCIF == 0);
    if(RCSTAbits.OERR == 1)
    {
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;
    }
    PIR1bits.RCIF = 0;
    return RCREG;
}

void Uart_Read_String(char* buf, int s_buf)
{
    int cont_buf = 0;
    char c;
    do
    {
        c = Uart_Read();
        buf[cont_buf++] = c;
        if(cont_buf >= s_buf) break;
    }while(c != '\n');
    buf[cont_buf-2] = '\0';
    cont_buf = 0;
}

void Uart_Read_Substring(char* cp, char* vc, int inc, int fn)
{
    int pt = 0;
    for(int lt=inc; lt<=fn; lt++)
    {
        vc[pt] = cp[lt];
        pt++;
    }
    vc[pt] = '\0';
    pt = 0;
}

void Uart_Read_StringUntil(char stop_c, char* buf, int st_size)
{
    unsigned int rx_cont = 0;
    while(*(buf-1) != stop_c)
    {
        rx_cont++;
        *buf++ = Uart_Read();
        if(rx_cont >= st_size) break;
    }
    *--buf = 0;
}
