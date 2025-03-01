;CÓDIGO DE LED SOLO
#include <p18F4550.inc>


CONFIG  FOSC = INTOSCIO_EC    
CONFIG  WDT = OFF                       
CONFIG  LVP = OFF                       

LIST    P=18F4550

ORG     0x0000    ;vector de reset
GOTO    START

ORG     0x0020    :inicio del prog

START:
    
MOVLW   0x72       ;configuración del oscilador
MOVWF   OSCCON

CLRF    TRISB      ;configura el puerto B como salida
CLRF    PORTB      ;limpia el puerto B

MAIN_LOOP:
BSF     LATB, RB0  ;enciende el led en RB0
GOTO    MAIN_LOOP

END


