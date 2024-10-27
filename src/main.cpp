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
#include "LoRa_E220.h"
#include "LoRaUtils.h"
#include "structures.h"
#include "LoRaSettings.h"

// -------------------------------------------------
#define FREQUENCY_868
// ---------- esp32 pins --------------
LoRa_E220 e220ttl(&Serial2, 18, 21, 19); //  RX AUX M0 M1
const int BOARD_ID = 54321;
const int pin_led = 4;

void setup()
{
  pinMode(pin_led, OUTPUT);
  Serial.begin(9600);
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
}

void print_message(Message message, ResponseStructContainer rsc)
{
  Serial.println("-----------NUEVO MENSAJE------------");
  Serial.print("Número de secuencia: ");
  Serial.println(message.seq_n);
  Serial.print("ID placa: ");
  Serial.println(message.id);
  Serial.print("Estado LED: ");
  Serial.println(message.enable);
  Serial.print("Temperatura: ");
  Serial.print(message.dht11.temperature);
  Serial.print("º   Humedad: ");
  Serial.print(message.dht11.humidity);
  Serial.println("%");
  Serial.print("RSSI: ");
  int rssi = (int)rsc.rssi;
  int rssi_dbm = -(256 - rssi);
  Serial.print(rssi_dbm);
  Serial.println(" dBm");
  Serial.println("-----------------------------------");
  Serial.println();
}


void loop()
{
  if (e220ttl.available() > 1)
  {
    // read the String messageI
    ResponseStructContainer rsc = e220ttl.receiveMessageRSSI(sizeof(Message));

    // Is something goes wrong print error
    if (rsc.status.code != 1)
    {
      Serial.println(rsc.status.getResponseDescription());
    }
    else
    {
      Serial.println(rsc.status.getResponseDescription());
      struct Message message = *(Message *)rsc.data;
      print_message(message, rsc);
      digitalWrite(pin_led, !digitalRead(pin_led));
    }
  }
}
