#include "MqttClient.h"
#include <string.h>

MqttClient::MqttClient(const char *mqtt_server) : mqtt_server(mqtt_server)
{
    mqttClient.setServer(mqtt_server, 1883);
}

void MqttClient::connect()
{
    while (!mqttClient.connected())
    {
        Serial.println("Conectando al servidor MQTT...");
        if (mqttClient.connect("arduino-client", "mqtt", "MqttAdmin"))
        {
            Serial.println("Conectado al servidor MQTT");

            sprintf(topic, "lora-receiver");
        }
        else
        {
            Serial.print("Error de conexión, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" Intentando de nuevo en 5 segundos");
            delay(5000);
        }
    }
}

void MqttClient::loop()
{
    if (!mqttClient.connected())
    {
        connect();
    }
    mqttClient.loop();
}

void MqttClient::send_message(Message message, GPSData gps, int int_rssi)
{
    String lat = String(gps.lat, 6); // Se asume que la latitud es un número
    String lng = String(gps.lng, 6); // Se asume que la longitud es un número
    String speed = String(gps.speed); // Se asume que la velocidad es un número
    String date = String(gps.time.hour) + ":" + String(gps.time.minute) + ":" + String(gps.time.second) + "  " +
                  String(gps.date.day) + "/" + String(gps.date.month) + "/" + String(gps.date.year); // Fecha como string
    String n_seq = String(message.seq_n); // Número de secuencia
    String rssi = String(int_rssi); // RSSI como número

    // Construcción del payload JSON
    String payload = "{";
    payload += "\"date\":\"" + date + "\","; // Fecha como string
    payload += "\"lat\":" + lat + ",";   // Latitud como número
    payload += "\"lng\":" + lng + ",";   // Longitud como número
    payload += "\"speed\":" + speed + ","; // Velocidad como número
    payload += "\"n_seq\":" + n_seq + ",";     // Número de secuencia como número
    payload += "\"rssi\":" + rssi;              // RSSI como número
    payload += "}";

    // Publicar el mensaje MQTT
    mqttClient.publish(topic, payload.c_str());
}


