#ifndef DHT11_H
#define	DHT11_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
#include <stdint.h>
#include <pic18f4550.h>
#include "Config.h"

// Pines definidos para el DHT11
#define DHT11_PIN PORTBbits.RB4
#define DHT11_TRIS TRISBbits.TRISB4

// Variables globales para almacenar los datos del sensor
extern uint8_t humidity_int, humidity_dec, temp_int, temp_dec, checksum;

// Prototipos de funciones
void DHT11_Start(void);
uint8_t DHT11_CheckResponse(void);
char DHT11_ReadData(void);

#ifdef	__cplusplus
}
#endif

#endif	/* DHT11_H */

