#include <p18F4550.inc>

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

LIST    P=18F4550

ORG     0x0000
GOTO    START

ORG     0x0020

START:
    
MOVLW   0x72
MOVWF   OSCCON

CLRF    TRISB
CLRF    PORTB

MAIN_LOOP:
BSF     LATB, RB0
GOTO    MAIN_LOOP

END


