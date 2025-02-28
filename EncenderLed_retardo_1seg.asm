LIST P=18F4550, F=INHX32
    #include <p18f4550.inc>

CONFIG  FOSC = INTOSC_EC      
CONFIG  WDT = OFF            
CONFIG  LVP = OFF 

    ORG 0x0000              ; Dirección de inicio del programa
    GOTO INICIO             ; Saltar a la etiqueta INICIO

    ORG 0x0008              ; Vector de interrupción (no usado en este caso)
    
;  DEFINICIÓN DE VARIABLES 
    CBLOCK 0x20             ; Bloque de memoria RAM desde la dirección 0x20
        TEMP1               ; Variable para retardos largos
        TEMP2               ; Variable auxiliar
    ENDC

; PROGRAMA PRINCIPAL 
INICIO:
    CLRF LATB               ; Limpiar puerto B
    CLRF TRISB              ; Configurar PORTB como salida (RB0 como salida)

BUCLE:
    BSF LATB, 0             ; Encender LED en RB0
    CALL RETARDO1000        ; Esperar 1000 ms
    BCF LATB, 0             ; Apagar LED en RB0
    CALL RETARDO1000        ; Esperar 1000 ms
    GOTO BUCLE              ; Repetir el ciclo

;  RETARDO 1000 ms 
RETARDO1000:
    MOVLW D'10'             ; Cargar 10 en W (10 x 100ms = 1000ms)
    MOVWF TEMP1
L1:
    CALL RETARDO100MS       ; Llamar retardo de 100ms
    DECFSZ TEMP1, F
    GOTO L1
    RETURN

;  RETARDO 100 ms 
RETARDO100MS:
    MOVLW D'250'           ; Bucle de retardo
    MOVWF TEMP2
L2:
    NOP                    ; No operación (espera mínima)
    DECFSZ TEMP2, F
    GOTO L2
    RETURN

    END
