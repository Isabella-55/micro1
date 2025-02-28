; PIC18F4550 Configuration Bit Settings

#include "p18F4550.inc"

; CONFIG1L
  CONFIG  PLLDIV = 1            
  CONFIG  CPUDIV = OSC1_PLL2    
  CONFIG  USBDIV = 1            

; CONFIG1H
  CONFIG  FOSC = INTOSCIO_EC    
  CONFIG  FCMEN = OFF           
  CONFIG  IESO = OFF            

; CONFIG2L
  CONFIG  PWRT = OFF            
  CONFIG  BOR = OFF             
  CONFIG  BORV = 3              
  CONFIG  VREGEN = OFF          

; CONFIG2H
  CONFIG  WDT = OFF             
  CONFIG  WDTPS = 32768         

; CONFIG3H
  CONFIG  CCP2MX = ON           
  CONFIG  PBADEN = OFF          
  CONFIG  LPT1OSC = OFF         
  CONFIG  MCLRE = OFF           

; CONFIG4L
  CONFIG  STVREN = ON           
  CONFIG  LVP = OFF             
  CONFIG  ICPRT = OFF           
  CONFIG  XINST = OFF           

; CONFIG5L
  CONFIG  CP0 = OFF             
  CONFIG  CP1 = OFF             
  CONFIG  CP2 = OFF             
  CONFIG  CP3 = OFF             

; CONFIG5H
  CONFIG  CPB = OFF             
  CONFIG  CPD = OFF             

; CONFIG6L
  CONFIG  WRT0 = OFF            
  CONFIG  WRT1 = OFF            
  CONFIG  WRT2 = OFF            
  CONFIG  WRT3 = OFF            

; CONFIG6H
  CONFIG  WRTC = OFF            
  CONFIG  WRTB = OFF            
  CONFIG  WRTD = OFF            

; CONFIG7L
  CONFIG  EBTR0 = OFF           
  CONFIG  EBTR1 = OFF           
  CONFIG  EBTR2 = OFF           
  CONFIG  EBTR3 = OFF           

; CONFIG7H
  CONFIG  EBTRB = OFF           

    LIST    P=18F4550           ; Especifica el microcontrolador

    ORG     0x0000              ; Vector de reset
    GOTO    START               ; Salta al inicio del programa

    ORG     0x0020              ; Inicio del código principal

START:
    MOVLW   0x72
    MOVWF   OSCCON              ; Configura el oscilador interno a 8MHz
    
    CLRF    TRISB               ; Configura el puerto B como salida
    CLRF    PORTB               ; Limpia el puerto B (apaga todos los LEDs)

MAIN_LOOP:
    BSF     LATB, 0             ; Enciende el LED en RB0
    CALL    DELAY               ; Llama a la rutina de retardo (1s)
    CALL    DELAY               ; Llama a la rutina de retardo (1s)
    CALL    DELAY               ; Llama a la rutina de retardo (1s)
    CALL    DELAY               ; Llama a la rutina de retardo (1s)
    CALL    DELAY               ; Llama a la rutina de retardo (1s)
    BCF     LATB, 0             ; Apaga el LED en RB0
    CALL    DELAY               ; Llama a la rutina de retardo (1s)
    CALL    DELAY               ; Llama a la rutina de retardo (1s)
    GOTO    MAIN_LOOP           ; Repite el ciclo

; ---------------------------
; Retardo optimizado para ~1 segundo
DELAY
  MOVLW   6
  MOVWF   0x23                 ; Contador externo para repetir el retardo

LOOP_DELAY:
  CALL    RETARDO
  DECFSZ  0x23, F
  GOTO    LOOP_DELAY
  RETURN 

RETARDO:
    MOVLW   0xD0
    MOVWF   0x21               ; Primer contador interno

REP_1:
    MOVLW   0xD0
    MOVWF   0x22               ; Segundo contador interno

REP_2:
    DECFSZ  0x22, F
    GOTO    REP_2
    DECFSZ  0x21, F
    GOTO    REP_1
    RETURN

    END                         ; Fin del programa
