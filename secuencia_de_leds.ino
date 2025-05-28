// Pines seguros para salida
const int pines[] = {5, 18, 19, 22, 23};  // GPIO15 reemplazado por GPIO5
const int numPines = sizeof(pines) / sizeof(pines[0]);

void setup() {
  for (int i = 0; i < numPines; i++) {
    pinMode(pines[i], OUTPUT);
  }
}

void loop() {
  // Encendido secuencial
  for (int i = 0; i < numPines; i++) {
    digitalWrite(pines[i], HIGH);
    delay(150);
    digitalWrite(pines[i], LOW);
    delay(150);
  }

  // Encendido en reversa
  for (int i = numPines - 1; i >= 0; i--) {
    digitalWrite(pines[i], HIGH);
    delay(150);
    digitalWrite(pines[i], LOW);
    delay(150);
  }
}
