/*
 * LoRa E220
 * Get configuration.
 * You must uncommend the correct constructor.
 *
 * by Renzo Mischianti <https://www.mischianti.org>
 *
 * https://www.mischianti.org
 *
 * E220		  	----- esp32
 * M0         	----- 19 (or 3.3v)
 * M1         	----- 21 (or 3.3v)
 * TX         	----- TX2 (PullUP)
 * RX         	----- RX2 (PullUP)
 * AUX        	----- 18  (PullUP)
 * VCC        	----- 3.3v/5v
 * GND        	----- GND
 *
 */
#include "Arduino.h"
#include <WiFi.h>
#include "LoRa_E220.h"
#include "LoRaUtils.h"
#include "structures.h"
#include "LoRaSettings.h"
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include "./MqttClient/MqttClient.h"

// -------------------------------------------------
#define FREQUENCY_868
// ---------- esp32 pins --------------
LoRa_E220 e220ttl(&Serial2, 18, 21, 19); //  RX AUX M0 M1
SoftwareSerial GPSSerial(22, 23);        // RX en GPIO22, TX en GPIO23
TinyGPSPlus gps;
MqttClient mqtt_client("raspberryisaac.ddns.net");


const int BOARD_ID = 54321;
const int pin_led = 4;
const char *WIFI_SSID = "Iphone de marcos";
const char *WIFI_PASSWORD = "marcos1234";

void wifi_setup()
{
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi");
  // Muestra la dirección IP
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());
}
void setup()
{

  pinMode(pin_led, OUTPUT);
  Serial.begin(9600);
  GPSSerial.begin(9600);
  while (!Serial)
  {
  };
  e220ttl.begin();
  // Configura los parámetros del módulo
  ResponseStructContainer c = e220ttl.getConfiguration();
  if (c.status.code == 1)
  {
    Configuration configuration = *(Configuration *)c.data;
    configureLoRa(configuration); // Configura el objeto `configuration`
    ResponseStatus rs = e220ttl.setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);
    Serial.println(rs.getResponseDescription());
    printParameters(configuration);
    c.close();
  }
  else
  {
    Serial.println("Error al obtener configuración LoRa");
    Serial.println(c.status.getResponseDescription());
  }
  wifi_setup();
  mqtt_client.connect();
}

int get_rssi(byte byte_rssi){
  int rssi = (int)byte_rssi;
  int rssi_dbm = -(256 - rssi);
  return rssi_dbm;
}

void print_message(Message message, int rssi_dbm, GPSData gps)
{
  Serial.println("=========NUEVO MENSAJE==============");
  Serial.println("-----------mensaje------------");
  Serial.print("Número de secuencia: ");
  Serial.println(message.seq_n);
  // Serial.print("ID placa: ");
  // Serial.println(message.id);
  // Serial.print("Estado LED: ");
  // Serial.println(message.enable);
  // Serial.print("Temperatura: ");
  // Serial.print(message.dht11.temperature);
  // Serial.print("º   Humedad: ");
  // Serial.print(message.dht11.humidity);
  // Serial.println("%");
  Serial.print("RSSI: ");
  Serial.print(rssi_dbm);
  Serial.println(" dBm");
  Serial.println("-----------localizacion------------");
  if (gps.lat == -200 || gps.lng == -200)
  {
    Serial.println("La localización no es válida");
  }
  else
  {
    Serial.print("Lat: ");
    Serial.println(gps.lat);
    Serial.print("Lng: ");
    Serial.println(gps.lng);
  }
  Serial.println("======================================");
  Serial.println();
}

GPSData read_gps()
{
  // Inicializar un objeto GPSData con valores predeterminados
  GPSData gps_data = {
      .lat = -200,
      .lng = -200,
      .speed = 0,
      .date = {0, 0, 0},
      .time = {0, 0, 0}};

  // Leer datos del GPS
  while (GPSSerial.available() > 0)
  {
    gps.encode(GPSSerial.read());
  }

  // Verifica si la localización es válida
  if (gps.location.isValid())
  {
    // Obtiene los datos GPS
    gps_data.lat = gps.location.lat();
    gps_data.lng = gps.location.lng();
    gps_data.speed = gps.speed.kmph();
    gps_data.date = {gps.date.day(), gps.date.month(), gps.date.year()};
    gps_data.time = {gps.time.hour(), gps.time.minute(), gps.time.second()};
  }
  else

    return gps_data;
}

GPSData last_gps;
void loop()
{
  mqtt_client.loop();
  last_gps = read_gps();
  // Verifica si hay datos disponibles en e220ttl
  if (e220ttl.available() > 1)
  {
    // Lee el mensaje y su RSSI
    ResponseStructContainer rsc = e220ttl.receiveMessageRSSI(sizeof(Message));

    // Si ocurre algún error, imprime el mensaje de error
    if (rsc.status.code != 1)
    {
      Serial.println(rsc.status.getResponseDescription());
    }
    else
    {
      int rssi_dbm = get_rssi(rsc.rssi);
      Serial.println(rsc.status.getResponseDescription());
      struct Message message = *(Message *)rsc.data;
      print_message(message, rssi_dbm, last_gps);
      mqtt_client.send_message(message,last_gps, rssi_dbm);                // Llama a la función para imprimir el mensaje
      digitalWrite(pin_led, !digitalRead(pin_led)); // Cambia el estado del LED
    }
  }
}
