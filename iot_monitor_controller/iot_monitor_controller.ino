#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <DHT.h>

// Configuración WiFi
const char* ssid = "X";
const char* password = "X";

// Configuración del servidor Django
const char* serverUrl = "X";

// Configuración DHT
#define DHTPIN 17
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Pin del sensor de presencia
#define presencia 14

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    // Espera conexión WiFi
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Conectando a WiFi...");
    }

    Serial.println("Conexión WiFi establecida.");

    dht.begin();

    pinMode(presencia, INPUT);
}

void loop() {
    float temperature = dht.readTemperature();
    if (isnan(temperature)) {
        Serial.println("Error leyendo temperatura.");
        return;
    }

    bool motionDetected = digitalRead(presencia);

    if (WiFi.status() == WL_CONNECTED) {
        sendSensorData(temperature, motionDetected);
    }

    delay(5000);
}

void sendSensorData(float temperature, bool motion) {
    WiFiClientSecure client; // Usar WiFiClientSecure para HTTPS (con SSL)
    HTTPClient https;

    client.setInsecure(); // Se hace la conexion sin validar los certificados de seguridad
    https.begin(client, serverUrl);
    https.addHeader("Content-Type", "application/json");

    // Construir JSON
    String payload = "{\"temperature\": " + String(temperature) + ", \"motion_detected\": " + (motion ? "true" : "false") + "}";

    int httpsCode = https.POST(payload);
    if (httpsCode > 0) {
        String response = https.getString();
        Serial.println("Datos enviados: " + response);
    } else {
        Serial.println("Error al enviar datos: " + String(httpsCode));
    }

    https.end();
}
