#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

#include <WiFi.h>
#include <PubSubClient.h>
#include "structures.h"

class MqttClient
{
public:
    MqttClient(const char *mqtt_server);
    void send_message(Message message, GPSData gps, int rssi);
    void connect();
    void loop();

private:
    WiFiClient espClient;
    PubSubClient mqttClient{espClient};
    const char *mqtt_server;
    char topic[50];

};
#endif
