;CÓDIGO DE SECUENCIA DE LEDS
; PIC18F4550 

#include "p18F4550.inc"

  CONFIG  FOSC = INTOSCIO_EC    
  CONFIG  WDT = OFF                       
  CONFIG  LVP = OFF                       

    LIST    P=18F4550           

    ORG     0x0000              ; vector de reset
    GOTO    START               

    ORG     0x0020              ; inicio del código principal

START:
    MOVLW   0x72
    MOVWF   OSCCON              ; configura el oscilador interno a 8MHz
    
    CLRF    TRISB               ; configura el puerto B como salida
    CLRF    PORTB               ; limpia el puerto B (apaga todos los LEDs)

MAIN_LOOP:
    MOVLW   0xFF               ; carga 0xFF en W (todos los bits en 1)
    MOVWF   LATB               ; enciende todos los LEDs en PORTB
    CALL    DELAY              ;llama a la rutina de retardo (1s)
    
    MOVLW   0x00               ; carga 0x00 en W (todos los bits en 0)
    MOVWF   LATB               ; apaga todos los LEDs en PORTB
    CALL    DELAY              ;llama a la rutina de retardo (1s)


    GOTO    MAIN_LOOP           ;repite el ciclo


DELAY
  MOVLW   6
  MOVWF   0x23                 ; contador externo para repetir el retardo

LOOP_DELAY:
  CALL    RETARDO
  DECFSZ  0x23, F
  GOTO    LOOP_DELAY
  RETURN 

RETARDO:
    MOVLW   0xD0
    MOVWF   0x21               ; 1er contador interno

REP_1:
    MOVLW   0xD0
    MOVWF   0x22               ; sgn contador interno

REP_2:
    DECFSZ  0x22, F
    GOTO    REP_2
    DECFSZ  0x21, F
    GOTO    REP_1
    RETURN

    END                         ; 

;se encienden todos y luego se apagan
;delay de un segundo
