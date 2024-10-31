#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

#include <WiFi.h>
#include <PubSubClient.h>

class MqttClient
{
public:
    MqttClient(const char *mqtt_server);
    void set_consumption(int consumption, int production);
    void send_confirmation(int strategy);
    void connect();
    void loop();

private:
    static void callback(char *topic, byte *payload, unsigned int length);
    WiFiClient espClient;
    PubSubClient mqttClient{espClient};
    const char *mqtt_server;
    char mode_topic[50];
    char mode_topic_petition[50];
    char consumption_topic[50];
    char production_topic[50];
    char buy_price_topic[50];
    char sell_price_topic[50];

};
extern int STRATEGY;
extern float BUY_PRICE;
extern float SELL_PRICE;
#endif
