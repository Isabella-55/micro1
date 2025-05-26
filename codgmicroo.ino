#include <Wire.h>
#include "RTClib.h"
#include <WiFi.h>
#include <DHT.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <WebServer.h>

// Configuración WiFi
#define WIFI_SSID "OPPO A79 5G Mercy"
#define WIFI_PASS "12345678"

// ===== CONFIGURACIÓN DE PINES =====
// DHT11
#define DHTPIN 4     // GPIO4
#define DHTTYPE DHT11

// GPS NEO-6M (UART1)
#define GPS_RX 18    // GPIO18 (RX del ESP32 <- TX del GPS)
#define GPS_TX 19    // GPIO19 (TX del ESP32 -> RX del GPS)

// ===== INICIALIZACIÓN DE COMPONENTES =====
RTC_DS1307 rtc;
DHT dht(DHTPIN, DHTTYPE);
HardwareSerial gpsSerial(1); // UART1 para GPS
TinyGPSPlus gps;
WebServer server(80);

// Variables para valores máximos y mínimos
float tempMax = -1000, tempMin = 1000;
float humMax = -1000, humMin = 1000;
float temp = 0, hum = 0;
String gpsData = "Sin señal GPS";
String fechaHora = "--/--/-- --:--:--";
int DESFASE_MINUTOS = 10;

// Variables de control
bool sistemaActivo = true;
bool gpsValido = false;

// Interfaz web moderna (igual que antes)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Monitor Ambiental Inteligente</title>
  <style>
    /* Estilos igual que antes */
  </style>
</head>
<body>
  <!-- HTML igual que antes -->
</body>
</html>
)rawliteral";

bool testWiFi() {
  Serial.print("Conectando a WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("\nConexión WiFi exitosa!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  return true;
}

void ajustarHoraDesdePC() {
  const char *compilationDate = DATE;
  const char *compilationTime = TIME;
  
  char monthStr[4];
  int day, year;
  sscanf(compilationDate, "%s %d %d", monthStr, &day, &year);
  
  int month = 1;
  const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", 
                         "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  for (int i = 0; i < 12; i++) {
    if (strcmp(monthStr, months[i]) == 0) {
      month = i + 1;
      break;
    }
  }
  
  int hour, minute, second;
  sscanf(compilationTime, "%d:%d:%d", &hour, &minute, &second);
  DateTime ahora(year, month, day, hour, minute, second);
  ahora = ahora + TimeSpan(0, 0, DESFASE_MINUTOS, 0);
  rtc.adjust(ahora);
  
  Serial.println("RTC ajustado desde hora de compilación con compensación");
}

void actualizarDatosGPS() {
  gpsValido = false;
  gpsData = "Buscando señal GPS...";
  unsigned long inicioEspera = millis();
  const unsigned long TIEMPO_MAX_ESPERA = 30000; // 30 segundos máximo de espera

  Serial.println("\nEsperando señal GPS...");
  
  while (millis() - inicioEspera < TIEMPO_MAX_ESPERA) {
    while (gpsSerial.available()) {
      if (gps.encode(gpsSerial.read())) {
        if (gps.location.isValid()) {
          gpsData = "Lat: " + String(gps.location.lat(), 6) + 
                   ", Lon: " + String(gps.location.lng(), 6);
          gpsValido = true;
          Serial.println("\nSeñal GPS obtenida!");
          return;
        }
      }
    }
    delay(100);
    Serial.print(".");
  }

  if (!gpsValido) {
    gpsData = "No se pudo obtener señal GPS";
    Serial.println("\nTiempo de espera agotado para GPS");
  }
}

void actualizarFechaHora() {
  DateTime now = rtc.now();
  fechaHora = String(now.year()) + "/" + 
              String(now.month()) + "/" + 
              String(now.day()) + " " + 
              String(now.hour()) + ":" + 
              String(now.minute()) + ":" + 
              String(now.second());
}

void handleSensorData() {
  String json = "{";
  json += "\"temp\":" + String(temp, 1) + ",";
  json += "\"tempMax\":" + String(tempMax, 1) + ",";
  json += "\"tempMin\":" + String(tempMin, 1) + ",";
  json += "\"hum\":" + String(hum, 1) + ",";
  json += "\"humMax\":" + String(humMax, 1) + ",";
  json += "\"humMin\":" + String(humMin, 1) + ",";
  json += "\"fechaHora\":\"" + fechaHora + "\",";
  json += "\"gpsData\":\"" + gpsData + "\",";
  json += "\"sistemaActivo\":" + String(sistemaActivo ? "true" : "false");
  json += "}";
  server.send(200, "application/json", json);
}

void handleControl() {
  if (server.hasArg("estado")) {
    String estado = server.arg("estado");
    sistemaActivo = (estado == "1");
    server.send(200, "text/plain", "OK");
    Serial.println("Sistema " + String(sistemaActivo ? "activado" : "desactivado"));
  } else {
    server.send(400, "text/plain", "Falta parámetro 'estado'");
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  Serial.println("Iniciando sistema...");
  
  // 1. Conectar WiFi
  Serial.println("\n[1/4] Conectando a WiFi...");
  bool wifiConnected = testWiFi();
  
  // 2. Inicializar RTC
  Serial.println("\n[2/4] Inicializando RTC...");
  if (!rtc.begin()) {
    Serial.println("Error en RTC");
    while(1);
  }
  ajustarHoraDesdePC();
  
  // 3. Inicializar GPS
  Serial.println("\n[3/4] Inicializando GPS...");
  gpsSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);
  actualizarDatosGPS(); // Esperará hasta obtener señal o timeout
  
  // 4. Inicializar DHT
  Serial.println("\n[4/4] Inicializando sensor DHT...");
  dht.begin();

  // Configurar rutas del servidor web
  server.on("/", HTTP_GET, []() {
    server.send_P(200, "text/html", index_html);
  });
  server.on("/sensors", HTTP_GET, handleSensorData);
  server.on("/control", HTTP_GET, handleControl);
  
  server.begin();
  Serial.println("\nServidor HTTP iniciado");
  Serial.println("\n=== Sistema listo ===");
}

void loop() {
  server.handleClient();

  if (sistemaActivo) {
    // Leer sensores
    temp = dht.readTemperature();
    hum = dht.readHumidity();

    // Actualizar máximos y mínimos
    if (!isnan(temp) && !isnan(hum)) {
      if (temp > tempMax) tempMax = temp;
      if (temp < tempMin) tempMin = temp;
      if (hum > humMax) humMax = hum;
      if (hum < humMin) humMin = hum;
    }
    
    // Actualizar GPS (solo si no tenemos señal válida aún)
    if (!gpsValido) {
      actualizarDatosGPS();
    }
    actualizarFechaHora();
    
    // Mostrar datos por serial cada 2 segundos
    static unsigned long lastPrint = 0;
    if (millis() - lastPrint >= 2000) {
      lastPrint = millis();
      Serial.println("\n=== Datos actuales ===");
      Serial.println("Hora: " + fechaHora);
      Serial.printf("Temp: %.1f°C (Max: %.1f°C, Min: %.1f°C)\n", temp, tempMax, tempMin);
      Serial.printf("Hum: %.1f%% (Max: %.1f%%, Min: %.1f%%)\n", hum, humMax, humMin);
      Serial.println("GPS: " + gpsData);
      Serial.println("=====================");
    }
  } else {
    // Mostrar mensaje de sistema inactivo cada 5 segundos
    static unsigned long lastPrint = 0;
    if (millis() - lastPrint >= 5000) {
      lastPrint = millis();
      Serial.println("Sistema inactivo (en espera)");
    }
  }
  
  delay(100); // Pequeño delay para evitar sobrecarga
}