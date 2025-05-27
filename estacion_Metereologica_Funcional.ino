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
String horaStr = "--:--:--";
String fechaStr = "--/--/----";
String lat = "--", lng = "--", alt = "--";

// Variables de control
bool sistemaActivo = true;
bool gpsValido = false;

// Nueva interfaz gráfica moderna (modificada para mostrar datos GPS separados)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Monitor Ambiental Inteligente</title>
  <style>
    :root {
      --primary: #4361ee;
      --secondary: #3f37c9;
      --accent: #4895ef;
      --light: #f8f9fa;
      --dark: #212529;
      --success: #4cc9f0;
      --danger: #f72585;
      --warning: #f8961e;
    }
    
    html {
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
      background-color: #f5f7fa;
      color: var(--dark);
    }
    
    body {
      max-width: 1200px;
      margin: 0 auto;
      padding: 20px;
    }
    
    header {
      text-align: center;
      margin-bottom: 30px;
      padding: 20px;
      background: linear-gradient(135deg, var(--primary), var(--secondary));
      color: white;
      border-radius: 10px;
      box-shadow: 0 4px 6px rgba(0,0,0,0.1);
    }
    
    h1 {
      margin: 0;
      font-size: 2.2rem;
      display: flex;
      align-items: center;
      justify-content: center;
      gap: 15px;
    }
    
    .dashboard {
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
      gap: 25px;
      margin-bottom: 30px;
    }
    
    .card {
      background: white;
      border-radius: 12px;
      padding: 20px;
      box-shadow: 0 2px 10px rgba(0,0,0,0.05);
      transition: transform 0.3s, box-shadow 0.3s;
    }
    
    .card:hover {
      transform: translateY(-5px);
      box-shadow: 0 6px 15px rgba(0,0,0,0.1);
    }
    
    .card-header {
      display: flex;
      align-items: center;
      margin-bottom: 15px;
      padding-bottom: 10px;
      border-bottom: 1px solid #eee;
    }
    
    .card-title {
      margin: 0;
      font-size: 1.3rem;
      color: var(--primary);
      display: flex;
      align-items: center;
      gap: 10px;
    }
    
    .icon {
      width: 24px;
      height: 24px;
    }
    
    .sensor-value {
      font-size: 2.5rem;
      font-weight: bold;
      margin: 15px 0;
      display: flex;
      align-items: flex-end;
      gap: 5px;
    }
    
    .unit {
      font-size: 1.2rem;
      color: #666;
      margin-bottom: 5px;
    }
    
    .sensor-details {
      display: flex;
      justify-content: space-between;
      margin-top: 15px;
    }
    
    .detail {
      text-align: center;
    }
    
    .detail-label {
      font-size: 0.9rem;
      color: #666;
    }
    
    .detail-value {
      font-size: 1.1rem;
      font-weight: bold;
    }
    
    .progress-container {
      height: 10px;
      background: #e9ecef;
      border-radius: 5px;
      margin: 15px 0;
      overflow: hidden;
    }
    
    .progress-bar {
      height: 100%;
      border-radius: 5px;
      transition: width 0.5s;
    }
    
    .temp-progress {
      background: linear-gradient(90deg, #3a86ff, #ff006e);
    }
    
    .hum-progress {
      background: linear-gradient(90deg, #3a86ff, #00b4d8);
    }
    
    .info-section {
      display: grid;
      grid-template-columns: 1fr 1fr;
      gap: 25px;
      margin-bottom: 30px;
    }
    
    @media (max-width: 768px) {
      .info-section {
        grid-template-columns: 1fr;
      }
    }
    
    .system-control {
      display: flex;
      flex-direction: column;
      align-items: center;
      padding: 25px;
      background: white;
      border-radius: 12px;
      box-shadow: 0 2px 10px rgba(0,0,0,0.05);
    }
    
    .switch {
      position: relative;
      display: inline-block;
      width: 80px;
      height: 40px;
      margin: 20px 0;
    }
    
    .switch input {
      opacity: 0;
      width: 0;
      height: 0;
    }
    
    .slider {
      position: absolute;
      cursor: pointer;
      top: 0;
      left: 0;
      right: 0;
      bottom: 0;
      background-color: #ccc;
      transition: .4s;
      border-radius: 40px;
    }
    
    .slider:before {
      position: absolute;
      content: "";
      height: 32px;
      width: 32px;
      left: 4px;
      bottom: 4px;
      background-color: white;
      transition: .4s;
      border-radius: 50%;
    }
    
    input:checked + .slider {
      background-color: var(--success);
    }
    
    input:checked + .slider:before {
      transform: translateX(40px);
    }
    
    .status {
      font-weight: bold;
      font-size: 1.2rem;
      margin-top: 10px;
      display: flex;
      align-items: center;
      gap: 8px;
    }
    
    .status-indicator {
      width: 12px;
      height: 12px;
      border-radius: 50%;
      display: inline-block;
    }
    
    .on {
      background-color: var(--success);
    }
    
    .off {
      background-color: var(--danger);
    }
    
    .map-container {
      height: 200px;
      background-color: #eee;
      border-radius: 10px;
      display: flex;
      align-items: center;
      justify-content: center;
      color: #666;
      margin-top: 15px;
    }
    
    .gps-info {
      margin-top: 10px;
      font-size: 0.9rem;
    }
    
    .gps-detail {
      display: flex;
      justify-content: space-between;
      margin-top: 5px;
    }
    
    .gps-label {
      color: #666;
    }
    
    .gps-value {
      font-weight: bold;
    }
    
    footer {
      text-align: center;
      margin-top: 40px;
      padding: 20px;
      color: #666;
      font-size: 0.9rem;
    }
  </style>
</head>
<body>
  <header>
    <h1>
      <svg class="icon" viewBox="0 0 24 24" fill="currentColor">
        <path d="M19 14a1 1 0 0 1 1 1v3a3 3 0 0 1-3 3H7a3 3 0 0 1-3-3v-3a1 1 0 0 1 1-1h14zm-8-9a1 1 0 0 1 2 0v1h3a1 1 0 0 1 0 2H6a1 1 0 1 1 0-2h3V5z"/>
      </svg>
      Monitor Ambiental Inteligente
    </h1>
  </header>
  
  <div class="dashboard">
    <!-- Tarjeta de Temperatura -->
    <div class="card">
      <div class="card-header">
        <h2 class="card-title">
          <svg class="icon" viewBox="0 0 24 24" fill="currentColor">
            <path d="M12 22a5 5 0 0 1-3-9V5a3 3 0 1 1 6 0v8a5 5 0 0 1-3 9zm0-18a1 1 0 0 0-1 1v8.54a1 1 0 0 1-.5.87 3 3 0 1 0 3 0 1 1 0 0 1-.5-.87V5a1 1 0 0 0-1-1z"/>
          </svg>
          Temperatura
        </h2>
      </div>
      <div class="sensor-value">
        <span id="temperature">--</span>
        <span class="unit">°C</span>
      </div>
      <div class="progress-container">
        <div id="temperature-progress" class="progress-bar temp-progress" style="width: 0%"></div>
      </div>
      <div class="sensor-details">
        <div class="detail">
          <div class="detail-label">Máxima</div>
          <div class="detail-value" id="temperature-max">--°C</div>
        </div>
        <div class="detail">
          <div class="detail-label">Mínima</div>
          <div class="detail-value" id="temperature-min">--°C</div>
        </div>
      </div>
    </div>
    
    <!-- Tarjeta de Humedad -->
    <div class="card">
      <div class="card-header">
        <h2 class="card-title">
          <svg class="icon" viewBox="0 0 24 24" fill="currentColor">
            <path d="M12 3.25S4 10 4 14a8 8 0 1 1 16 0c0-4-8-10.75-8-10.75zm0 15a4 4 0 1 0 0-8 4 4 0 0 0 0 8z"/>
          </svg>
          Humedad
        </h2>
      </div>
      <div class="sensor-value">
        <span id="humidity">--</span>
        <span class="unit">%</span>
      </div>
      <div class="progress-container">
        <div id="humidity-progress" class="progress-bar hum-progress" style="width: 0%"></div>
      </div>
      <div class="sensor-details">
        <div class="detail">
          <div class="detail-label">Máxima</div>
          <div class="detail-value" id="humidity-max">--%</div>
        </div>
        <div class="detail">
          <div class="detail-label">Mínima</div>
          <div class="detail-value" id="humidity-min">--%</div>
        </div>
      </div>
    </div>
  </div>
  
  <div class="info-section">
    <!-- Información de Tiempo -->
    <div class="card">
      <div class="card-header">
        <h2 class="card-title">
          <svg class="icon" viewBox="0 0 24 24" fill="currentColor">
            <path d="M12 2a10 10 0 1 0 10 10A10 10 0 0 0 12 2zm0 18a8 8 0 1 1 8-8 8 8 0 0 1-8 8zm1-13h-2v6h6v-2h-4z"/>
          </svg>
          Información de Tiempo
        </h2>
      </div>
      <div style="margin: 15px 0;">
        <div style="display: flex; align-items: center; gap: 10px; margin-bottom: 10px;">
          <svg width="20" height="20" viewBox="0 0 24 24" fill="currentColor">
            <path d="M12 2a10 10 0 1 0 10 10A10 10 0 0 0 12 2zm0 18a8 8 0 1 1 8-8 8 8 0 0 1-8 8zm1-13h-2v6h6v-2h-4z"/>
          </svg>
          <span id="hora" style="font-size: 1.1rem;">--:--:--</span>
        </div>
        <div style="display: flex; align-items: center; gap: 10px;">
          <svg width="20" height="20" viewBox="0 0 24 24" fill="currentColor">
            <path d="M12 2a10 10 0 1 0 10 10A10 10 0 0 0 12 2zm0 18a8 8 0 1 1 8-8 8 8 0 0 1-8 8zm1-13h-2v6h6v-2h-4z"/>
          </svg>
          <span id="fecha" style="font-size: 1.1rem;">--/--/----</span>
        </div>
      </div>
    </div>
    
    <!-- Información de Ubicación -->
    <div class="card">
      <div class="card-header">
        <h2 class="card-title">
          <svg class="icon" viewBox="0 0 24 24" fill="currentColor">
            <path d="M12 2a8 8 0 0 0-8 8c0 5.4 7 11.5 7.3 11.8a1 1 0 0 0 1.4 0C13 21.5 20 15.4 20 10a8 8 0 0 0-8-8zm0 17.6c-2.1-2-6-6.3-6-9.6a6 6 0 0 1 12 0c0 3.3-3.9 7.6-6 9.6zM12 6a4 4 0 1 0 4 4 4 4 0 0 0-4-4z"/>
          </svg>
          Información de Ubicación
        </h2>
      </div>
      <div class="gps-info">
        <div class="gps-detail">
          <span class="gps-label">Latitud:</span>
          <span class="gps-value" id="lat">--</span>
        </div>
        <div class="gps-detail">
          <span class="gps-label">Longitud:</span>
          <span class="gps-value" id="lng">--</span>
        </div>
        <div class="gps-detail">
          <span class="gps-label">Altitud:</span>
          <span class="gps-value" id="alt">--</span> m
        </div>
      </div>
      <div class="map-container">
        <svg width="48" height="48" viewBox="0 0 24 24" fill="#666">
          <path d="M12 2a8 8 0 0 0-8 8c0 5.4 7 11.5 7.3 11.8a1 1 0 0 0 1.4 0C13 21.5 20 15.4 20 10a8 8 0 0 0-8-8zm0 17.6c-2.1-2-6-6.3-6-9.6a6 6 0 0 1 12 0c0 3.3-3.9 7.6-6 9.6zM12 6a4 4 0 1 0 4 4 4 4 0 0 0-4-4z"/>
        </svg>
      </div>
    </div>
  </div>
  
  <div class="system-control">
    <h2 style="margin: 0; color: var(--primary);">Control del Sistema</h2>
    <label class="switch">
      <input type="checkbox" id="toggle-system" onchange="toggleSystem()" checked>
      <span class="slider"></span>
    </label>
    <div class="status" id="system-status">
      <span class="status-indicator on"></span>
      <span>SISTEMA ACTIVO</span>
    </div>
  </div>
  
  <footer>
    <p>Sistema de Monitoreo Ambiental &copy; 2023 | ESP32 IoT</p>
  </footer>

<script>
function updateData() {
  fetch('/sensors')
    .then(response => response.json())
    .then(data => {
      // Actualizar sensores
      document.getElementById('temperature').textContent = data.temp.toFixed(1);
      document.getElementById('temperature-max').textContent = data.tempMax.toFixed(1) + '°C';
      document.getElementById('temperature-min').textContent = data.tempMin.toFixed(1) + '°C';
      document.getElementById('humidity').textContent = data.hum.toFixed(1);
      document.getElementById('humidity-max').textContent = data.humMax.toFixed(1) + '%';
      document.getElementById('humidity-min').textContent = data.humMin.toFixed(1) + '%';
      
      // Actualizar barras de progreso
      document.getElementById('temperature-progress').style.width = Math.min(data.temp, 100) + '%';
      document.getElementById('humidity-progress').style.width = Math.min(data.hum, 100) + '%';
      
      // Actualizar hora y fecha
      document.getElementById('hora').textContent = data.hora;
      document.getElementById('fecha').textContent = data.fecha;
      
      // Actualizar GPS
      document.getElementById('lat').textContent = data.lat;
      document.getElementById('lng').textContent = data.lng;
      document.getElementById('alt').textContent = data.alt;
      
      // Actualizar estado del sistema
      document.getElementById('toggle-system').checked = data.sistemaActivo;
      updateSystemStatus(data.sistemaActivo);
    });
}

function toggleSystem() {
  const isActive = document.getElementById('toggle-system').checked;
  fetch('/control?estado=' + (isActive ? '1' : '0'))
    .then(response => {
      updateSystemStatus(isActive);
    });
}

function updateSystemStatus(isActive) {
  const statusElement = document.getElementById('system-status');
  const indicator = statusElement.querySelector('.status-indicator');
  const text = statusElement.querySelector('span:not(.status-indicator)');
  
  if (isActive) {
    indicator.className = 'status-indicator on';
    text.textContent = 'SISTEMA ACTIVO';
  } else {
    indicator.className = 'status-indicator off';
    text.textContent = 'SISTEMA INACTIVO';
  }
}

setInterval(updateData, 2000);
window.onload = updateData;
</script>
</body>
</html>
)rawliteral";

bool testWiFi() {
  Serial.print("Conectando a WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  
  Serial.println("\nConexión WiFi exitosa!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  return true;
}

void ajustarHoraDesdePC() {
  const char *compilationDate = __DATE__;
  const char *compilationTime = __TIME__;
  
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
  rtc.adjust(ahora);
  
  Serial.println("RTC adjusted from compilation time");
}

void actualizarDatosGPS() {
  // Leer datos del GPS mientras haya información disponible
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      if (gps.location.isValid()) {
        lat = String(gps.location.lat(), 6);
        lng = String(gps.location.lng(), 6);
        alt = String(gps.altitude.meters(), 2);
        gpsValido = true;
      } else {
        lat = "--";
        lng = "--";
        alt = "--";
        gpsValido = false;
      }
    }
  }
}

void actualizarFechaHora() {
  DateTime now = rtc.now();
  horaStr = String(now.hour()) + ":" + 
            String(now.minute()) + ":" + 
            String(now.second());
            
  fechaStr = String(now.day()) + "/" + 
             String(now.month()) + "/" + 
             String(now.year());
}

void handleSensorData() {
  String json = "{";
  json += "\"temp\":" + String(temp, 1) + ",";
  json += "\"tempMax\":" + String(tempMax, 1) + ",";
  json += "\"tempMin\":" + String(tempMin, 1) + ",";
  json += "\"hum\":" + String(hum, 1) + ",";
  json += "\"humMax\":" + String(humMax, 1) + ",";
  json += "\"humMin\":" + String(humMin, 1) + ",";
  json += "\"hora\":\"" + horaStr + "\",";
  json += "\"fecha\":\"" + fechaStr + "\",";
  json += "\"lat\":\"" + lat + "\",";
  json += "\"lng\":\"" + lng + "\",";
  json += "\"alt\":\"" + alt + "\",";
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
  
  Serial.println("Probando WiFi...");
  bool wifiConnected = testWiFi();
  Serial.println("\n=== Resultados ===");
  Serial.printf("WiFi: %s\n", wifiConnected ? "CONECTADO" : "FALLÓ");

  if (!rtc.begin()) {
    Serial.println("Error en RTC");
    while(1);
  }
  ajustarHoraDesdePC();

  gpsSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);
  dht.begin();

  // Configurar rutas del servidor web
  server.on("/", HTTP_GET, []() {
    server.send_P(200, "text/html", index_html);
  });
  server.on("/sensors", HTTP_GET, handleSensorData);
  server.on("/control", HTTP_GET, handleControl);
  
  server.begin();
  Serial.println("Servidor HTTP iniciado");
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
    
    // Actualizar GPS y hora
    actualizarDatosGPS();
    actualizarFechaHora();
    
    // Mostrar datos por serial
    Serial.println("=== Datos ===");
    Serial.println("Hora: " + horaStr);
    Serial.println("Fecha: " + fechaStr);
    Serial.printf("Temp: %.1f°C (Max: %.1f°C, Min: %.1f°C)\n", temp, tempMax, tempMin);
    Serial.printf("Hum: %.1f%% (Max: %.1f%%, Min: %.1f%%)\n", hum, humMax, humMin);
    Serial.println("GPS - Lat: " + lat + ", Lng: " + lng + ", Alt: " + alt + " m");
    Serial.println("=============");
  } else {
    Serial.println("Sistema inactivo (en espera)");
  }
  
  delay(2000);
}