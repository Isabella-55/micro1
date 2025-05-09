#ifndef DS1307_H
#define	DS1307_H

uint8_t BCD_a_Decimal(uint8_t numero); //Función que convierte un número BCD a decimal.
uint8_t Decimal_a_BCD(uint8_t numero); //Función que convierte un número decimal a BCD. 
void leerRTC(uint8_t *hora, uint8_t *minutos, uint8_t *segundos, uint8_t *dia, uint8_t *mes, uint8_t *ano);
void escribirRTC(uint8_t hor, uint8_t min, uint8_t seg, uint8_t dia, uint8_t mes, uint8_t ano);

#endif	/* DS1307_H */


