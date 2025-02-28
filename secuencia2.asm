LIST P=18F4550
#include <p18f4550.inc>

CONFIG  FOSC = INTOSC_EC      
CONFIG  WDT = OFF            
CONFIG  LVP = OFF 

ORG 0x00

INIT:
    CLRF PORTB        ; Apagar todos los LEDs en PORTB
    CLRF LATB         ; Limpiar registro de salida
    CLRF TRISB        ; Configurar PORTB como salida
    MOVLW 0x01        ; Empezar con el LED en RB0 (00000001)
    MOVWF LED_SEQ     ; Guardar en LED_SEQ

MAIN_LOOP:
    CALL LED_SHIFT    ; Llamar rutina de desplazamiento
    GOTO MAIN_LOOP    ; Repetir indefinidamente

; Rutina para el desplazamiento de LEDs 
LED_SHIFT:
    MOVF LED_SEQ, W   ; Cargar LED_SEQ en W
    MOVWF PORTB       ; Enviar valor a los LEDs
    CALL DELAY        ; Esperar 1 segundo
    RLNCF LED_SEQ, F  ; Rotar el bit a la izquierda (mueve el LED)
    BTFSC LED_SEQ, 8  ; Si el bit 7 (RB7) se enciende, reiniciar
    MOVLW 0x01        ; Reiniciar secuencia
    BTFSC LED_SEQ, 8  ; Si no, mantener
    MOVWF LED_SEQ
    RETURN

; Retardo de 1 segundo 
DELAY:
    MOVLW 0xFF
    MOVWF COUNTER1
DELAY_LOOP1:
    MOVLW 0xFF
    MOVWF COUNTER2
DELAY_LOOP2:
    DECFSZ COUNTER2, F
    GOTO DELAY_LOOP2
    DECFSZ COUNTER1, F
    GOTO DELAY_LOOP1
    RETURN

LED_SEQ  EQU 0x20  ; Registro temporal para la secuencia de LEDs
COUNTER1 EQU 0x21  ; Variables para el retardo
COUNTER2 EQU 0x22

END
