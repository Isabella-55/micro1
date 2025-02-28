 #include "p18f4550.inc"

  CONFIG  FOSC = INTOSC_EC      ; Oscillator Selection bits (Internal oscillator, CLKO function on RA6, EC used by USB (INTCKO))
  CONFIG  WDT = OFF             ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
  CONFIG  LVP = OFF             ; Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
  CONFIG  PBADEN = OFF          ; PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
       
     LIST P=18F4550           ; Especifica el microcontrolador PIC18F455

    ORG     0x0000          ; Vector de reset
    GOTO    START           ; Salta al inicio del programa

    ORG     0x0020          ; Inicio del programa principal

START:
    CLRF    TRISB           ; Configura el puerto RB como salida
    CLRF    PORTB           ; Apaga todos los LEDs

MAIN_LOOP:
    ; Encender RB0, RB2, RB4, RB6
    MOVLW   b'01010101'     ; Cargar el patrón 01010101 en W
    MOVWF   PORTB           ; Mover el patrón al puerto RB
    CALL    DELAY           ; Llamar a la rutina de retardo

    ; Encender RB1, RB3, RB5, RB7
    MOVLW   b'10101010'     ; Cargar el patrón 10101010 en W
    MOVWF   PORTB           ; Mover el patrón al puerto RB
    CALL    DELAY           ; Llamar a la rutina de retardo

    GOTO    MAIN_LOOP       ; Repetir el ciclo

; Rutina de retardo de aproximadamente 1 segundo
DELAY:
    MOVLW   0x0F            ; Cargar el valor inicial para el retardo
    MOVWF   0x20            ; Usar el registro 0x20 como contador

DELAY_LOOP:
    DECFSZ  0x20, F         ; Decrementar el contador y saltar si es cero
    GOTO    DELAY_LOOP      ; Repetir el bucle de retardo
    RETURN                  ; Retornar de la rutina de retardo

    END                     ; Fin del programa             


