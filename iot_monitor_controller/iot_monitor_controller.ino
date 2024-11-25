#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <DHT.h>

// Configuración WiFi
const char* ssid = "X";
const char* password = "X";

// Configuración del servidor Django
const char* sensorDataUrl = "https://aguirrehdezma.pythonanywhere.com/api/sensor_data/";
const char* fanStatusUrl = "https://aguirrehdezma.pythonanywhere.com/api/get_fan_status/";

// Configuración DHT
#define DHTPIN 17
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Pin del sensor de presencia
#define presencia 14
// Pin del abanico
#define abanico 19

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
    pinMode(abanico, OUTPUT);
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
        controlFan();
    }

    delay(5000);
}

void sendSensorData(float temperature, bool motion) {
    WiFiClientSecure client; // Usar WiFiClientSecure para HTTPS (con SSL)
    HTTPClient https;

    client.setInsecure(); // Se hace la conexion sin validar los certificados de seguridad
    https.begin(client, sensorDataUrl);
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

void controlFan() {
    WiFiClientSecure client;
    HTTPClient https;

    client.setInsecure();
    https.begin(fanStatusUrl);

    int httpsCode = https.GET();
    if (httpsCode > 0) {
        String response = https.getString();
        Serial.println("Comando recibido: " + response);

        // Analiza el JSON para extraer el estado del abanico
        if (response.indexOf("\"fan_status\":true") > -1) {
            digitalWrite(abanico, HIGH);
            Serial.println("Abanico encendido.");
        } else {
            digitalWrite(abanico, LOW);
            Serial.println("Abanico apagado.");
        }
    } else {
        Serial.println("Error al recibir comando: " + String(httpsCode));
    }

    https.end();
}
