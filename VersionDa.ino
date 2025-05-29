#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>
#include <TinyGPS++.h>
#include <Wire.h>
#include <RTClib.h>

// --- Pines y constantes ---
#define DHTPIN 4
#define DHTTYPE DHT11
#define GPS_RX 16
#define GPS_TX 17

// --- Objetos ---
DHT dht(DHTPIN, DHTTYPE);
RTC_DS1307 rtc;
TinyGPSPlus gps;
HardwareSerial gpsSerial(1);
WebServer server(80);

// --- Red WiFi ---
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// --- Funciones ---
void conectarWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("\nConectado a WiFi. IP: " + WiFi.localIP().toString());
}

String obtenerHTML() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  DateTime now = rtc.now();

  String html = "<html><head><meta http-equiv='refresh' content='10'/><title>Monitor</title></head><body>";
  html += "<h1>Monitor Ambiental</h1>";
  html += "<p><b>Temperatura:</b> " + String(t) + " °C</p>";
  html += "<p><b>Humedad:</b> " + String(h) + " %</p>";
  html += "<p><b>Hora:</b> " + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()) + "</p>";
  html += "<p><b>Fecha:</b> " + String(now.day()) + "/" + String(now.month()) + "/" + String(now.year()) + "</p>";

  if (gps.location.isValid()) {
    html += "<p><b>Latitud:</b> " + String(gps.location.lat(), 6) + "</p>";
    html += "<p><b>Longitud:</b> " + String(gps.location.lng(), 6) + "</p>";
  } else {
    html += "<p><b>GPS:</b> Sin señal</p>";
  }

  html += "</body></html>";
  return html;
}

void manejarWeb() {
  server.send(200, "text/html", obtenerHTML());
}

void configurarSensores() {
  dht.begin();
  gpsSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);
  Wire.begin();
  
  if (!rtc.begin()) {
    Serial.println("No se detectó el RTC.");
  }
}

void setup() {
  Serial.begin(115200);
  configurarSensores();
  conectarWiFi();

  server.on("/", manejarWeb);
  server.begin();
}

void loop() {
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }
  server.handleClient();
}
