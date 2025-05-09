#ifndef I2C_H
#define	I2C_H

#define PIN_IN   1
#define PIN_OUT  0

void I2C_Master_Init(uint32_t clock);
void I2C_Master_Wait(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Repeated_Start(void);
void I2C_Master_Write(uint8_t dato);
uint8_t I2C_Master_Read(uint8_t ACK);

#endif	/* I2C_H */


