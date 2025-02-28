
#include "p18f4550.inc"

  CONFIG  FOSC = INTOSC_EC      ; Oscillator Selection bits (Internal oscillator, CLKO function on RA6, EC used by USB (INTCKO))
  CONFIG  WDT = OFF             ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
  CONFIG  LVP = OFF             ; Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
  CONFIG  PBADEN = OFF          ; PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
       
     LIST P=18F4550           ; Especifica el microcontrolador PIC18F4550
   
   ;Se definen variables en la RAM
    UDATA ;indica el inicio de una sección de datos no inicializados en la RAM.
CONTADOR1   RES 1      ; Reserva 1 byte en RAM para la variable CONTADOR1
CONTADOR2   RES 1      ; Reserva 1 byte en RAM para la variable CONTADOR2
CONTADOR3   RES 1      ; Reserva 1 byte en RAM para la variable CONTADOR3

    
    ORG 0x0000          ; Dirección de inicio en memoria
    GOTO Inicio         ; Salta a  Inicio cuando prende micro

    ORG 0x0020          ; Dirección donde comienza el código principal

Inicio:
   
    MOVLW 0x60          ; Carga el valor 0x60 en WREG (IRCF = 110 -> 8 MHz)
    MOVWF OSCCON        ; Configura la frecuencia del oscilador interno a 8 MHz

   
    CLRF TRISB          ; Configura PORTB como salida, pone todos los bits en 0
    CLRF LATB           ; Apaga todos los pines de PORTB inicializa en 0

MAIN_LOOP:
    BSF LATB, 0         ; Enciende el LED en RB0 pone el bit en 1 
    CALL RETARDO_5S     ; Llama a la subrutina de retardo de 5s
    BCF LATB, 0         ; Apaga el LED en RB0 
    CALL RETARDO_2S     ; Llama a la subrutina de retardo de 2s
    GOTO MAIN_LOOP      ; Repite el bucle enecendiendo y apagando el led indefinidamente

   ;Subrutina de retardo 5s
RETARDO_5S:
    MOVLW D'5'          ; Carga el valor decimal 5 en WREG (para los 5 segundos)
    MOVWF CONTADOR1     ; Guarda el valor en CONTADOR1 
RETARDO_5S_LOOP1:
    MOVLW D'200'        ; Carga el valor decimal 200 en WREG
    MOVWF CONTADOR2     ; Guarda el valor en CONTADOR2 
RETARDO_5S_LOOP2:
    MOVLW D'200'        ; Carga el valor decimal 200 en WREG
    MOVWF CONTADOR3     ; Guarda el valor en CONTADOR3 
RETARDO_5S_LOOP3:
    NOP                 ; No hace nada permite que se generar un pequeño retardo
    NOP                 ; No hace nada
    NOP                 ; No hace nada
    DECFSZ CONTADOR3, 1 ; Decrementa CONTADOR3 y si llega a 0, salta a la siguiente instrucción
    GOTO RETARDO_5S_LOOP3  ; Si CONTADOR3 no es 0, sigue repitiendo el bucle interno
    DECFSZ CONTADOR2, 1 ; Decrementa CONTADOR2 y si llega a 0, salta a la siguiente instrucción
    GOTO RETARDO_5S_LOOP2  ; Si CONTADOR2 no es 0, sigue repitiendo el bucle medio
    DECFSZ CONTADOR1, 1 ; Decrementa CONTADOR1 y si llega a 0, sale de la subrutina
    GOTO RETARDO_5S_LOOP1  ; Si CONTADOR1 no es 0, sigue repitiendo el bucle externo
    RETURN             ; Regresa al punto donde fue llamada la subrutina

   ;subrutina retardo 2s
RETARDO_2S:
    MOVLW D'2'          ; Carga el valor decimal 2 en WREG (para los 2 segundos)
    MOVWF CONTADOR1     ; Guarda el valor en CONTADOR1 (bucle externo)
RETARDO_2S_LOOP1:
    MOVLW D'200'        ; Carga el valor decimal 200 en WREG
    MOVWF CONTADOR2     ; Guarda el valor en CONTADOR2 (bucle medio)
RETARDO_2S_LOOP2:
    MOVLW D'200'        ; Carga el valor decimal 200 en WREG
    MOVWF CONTADOR3     ; Guarda el valor en CONTADOR3 (bucle interno)
RETARDO_2S_LOOP3:
    NOP                 ; No hace nada (permite generar un pequeño retardo)
    NOP                 ; No hace nada
    NOP                 ; No hace nada
    DECFSZ CONTADOR3, 1 ; Decrementa CONTADOR3 y si llega a 0, salta a la siguiente instrucción
    GOTO RETARDO_2S_LOOP3  ; Si CONTADOR3 no es 0, sigue repitiendo el bucle interno
    DECFSZ CONTADOR2, 1 ; Decrementa CONTADOR2 y si llega a 0, salta a la siguiente instrucción
    GOTO RETARDO_2S_LOOP2  ; Si CONTADOR2 no es 0, sigue repitiendo el bucle medio
    DECFSZ CONTADOR1, 1 ; Decrementa CONTADOR1 y si llega a 0, sale de la subrutina
    GOTO RETARDO_2S_LOOP1  ; Si CONTADOR1 no es 0, sigue repitiendo el bucle externo
    RETURN             ; Regresa al punto donde fue llamada la subrutina

   
    END               