#ifndef CONFIG_H
#define CONFIG_H

// Broker MQTT público de prueba
#define MQTT_HOST "test.mosquitto.org"
#define MQTT_PORT 1883

// Topic de subscrición e de publicación (por agora o mesmo)
#define MQTT_TOPIC_SUB "devices/furgoniot/testout"
#define MQTT_TOPIC_PUB "devices/furgoniot/testout"

// Cadenda de publicación: 1 minuto
#define PUBLISH_INTERVAL_MS 60000UL

#endif // CONFIG_H
