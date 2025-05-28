void setup() {
  // Inicializa el pin digital GPIO18 como salida
  pinMode(18, OUTPUT);
}

void loop() {
  digitalWrite(18, HIGH); // Enciende el LED
  delay(500);             // Espera 500 milisegundos
  digitalWrite(18, LOW);  // Apaga el LED
  delay(500);             // Espera 500 milisegundos
}