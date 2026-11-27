#include <WiFi.h>
#include <DHT.h>

#include "secrets.h"
#include "mqtt.h"
#include "config.h"
#include "sensors.h"


unsigned long lastReading = 0;
float temperatura = 0.0;
float humidade = 0.0;
DHT dht(DHT_PIN, DHT_TYPE);

void connectWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando a WiFi");
  unsigned long inicio = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - inicio < 15000) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("WiFi conectado. IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Non se puido conectar ao WiFi. Reintentando...");
  }
}

void setup() {
  Serial.begin(115200);

  unsigned long t0 = millis();
  while (!Serial && millis() - t0 < 2000) delay(10);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  dht.begin();
  connectWiFi();
  mqtt::begin();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
    return;
  }

  // Ler o sensor cada 2 segundos (mínimo que pide o DHT11)
  if (millis() - lastReading >= 2000) {
    lastReading = millis();
    humidade = dht.readHumidity();
    temperatura = dht.readTemperature();

    if (isnan(humidade) || isnan(temperatura)) {
      Serial.println("Erro lendo o DHT11");
      return;
    }

    Serial.print("Temp: ");
    Serial.print(temperatura);
    Serial.print(" °C  Hum: ");
    Serial.print(humidade);
    Serial.println(" %");
  }

  mqtt::loop();
}   