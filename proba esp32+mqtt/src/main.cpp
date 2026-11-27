#include <WiFi.h>
#include "secrets.h"
#include "mqtt.h"
#include "config.h"

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

  // Espera breve polo USB CDC sen bloquear se non hai monitor aberto
  unsigned long t0 = millis();
  while (!Serial && millis() - t0 < 2000) delay(10);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  connectWiFi();
  mqtt::begin();
}

void loop() {
  // Sen rede non ten sentido intentar MQTT
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
    return;
  }

  // Aquí delegamos toda a lóxica MQTT ao outro ficheiro
  mqtt::loop();
}
