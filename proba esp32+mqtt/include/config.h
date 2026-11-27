#ifndef CONFIG_H
#define CONFIG_H

// Broker MQTT público de prueba
#define MQTT_HOST "test.mosquitto.org"
#define MQTT_PORT 1883

// Topic de envío de datos
#define MQTT_TOPIC_PUB "devices/furgoniot/testout"

// Topic de subscrición para controlar o LED integrado
#define MQTT_TOPIC_LED "devices/furgoniot/testled"

// LED integrado da FireBeetle 2 ESP32-S3 (D13)
#define LED_PIN 21

// Cadenda de publicación: 1 minuto
#define PUBLISH_INTERVAL_MS 60000UL

//Configuración pin e tipo de sensor de temperatura e humidade
#define DHT_PIN D14
#define DHT_TYPE DHT11

#endif // CONFIG_H
