#include <xc.h>
#include <stdint.h>
#include "Config.h"
#include "I2C.h"
// Para realizar la libreria del MODULO I2C me base de este video: https://www.youtube.com/watch?v=uAR0lRPnKVA&list=PLmX2_OhrubDChbULG3e9M6o0AQsyAJNwP&index=74
void I2C_Master_Init(uint32_t clock){
    TRISBbits.RB0 = PIN_IN;    
    TRISBbits.RB1 = PIN_IN;
    SSPSTATbits.SMP = 1;       // Frecuencia de operacion Standard velocidad 100kHz-1Mhz
    SSPCON1bits.SSPEN = 1;     // habilito los pines para I2C
    SSPCON1bits.SSPM = 0b1000; // modo maestro i2c
    // FORMULA DE DATASHEET: CLOCK = Fosc/(4*(SSPADD+1)) 
    //despejamos SSPADD
    SSPADD = (uint8_t)((_XTAL_FREQ/(4.0*clock))-1);
    SSPCON2=0x00;             //clareo el registro que tiene las configuraciones del I2C    
}

//Garantizar que la situacion para poder continuar con la escritura o lectura de datos este presente y correcta
//mientras el buffer esta lleno esperar hasta q se vacie y bits 0-5 del registro SSPCON2
//mientras esta leyendo esperar para que escriba bit 3 del registro SSPSTAT
void I2C_Master_Wait(void){  
    while( (SSPCON2 & 0b00011111) || (SSPSTAT & 0b00000100) );
}

void I2C_Start(void){
    I2C_Master_Wait();
    SSPCON2bits.SEN = 1; //mando la condicion de start
}

void I2C_Stop(void){
    I2C_Master_Wait();
    SSPCON2bits.PEN = 1; //mando la condicion de stop
}

void I2C_Repeated_Start(void){
    I2C_Master_Wait();
    SSPCON2bits.RSEN = 1; //mando la condicion de repetir start   
}

void I2C_Master_Write(uint8_t dato){
    I2C_Master_Wait();
    SSPBUF = dato;      //cargamos el dato en el buffer para enviar
}

uint8_t I2C_Master_Read(uint8_t ACK){
    uint8_t dato;
    I2C_Master_Wait();
    SSPCON2bits.RCEN = 1; //habilito la recepcion de datos
    I2C_Master_Wait();
    dato = SSPBUF;         //leo el dato de SSPBUF
    I2C_Master_Wait();
    //el que MANDA el ACK es el que RECIBE el dato
    //el maestro esta mandando ACK
    //el ACK es para indicarle si el dato llego o no llego, como el maestro recibio el dato tiene que indicar como lo recibio
    SSPCON2bits.ACKDT = (ACK)?0:1; //bit de acknowledge //condicional?TRUE:FALSE; 
    //si a=1 -> ACKDT = 0 (Acknowledge)    RECONOCIMIENTO    - EL DATO LLEGO CON EXITO
    //si a=0 -> ACKDT = 1 (No Acknowledge) NO RECONOCIMIENTO - EL DATO LLEGO CON PROBLEMAS
    
    //Y AHORA RECIEN ENVIAMOS EL ACK CON ACKEN
    SSPCON2bits.ACKEN = 1; //transmite el bit de dato de ACKDT 
    return dato;   
}
