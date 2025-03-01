; PIC18F4550 C
#include "p18F4550.inc"

  CONFIG  FOSC = INTOSCIO_EC    
  CONFIG  WDT = OFF             
  CONFIG  LVP = OFF             
                       

    LIST    P=18F4550           ; especifica el microcontrolador

    ORG     0x0000              ; vector de reset
    GOTO    START               ; salta al inicio del programa

    ORG     0x0020              ; inicio del código principal

START:
    MOVLW   0x72
    MOVWF   OSCCON              ; configura el oscilador interno a 8MHz
    
    CLRF    TRISB               ; configura el puerto B como salida
    CLRF    PORTB               ; limpia el puerto B (apaga todos los LEDs)

MAIN_LOOP:
    BSF     LATB, 0             ; enciende el LED en RB0
    CALL    DELAY               ; llama a la rutina de retardo (1s)
    CALL    DELAY               ; llama a la rutina de retardo (1s)
    CALL    DELAY               ; llama a la rutina de retardo (1s)
    CALL    DELAY               ; llama a la rutina de retardo (1s)
    CALL    DELAY               ; llama a la rutina de retardo (1s)
    BCF     LATB, 0             ; apaga el LED en RB0
    CALL    DELAY               ; llama a la rutina de retardo (1s)
    CALL    DELAY               ; llama a la rutina de retardo (1s)
    GOTO    MAIN_LOOP           ; repite el ciclo

; retardo para 1 seg
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
    MOVWF   0x22               ; segnd contador interno

REP_2:
    DECFSZ  0x22, F
    GOTO    REP_2
    DECFSZ  0x21, F
    GOTO    REP_1
    RETURN

    END                         
