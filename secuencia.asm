LIST P=18F4550
    #include <p18f4550.inc>

CONFIG  FOSC = INTOSC_EC      
CONFIG  WDT = OFF            
CONFIG  LVP = OFF 

    ORG 0x00

    CLRF PORTB        ; Apagar todos los LEDs en PORTB
    CLRF LATB         ; Limpiar registro de salida
    CLRF TRISB        ; Configurar PORTB como salida

BUCLE:
    MOVLW 0x01        ; Empezar con el LED en RB0 (00000001)
    MOVWF TEMP        ; Guardar en TEMP

CICLO:
    MOVF TEMP, W      ; Cargar TEMP en W
    MOVWF PORTB       ; Enviar valor a los LEDs
    CALL RETARDO      ; Esperar 1 segundo
    RLCF TEMP, F       ; Rotar el bit a la izquierda (mueve el LED)
    BTFSC TEMP, 8     ; Si el bit 7 (RB7) se enciende, detenerse y reiniciar
    GOTO BUCLE        ; Reiniciar la secuencia
    GOTO CICLO        ; Continuar con el siguiente LED

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

TEMP EQU 0x20  ; Registro temporal para la secuencia
C1   EQU 0x21  ; Variables para el retardo
C2   EQU 0x22

END





