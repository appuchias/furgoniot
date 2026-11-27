#include <WiFi.h>
#include <PubSubClient.h>
#include <cstring>

#include "config.h"
#include "mqtt.h"
#include "sensors.h"

namespace {

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastPublish = 0;

// O payload chega como bytes; convertémolo a texto e normalizámolo
String payloadToString(byte* payload, unsigned int length) {
  String texto;
  texto.reserve(length);
  for (unsigned int i = 0; i < length; i++) {
    texto += (char)payload[i];
  }
  texto.trim();
  texto.toLowerCase();
  return texto;
}

void setLed(bool encendido) {
  digitalWrite(LED_PIN, encendido ? HIGH : LOW);
}

// Chamado cada vez que chega un mensaxe a algún topic subscrito
void onMessage(char* topic, byte* payload, unsigned int length) {
  String comando = payloadToString(payload, length);

  Serial.printf("[MQTT] Recibido en '%s': %s\n", topic, comando.c_str());

  if (strcmp(topic, MQTT_TOPIC_LED) == 0) {
    if (comando == "on") {
      setLed(true);
      Serial.println("[MQTT] LED integrado ON");
    } else if (comando == "off") {
      setLed(false);
      Serial.println("[MQTT] LED integrado OFF");
    } else {
      Serial.println("[MQTT] Comando non recoñecido (usa 'on' ou 'off')");
    }
  }
}

// ID único por placa, baseado na MAC, para non colidir con outros clientes
String clientId() {
  return "furgoniot-" + WiFi.macAddress();
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("[MQTT] Conectando con " MQTT_HOST "... ");

    if (client.connect(clientId().c_str())) {
      Serial.println("OK");

      client.subscribe(MQTT_TOPIC_LED);
      Serial.printf("[MQTT] Subscrito a '%s'\n", MQTT_TOPIC_LED);
    } else {
      Serial.printf("fallou (code %d). Reintentando en 5 s...\n", client.state());
      delay(5000);
    }
  }
}

} // namespace

void mqtt::begin() {

  client.setServer(MQTT_HOST, MQTT_PORT);
  client.setCallback(onMessage);
  lastPublish = millis();

  Serial.printf("[MQTT] Configurado: %s:%d\n", MQTT_HOST, MQTT_PORT);
}

void mqtt::loop() {
  if (!client.connected()) reconnect();

  // Mantén viva a conexión e procesa mensaxes entrantes
  client.loop();

  if (millis() - lastPublish >= PUBLISH_INTERVAL_MS) {
    lastPublish = millis();

    String payload = "Temperatura:" + String(temperatura, 1) + ";Humidade" + String(humidade, 1);
    if (client.publish(MQTT_TOPIC_PUB, payload.c_str())) {
      Serial.printf("[MQTT] Publicado en '%s': %s\n", MQTT_TOPIC_PUB, payload.c_str());
    } else {
      Serial.println("[MQTT] Non se puido publicar");
    }
  }
}
