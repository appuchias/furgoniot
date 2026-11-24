#include <WiFi.h>
#include <PubSubClient.h>

#include "config.h"
#include "mqtt.h"

namespace {

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastPublish = 0;
int contador = 0;

// Chamado cada vez que chega un mensaxe a un topic subscrito
void onMessage(char* topic, byte* payload, unsigned int length) {
  Serial.printf("[MQTT] Recibido en '%s': ", topic);
  for (unsigned int i = 0; i < length; i++) {
    Serial.write(payload[i]);
  }
  Serial.println();
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
      client.subscribe(MQTT_TOPIC_SUB);
      Serial.printf("[MQTT] Subscrito a '%s'\n", MQTT_TOPIC_SUB);
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

    String payload = "ola_mundo_" + String(contador++);
    if (client.publish(MQTT_TOPIC_PUB, payload.c_str())) {
      Serial.printf("[MQTT] Publicado en '%s': %s\n", MQTT_TOPIC_PUB, payload.c_str());
    } else {
      Serial.println("[MQTT] Non se puido publicar");
    }
  }
}
