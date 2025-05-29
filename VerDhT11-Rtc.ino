#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>
#include <Wire.h>
#include <RTClib.h>

#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
RTC_DS1307 rtc;
WebServer server(80);

const char* ssid = "Wokwi-GUEST";
const char* password = "";

void setup() {
  Serial.begin(115200);
  dht.begin();
  Wire.begin();

  if (!rtc.begin()) {
    Serial.println("No se encontró el módulo RTC");
  }

  WiFi.begin(ssid, password);
  Serial.print("Conectando");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConectado a WiFi");
  Serial.println(WiFi.localIP());

  server.on("/", []() {
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();
    DateTime now = rtc.now();

    String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    html += "<meta http-equiv='refresh' content='10'>";
    html += "<style>";
    html += "body { font-family: Arial, sans-serif; text-align: center; background-color: #f4f4f4; }";
    html += "h1 { color: #333; }";
    html += "p { font-size: 1.2em; margin: 10px; }";
    html += "</style></head><body>";
    html += "<h1>Monitoreo Ambiental</h1>";
    html += "<p>Temperatura: " + String(temp) + " °C</p>";
    html += "<p>Humedad: " + String(hum) + " %</p>";
    html += "<p>Hora: " + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()) + "</p>";
    html += "<p>Fecha: " + String(now.day()) + "/" + String(now.month()) + "/" + String(now.year()) + "</p>";
    html += "</body></html>";

    server.send(200, "text/html", html);
  });

  server.begin();
}

void loop() {
  server.handleClient();
}
