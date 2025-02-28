
#include "p18F4550.inc"


  CONFIG  FOSC = INTOSC_EC      ; Oscillator Selection bits (Internal oscillator, CLKO function on RA6, EC used by USB (INTCKO))
  CONFIG  WDT = OFF             ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
  CONFIG  LVP = OFF             ; Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
  CONFIG  PBADEN = OFF          ; PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)

 
    LIST    P=18F4550           ; se especifica el micro con el que se trabaja

    ORG     0x0000              ; direccion de incio en memoria
    GOTO    Inicio              ; va a inicio cuando se prende el micro 

    ORG     0x0020              ; ubicacion donde comienza el codigo principal 

Inicio:
    ; Configurar el oscilador interno a 8 MHz
    MOVLW   0x60                ; carga el valor 0x60 en registro W, 0x60 configura el oscilador a 8MHz 
    MOVWF   OSCCON              ; Mueve valor d W s OSCCON configurando velocidad de reloj interno

    ; Configurar PORTB como salida
    CLRF    TRISB               ; pone en 0 todod los bits de TRISB
    CLRF    LATB                ; Apagar todos los pines de PORTB

Encender_LED:
    BSF     LATB, 0             ; Establecer RB0 en alto (encender LED)

Bucle_Infinito:
	goto     Bucle_Infinito      ; Mantenerse en un bucle infinito

    END
