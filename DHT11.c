#include "DHT11.h"

uint8_t humidity_int, humidity_dec, temp_int, temp_dec, checksum;

void DHT11_Start() {
    DHT11_TRIS = 0;             // Configurar como salida
    LATBbits.LATB0 = 0;         // Poner en bajo
    __delay_ms(18);             // Mantener por al menos 18ms
    LATBbits.LATB0 = 1;         // Subir la línea
    __delay_us(20);
    DHT11_TRIS = 1;             // Configurar como entrada
}

uint8_t DHT11_CheckResponse() {
    while(DHT11_PIN & 1);        
    while(!(DHT11_PIN & 1));     
    while(DHT11_PIN & 1);       
    return 1;
}

char DHT11_ReadData() {
    char i, data = 0;
    for (i = 0; i < 8; i++) {
        while (!(DHT11_PIN & 1));   // Esperar pulso de inicio
        __delay_us(30);
        if (DHT11_PIN & 1)
            data = (data << 1) | 1;
        else
            data = data << 1;
        while (DHT11_PIN & 1);     // Esperar fin de pulso
    }
    return data;
}
