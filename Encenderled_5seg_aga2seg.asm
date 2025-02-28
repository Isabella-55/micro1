LIST P=18F4550, F=INHX32
    #include <p18f4550.inc>

    CONFIG FOSC = HS         ; Oscilador externo (High-Speed)
    CONFIG WDT = OFF         ; Watchdog Timer desactivado
    CONFIG MCLRE = OFF        ; MCLR habilitado

    ORG 0x0000              ; Dirección de inicio del programa
    GOTO INICIO             ; Saltar a la etiqueta INICIO

    ORG 0x0008              ; Vector de interrupción (no usado en este caso)
    

    CBLOCK 0x20             ; Bloque de memoria RAM desde la dirección 0x20
        TEMP1               ; Variable para retardos largos
        TEMP2               ; Variable para retardos cortos
        TEMP3               ; Variable auxiliar
    ENDC


INICIO:
    CLRF LATB               ; Limpiar puerto B
    CLRF TRISB              ; Configurar PORTB como salida (RB0 como salida)

BUCLE:
    BSF LATB, 0             ; Encender LED en RB0
    CALL RETARDO5000        ; Esperar 5000 ms
    BCF LATB, 0             ; Apagar LED en RB0
    CALL RETARDO2000        ; Esperar 2000 ms
    GOTO BUCLE              ; Repetir el ciclo

;  RETARDO 5000 ms 
RETARDO5000:
    MOVLW D'50'            ; Cargar 50 en W (50 x 100ms = 5000ms)
    MOVWF TEMP1
L1:
    CALL RETARDO100MS      ; Llamar retardo de 100ms
    DECFSZ TEMP1, F
    GOTO L1
    RETURN

;  RETARDO 2000 ms 
RETARDO2000:
    MOVLW D'20'            ; Cargar 20 en W (20 x 100ms = 2000ms)
    MOVWF TEMP1
L2:
    CALL RETARDO100MS
    DECFSZ TEMP1, F
    GOTO L2
    RETURN

; --------------------- RETARDO 100 ms ---------------------
RETARDO100MS:
    MOVLW D'250'           ; Bucle de retardo
    MOVWF TEMP2
L3:
    MOVLW D'250'
    MOVWF TEMP3
L4:
    NOP                    ; No operación (espera mínima)
    DECFSZ TEMP3, F
    GOTO L4
    DECFSZ TEMP2, F
    GOTO L3
    RETURN

    END


