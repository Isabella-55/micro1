#include "p18f4550.inc"


 CONFIG  FOSC = INTOSC_HS      ; Oscillator Selection bits (Internal oscillator, HS oscillator used by USB (INTHS))
 CONFIG  WDT = OFF             ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
 CONFIG  LVP = OFF    

ORG 0x00

    CLRF PORTB      ; Limpiar puerto B
    BCF TRISB, 0    ; Configurar RB0 como salida

BUCLE:
    BSF PORTB, 0    ; Encender LED
    CALL RETARDO    ; Esperar 1 segundo

    BCF PORTB, 0    ; Apagar LED
    CALL RETARDO    ; Esperar 1 segundo
   

    GOTO BUCLE      ; Repetir indefinidamente

; --- Retardo aproximado de 1 segundo ---
RETARDO:
    MOVLW 0xFF
    MOVWF C1
R1:
    MOVLW 0xFF
    MOVWF C2
R2:
    DECFSZ C2, F
    GOTO R2
    DECFSZ C1, F
    GOTO R1
    RETURN

C1 EQU 0x20
C2 EQU 0x21

END

