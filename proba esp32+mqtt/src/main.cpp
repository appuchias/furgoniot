#include <WiFi.h>
#include "secrets.h"

const char* ssid = "TU_WIFI";
const char* password = "TU_CONTRASENA";

const int ledPin = 21;  // LED integrado (D13)

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    // Parpadeo rápido mientras conecta
    digitalWrite(ledPin, !digitalRead(ledPin));
    delay(200);
  }

  Serial.println();
  Serial.print("Conectado. IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // LED encendido 1s, apagado 0.5s
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
  delay(500);
}