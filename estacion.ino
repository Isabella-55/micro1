#include "arduino_secrets.h"

#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include "RTClib.h"  // Biblioteca para el RTC DS1307

// -------- CONFIGURACIÃN DE LA RED WiFi --------
const char* ssid = "NOMBRE_DE_LA_RED";
const char* password = "CONTRASEÃA";

// -------- CONFIGURACIÃN DEL SENSOR DHT --------
#define DHTPIN 27
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// -------- CONFIGURACIÃN DEL GPS --------
#define RX_PIN 16
#define TX_PIN 17
HardwareSerial SerialGPS(1);
TinyGPSPlus gps;

// -------- CONFIGURACIÃN DEL RTC DS1307 --------
RTC_DS1307 rtc;

// -------- VARIABLES GLOBALES --------
WebServer server(80);
float tempMax = -1000, tempMin = 1000;
float humMax = -1000, humMin = 1000;
float temp = 0, hum = 0;
double latitude = 0, longitude = 0;
String fecha = "--/--/--", hora = "--:--:--";
int satelites = 0;
float altitud = 0;
bool useRTC = false;  // Para controlar si usamos el RTC o el GPS para la hora

// -------- HTML CON INTERFAZ MEJORADA --------
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>EstaciÃ³n MeteorolÃ³gica con GPS y RTC</title>
  <style>
    html { font-family: Arial; text-align: center; background-color: #f2f2f2; }
    h2 { font-size: 2.2rem; margin-bottom: 20px; color: #2c3e50; }
    .container { display: flex; flex-wrap: wrap; justify-content: center; gap: 30px; margin: 20px; }
    .panel { background: white; border-radius: 10px; padding: 20px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); width: 300px; }
    .thermo-block { margin-bottom: 20px; }
    .bar-wrapper { display: flex; align-items: flex-end; gap: 10px; margin: 15px 0; }
    .bar-container {
      position: relative; width: 60px; height: 250px;
      background: linear-gradient(to top, #0000ff, #00ffff, #00ff00, #ffff00, #ff0000);
      border-radius: 30px; overflow: hidden; border: 2px solid #999;
    }
    .bar-mask {
      position: absolute; top: 0; width: 100%;
      background: rgba(0, 0, 0, 0.5); transition: height 0.5s;
    }
    .scale { display: flex; flex-direction: column; justify-content: space-between; height: 250px; }
    .label { font-size: 1.2rem; font-weight: bold; margin-bottom: 5px; }
    .value { font-size: 1.8rem; margin: 10px 0; color: #2c3e50; }
    .minmax { font-size: 0.9rem; color: #7f8c8d; }
    .gps-info { margin-top: 15px; text-align: left; }
    .gps-value { font-weight: bold; color: #2980b9; }
    .status { margin-top: 20px; padding: 10px; border-radius: 5px; background: #f8f9fa; }
    .map-link { margin-top: 10px; }
    a { color: #3498db; text-decoration: none; }
    a:hover { text-decoration: underline; }
    .rtc-info { margin-top: 10px; font-size: 0.9rem; color: #666; }
  </style>
</head>
<body>
  <h2>EstaciÃ³n MeteorolÃ³gica con GPS y RTC</h2>

  <div class="container">
    <!-- Panel de Sensores Ambientales -->
    <div class="panel">
      <div class="thermo-block">
        <div class="label">ð¡ Temperatura</div>
        <div class="value"><span id="temperature">--</span> Â°C</div>
        <div class="minmax">MÃ¡x: <span id="temperature-max">--</span> Â°C | MÃ­n: <span id="temperature-min">--</span> Â°C</div>
        <div class="bar-wrapper">
          <div class="bar-container"><div class="bar-mask" id="temperature-mask" style="height: 100%;"></div></div>
          <div class="scale"><div>50</div><div>40</div><div>30</div><div>20</div><div>10</div><div>0</div></div>
        </div>
      </div>

      <div class="thermo-block">
        <div class="label">ð§ Humedad</div>
        <div class="value"><span id="humidity">--</span> %</div>
        <div class="minmax">MÃ¡x: <span id="humidity-max">--</span> % | MÃ­n: <span id="humidity-min">--</span> %</div>
        <div class="bar-wrapper">
          <div class="bar-container"><div class="bar-mask" id="humidity-mask" style="height: 100%;"></div></div>
          <div class="scale"><div>100</div><div>80</div><div>60</div><div>40</div><div>20</div><div>0</div></div>
        </div>
      </div>
    </div>

    <!-- Panel de InformaciÃ³n GPS y RTC -->
    <div class="panel">
      <div class="label">ð UbicaciÃ³n GPS</div>
      <div class="gps-info">
        <div>Latitud: <span class="gps-value" id="latitude">--</span></div>
        <div>Longitud: <span class="gps-value" id="longitude">--</span></div>
        <div>Altitud: <span class="gps-value" id="altitude">--</span> m</div>
        <div>SatÃ©lites: <span class="gps-value" id="satellites">--</span></div>
        <div>Fecha: <span class="gps-value" id="date">--/--/--</span></div>
        <div>Hora: <span class="gps-value" id="time">--:--:--</span></div>
      </div>
      
      <div class="map-link" id="map-link" style="display: none;">
        <a id="google-map" target="_blank">Ver en Google Maps</a>
      </div>
      
      <div class="status" id="gps-status">
        Buscando seÃ±al GPS...
      </div>
      
      <div class="rtc-info">
        Fuente de hora: <span id="time-source">RTC</span>
      </div>
    </div>
  </div>

<script>
function updateData() {
  fetch('/sensors')
    .then(response => response.json())
    .then(data => {
      // Actualizar datos ambientales
      document.getElementById('temperature').textContent = data.temp.toFixed(1);
      document.getElementById('temperature-max').textContent = data.tempMax.toFixed(1);
      document.getElementById('temperature-min').textContent = data.tempMin.toFixed(1);
      document.getElementById('humidity').textContent = data.hum.toFixed(1);
      document.getElementById('humidity-max').textContent = data.humMax.toFixed(1);
      document.getElementById('humidity-min').textContent = data.humMin.toFixed(1);
      
      // Ajustar barras de temperatura y humedad
      document.getElementById('temperature-mask').style.height = (100 - (data.temp * 2)) + '%';
      document.getElementById('humidity-mask').style.height = (100 - Math.min(data.hum, 100)) + '%';

      // Actualizar datos GPS
      if(data.lat !== 0 && data.lng !== 0) {
        document.getElementById('latitude').textContent = data.lat.toFixed(6);
        document.getElementById('longitude').textContent = data.lng.toFixed(6);
        document.getElementById('altitude').textContent = data.alt.toFixed(1);
        document.getElementById('satellites').textContent = data.sats;
        document.getElementById('date').textContent = data.date;
        document.getElementById('time').textContent = data.time;
        
        // Mostrar enlace a Google Maps
        const mapLink = document.getElementById('map-link');
        const googleMap = document.getElementById('google-map');
        googleMap.href = `https://www.google.com/maps?q=${data.lat},${data.lng}`;
        mapLink.style.display = 'block';
        
        document.getElementById('gps-status').textContent = "SeÃ±al GPS estable";
        document.getElementById('gps-status').style.background = "#d4edda";
        document.getElementById('gps-status').style.color = "#155724";
      } else {
        document.getElementById('gps-status').textContent = "Buscando seÃ±al GPS...";
        document.getElementById('gps-status').style.background = "#fff3cd";
        document.getElementById('gps-status').style.color = "#856404";
      }
      
      // Mostrar fuente de la hora
      document.getElementById('time-source').textContent = data.timeSource;
    });
}
setInterval(updateData, 2000);
window.onload = updateData;
</script>
</body>
</html>
)rawliteral";

void handleSensorData() {
  // Leer datos del DHT11
  temp = dht.readTemperature();
  hum = dht.readHumidity();

  // Actualizar mÃ¡ximos y mÃ­nimos
  if (!isnan(temp)) {
    if (temp > tempMax) tempMax = temp;
    if (temp < tempMin) tempMin = temp;
  }
  if (!isnan(hum)) {
    if (hum > humMax) humMax = hum;
    if (hum < humMin) humMin = hum;
  }

  // Obtener fecha y hora del RTC si no hay GPS
  DateTime now = rtc.now();
  if (!gps.time.isValid() || !gps.date.isValid()) {
    useRTC = true;
    hora = String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
    fecha = String(now.day()) + "/" + String(now.month()) + "/" + String(now.year());
  } else {
    useRTC = false;
  }

  // Construir respuesta JSON
  String json = "{";
  json += "\"temp\":" + String(temp, 1) + ",";
  json += "\"tempMax\":" + String(tempMax, 1) + ",";
  json += "\"tempMin\":" + String(tempMin, 1) + ",";
  json += "\"hum\":" + String(hum, 1) + ",";
  json += "\"humMax\":" + String(humMax, 1) + ",";
  json += "\"humMin\":" + String(humMin, 1) + ",";
  json += "\"lat\":" + String(latitude, 6) + ",";
  json += "\"lng\":" + String(longitude, 6) + ",";
  json += "\"alt\":" + String(altitud, 1) + ",";
  json += "\"sats\":" + String(satelites) + ",";
  json += "\"date\":\"" + fecha + "\",";
  json += "\"time\":\"" + hora + "\",";
  json += "\"timeSource\":\"" + String(useRTC ? "RTC" : "GPS") + "\"";
  json += "}";

  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);
  
  // Inicializar I2C para el RTC
  Wire.begin();
  
  // Inicializar RTC
  if (!rtc.begin()) {
    Serial.println("No se encontrÃ³ el RTC");
    while (1);
  }
  
  // Si el RTC no estÃ¡ corriendo, configurarlo con la hora de compilaciÃ³n
  if (!rtc.isrunning()) {
    Serial.println("RTC no estÃ¡ funcionando, configurando hora...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Inicializar GPS
  SerialGPS.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  
  // Inicializar DHT
  dht.begin();

  // Conectar a WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conectado. IP: " + WiFi.localIP().toString());

  // Configurar rutas del servidor web
  server.on("/", HTTP_GET, []() {
    server.send_P(200, "text/html", index_html);
  });
  server.on("/sensors", HTTP_GET, handleSensorData);
  server.begin();
}

void loop() {
  server.handleClient();
  
  // Leer datos del GPS
  while (SerialGPS.available() > 0) {
    if (gps.encode(SerialGPS.read())) {
      if (gps.location.isValid()) {
        latitude = gps.location.lat();
        longitude = gps.location.lng();
        altitud = gps.altitude.meters();
        satelites = gps.satellites.value();
        
        // Actualizar RTC con la hora del GPS si estÃ¡ disponible
        if (gps.date.isValid() && gps.time.isValid()) {
          DateTime gpsTime(
            gps.date.year(),
            gps.date.month(),
            gps.date.day(),
            gps.time.hour(),
            gps.time.minute(),
            gps.time.second()
          );
          rtc.adjust(gpsTime);
          
          // Formatear fecha y hora
          fecha = String(gps.date.day()) + "/" + String(gps.date.month()) + "/" + String(gps.date.year());
          hora = String(gps.time.hour()) + ":" + String(gps.time.minute()) + ":" + String(gps.time.second());
          useRTC = false;
        }
      }
    }
  }

  // Si no hay datos del GPS por mucho tiempo, usar el RTC
  static unsigned long lastGPSData = 0;
  if (gps.location.isUpdated()) {
    lastGPSData = millis();
  }
  if (millis() - lastGPSData > 10000 && lastGPSData != 0) {
    DateTime now = rtc.now();
    fecha = String(now.day()) + "/" + String(now.month()) + "/" + String(now.year());
    hora = String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
    useRTC = true;
    lastGPSData = millis(); // Para no saturar el serial
  }
}
