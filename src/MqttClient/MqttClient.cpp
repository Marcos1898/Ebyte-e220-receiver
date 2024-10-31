#include "MqttClient.h"
#include <string.h>

int STRATEGY = 0; // 0 = CONSERVADOR , 1 = MODERADO ,2 = ARRIESGADO
float BUY_PRICE = 0.0;
float SELL_PRICE = 0.0;
char MODE_TOPIC[50];
char MODE_TOPIC_PETITION[50];
MqttClient::MqttClient(const char *mqtt_server) : mqtt_server(mqtt_server)
{
    mqttClient.setServer(mqtt_server, 1883);
    mqttClient.setCallback(callback);
}

void MqttClient::connect()
{
    while (!mqttClient.connected())
    {
        Serial.println("Conectando al servidor MQTT...");
        if (mqttClient.connect("arduino-client"))
        {
            Serial.println("Conectado al servidor MQTT");
            // Obtener la dirección IP local
            IPAddress localIP = WiFi.localIP();

            // Construir el topic con la dirección IP
            char ip_str[50]; // Ajusta el tamaño según sea necesario
            sprintf(ip_str, "%d.%d.%d.%d", localIP[0], localIP[1], localIP[2], localIP[3]);
            sprintf(mode_topic, "%s/mode", ip_str);
            sprintf(mode_topic_petition, "%s/mode-petition", ip_str);
            sprintf(consumption_topic, "%s/consumption", ip_str);
            sprintf(production_topic, "%s/production", ip_str);
            sprintf(buy_price_topic, "%s/buy-price", ip_str);
            sprintf(sell_price_topic, "%s/sell-price", ip_str);
            mqttClient.subscribe(mode_topic);
            mqttClient.subscribe(mode_topic_petition);
            mqttClient.subscribe(buy_price_topic);
            mqttClient.subscribe(sell_price_topic);

            char buffer[3]; // Tamaño del buffer

            sprintf(buffer, "%d", STRATEGY);
            // Publicar un mensaje al conectar
            strcpy(MODE_TOPIC_PETITION, mode_topic_petition);
            strcpy(MODE_TOPIC, mode_topic);
            mqttClient.publish(mode_topic, buffer);
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

void MqttClient::callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Mensaje recibido en el tema: ");
    Serial.println(topic);
    Serial.print("Contenido: ");

    // Variable para almacenar el valor recibido
    int intValue = 0;
    float floatValue = 0.0;

    // Convertir payload a una cadena de caracteres
    char payloadStr[length + 1];
    memcpy(payloadStr, payload, length);
    payloadStr[length] = '\0'; // Añadir el carácter nulo al final

    // Verificar si el topic contiene "mode-petition"
    if (strstr(topic, "mode-petition") != NULL)
    {
        // Convertir payload a un entero
        for (int i = 0; i < length; i++)
        {
            if (payload[i] >= '0' && payload[i] <= '9')
            {
                intValue = intValue * 10 + (payload[i] - '0');
            }
        }

        // Imprimir el valor del entero recibido
        Serial.println(intValue);

        // Actualizar STRATEGY
        STRATEGY = intValue;
    }
    else
    {
        // Convertir payload a un float
        floatValue = atof(payloadStr);
        // Imprimir el valor del float recibido
        Serial.println(floatValue);
        if (strstr(topic, "buy-price") != NULL)
        {
            BUY_PRICE = floatValue;
        }
        if (strstr(topic, "sell-price") != NULL)
        {
            SELL_PRICE = floatValue;
        }

    }
}

void MqttClient::set_consumption(int consumption, int production)
{
    char buffer[20]; // Tamaño del buffer

    sprintf(buffer, "%d", consumption);
    mqttClient.publish(consumption_topic, buffer);
    char buffer1[20]; // Tamaño del buffer

    sprintf(buffer1, "%d", production);
    mqttClient.publish(production_topic, buffer1);
}
void MqttClient::send_confirmation(int strategy)
{
    char buffer[20]; // Tamaño del buffer
    sprintf(buffer, "%d", strategy);
    mqttClient.publish(mode_topic, buffer);
}