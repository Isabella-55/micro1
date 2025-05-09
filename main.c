#include <xc.h>
#include <pic18f4550.h>
#include <stdio.h>
#include "LCD.h"
#include "Config.h"
#include "DHT11.h"
#include "uart.h"
#include "nmea_gps.h"
#include "DS1307.h"
#include "I2C.h"

// Configuración de bits del sistema (ajústalo si no está en tu archivo "Config.h")
#pragma config FOSC = INTOSC_EC  // Oscilador interno
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config PBADEN = OFF

// Prototipos
void Init_System(void);
void Show_GPS_Data(void);
void Show_GPS_Alt(void);
void Show_DHT11_Data(void);
void Show_RTC_Data(void);

// Inicialización general del sistema
void Init_System(void) {
    OSCCON = 0x72;               // Oscilador interno a 8 MHz
    ADCON1 = 0x0F;               // Todo como digital

    TRISBbits.TRISB0 = 1;        // Pin del DHT11 como entrada
    TRISBbits.TRISB2 = 1;        // RB2 como entrada (INT2)
    TRISDbits.TRISD0 = 0;        // RD0 como salida (LED)
    LATDbits.LATD0 = 0;          // LED apagado

    Uart_Init(9600);             // UART para Bluetooth
    I2C_Master_Init(100000);     // I2C para RTC
    LCD_Init();                  // Inicializa la LCD
    LCD_Clear();

    // Configuración de interrupción INT2 en RB2
    INTCON3bits.INT2IE = 1;      // Habilita interrupción INT2
    INTCON3bits.INT2IF = 0;      // Limpia la bandera de INT2
    INTCON2bits.INTEDG2 = 0;     // Flanco de bajada
    INTCONbits.PEIE = 1;         // Habilita interrupciones periféricas
    INTCONbits.GIE = 1;          // Habilita interrupciones globales
}

// Funciones de visualización
void Show_GPS_Data(void) {
    LCD_Clear();
    LCD_String_xy(0, 0, "Lat: 0.00");
    LCD_String_xy(1, 0, "Lon: 0.00");
}

void Show_GPS_Alt(void){
    LCD_Clear();
    LCD_String_xy(0, 0, "Alt: 0.00");
}

void Show_DHT11_Data(void) {
    char buffer[16];
    unsigned char humidity_int, humidity_dec;
    unsigned char temp_int, temp_dec;
    unsigned char checksum;

    DHT11_Start();

    if (DHT11_CheckResponse()) {
        humidity_int = DHT11_ReadData();
        humidity_dec = DHT11_ReadData();
        temp_int = DHT11_ReadData();
        temp_dec = DHT11_ReadData();
        checksum = DHT11_ReadData();

        if ((humidity_int + humidity_dec + temp_int + temp_dec) == checksum) {
            LCD_Clear();
            sprintf(buffer, "RH: %d.%d %%", humidity_int, humidity_dec);
            LCD_String_xy(0, 0, buffer);

            sprintf(buffer, "TMP: %d.%d", temp_int, temp_dec);
            LCD_String_xy(1, 0, buffer);
            LCD_Char(0xdf);  // Símbolo de grados
            LCD_Char('C');
        } else {
            LCD_Clear();
            LCD_String_xy(0, 0, "Checksum Error");
        }
    } else {
        LCD_Clear();
        LCD_String_xy(0, 0, "DHT11 sin resp");
    }
}

void Show_RTC_Data(void){
    unsigned char h, m, s, d, mo, y;
    char buffer[17];
    
    leerRTC(&h, &m, &s, &d, &mo, &y);
        
    sprintf(buffer, "Fecha:%02d/%02d/20%02d", d, mo, y);
    LCD_String_xy(0, 0, buffer);

    sprintf(buffer, "Hora: %02d:%02d:%02d", h, m, s);
    LCD_String_xy(1, 0, buffer);
}

// ISR para INT2
void __interrupt() ISR(void) {
    if (INTCON3bits.INT2IF) {
        LATDbits.LATD0 ^= 1;           // Cambia el estado del LED
        INTCON3bits.INT2IF = 0;        // Limpia la bandera
    }
}
int Read_HC05(void);  // Prototipo de la función

// Función principal
void main(void) {
    Init_System();
    LCD_Clear();
    LCD_String_xy(0, 0, "Sistema iniciado");
    LCD_String_xy(1, 0, "Esperando datos");
    __delay_ms(2000);     // Mostrar el mensaje por 2 segundos

    int keepReading = 1;
    int lastSensor = 1;

    while (1) {
        int command = Read_HC05();
        if (command != -1) {
            keepReading = command;
        }

        if (keepReading == 1) {
            LCD_Clear();

            if (lastSensor == 1) {
                Show_DHT11_Data();
                lastSensor = 2;
            } else if (lastSensor == 2) {
                Show_RTC_Data();
                lastSensor = 3;
            } else if (lastSensor == 3) {
                Show_GPS_Data();
                lastSensor = 0;
            } else {
                Show_GPS_Alt();
                lastSensor = 1;
            }

            __delay_ms(1000);
        }
    }
}

