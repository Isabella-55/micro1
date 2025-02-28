PROCESSOR 18F4550
#INCLUDE <P18F4550.INC>

CONFIG  FOSC = INTOSC_EC      ; Oscillator Selection bits (Internal oscillator, HS oscillator used by USB (INTHS))
CONFIG  WDT = OFF             ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
CONFIG  LVP = OFF    

ORG 0x00         ; Dirección de inicio
GOTO MAIN        ; Saltar a la rutina principal

ORG 0x08         ; Vector de interrupción
RETFIE

MAIN:
    CLRF TRISB   ; Configura todo el puerto B como salida
    BSF PORTB, 0 ; Enciende el LED en RB0

LOOP:
    GOTO LOOP    ; Mantenerse en un bucle infinito

END
