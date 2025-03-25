/*
 * File:   Main.c
 * Author: Electro C
 *
 * Created on 15 de junio de 2021, 09:35 AM
 */
#define RS LATDbits.LATD5
#define EN LATDbits.LATD4
#define D4 LATDbits.LATD0
#define D5 LATDbits.LATD1
#define D6 LATDbits.LATD2
#define D7 LATDbits.LATD3

// PIC18F4550 Configuration Bit Settings

// CONFIG1L
#pragma config PLLDIV = 1       // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config USBDIV = 1       // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)

// CONFIG1H
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator (HS))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = ON         // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown-out Reset Voltage bits (Minimum setting 2.05V)
#pragma config VREGEN = OFF     // USB Voltage Regulator Enable bit (USB voltage regulator disabled)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = ON      // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit (ICPORT disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)

#define _XTAL_FREQ 8000000
#include <xc.h>
#include "lcd.h"
#include <stdio.h>

void main(void) {
    
    TRISD = 0x00;
    LATD = 0x00;
    TRISA = 0xFF;
    ADCON1 = 0b00001110;
    ADCON0 = 0b00000001; 
    ADCON2 = 0b00010001;
    float temp = 0;
    char temperatura [4];
    Lcd_Init();
    Lcd_Clear();
    Lcd_Set_Cursor(1,2);
    Lcd_Write_String("Temperatura:");
    while(1){
        
        ADCON0bits.GO_DONE = 1;
        while(ADCON0bits.GO_DONE == 1){
        }
        temp = (ADRESH*150)/77;
        sprintf(temperatura,"%.2f",temp);
        Lcd_Set_Cursor(2,6);
        Lcd_Write_String(temperatura);
        Lcd_Set_Cursor(2,12);
        Lcd_Write_String("C");
        __delay_ms(1000);
        
    return;
    }
}

/*
 En este caso usamos un cristal de 8MHz
 * 
 * Entonces:
 *  1/8MHz = 125ns
 * TADmin = 0.7us
 * Entonces debemos multiplicar nuestro periodo por una potencia de dos para tener un TAD mayor a 0.7us
 * (125ns)(8) = 1us, entonces debemos tener un TAD = 1us //// 8 = ADCS2:ADCS0
 * 2.45us < xTAD
 * TACQ = 4TAD = 4us
 * 
 */

/*
 Sensor LM35
 * +10mV y -10mV por cada aumento o disminución de un grado centígrado
 * Temp Máx del sensor es 150°C
 * Entonces 150°(10mV) = 1.5V
 * 
 * 2^10-1 = 1023 bits 
 * 
 * Pero en nuestro caso solo usaremos los 8 bits del ADRESH ya que justificamos a la izquierda
 * Entonces:
 * 
 * 2^8-1 = 255 bits
 * Como el LM35 será alimentado con 5V entonces
 * 
 * 255 bits = 5V
 * 
 * Aplicamos regla de 3:
 * 255 bits <===========> 5V
 * ¿? <=================> 1.5V
 * 
 * 77 bits = 1.5V (bits máximos a usar) 
 * 
 * Por lo tanto
 * 
 * 77 bits <==============> 150 °C
 * ADRESH <===============> temp
 * 
 * temp = (ADRESH*150)/77
 */