/*
 * LoRa E220
 * Get configuration.
 * You must uncommend the correct constructor.
 *
 * by Renzo Mischianti <https://www.mischianti.org>
 *
 * https://www.mischianti.org
 *
 * E220		  ----- WeMos D1 mini	----- esp32			----- Arduino Nano 33 IoT	----- Arduino MKR	----- Raspberry Pi Pico   ----- stm32               ----- ArduinoUNO
 * M0         ----- D7 (or 3.3v)	----- 19 (or 3.3v)	----- 4 (or 3.3v)			----- 2 (or 3.3v)	----- 10 (or 3.3v)	      ----- PB0 (or 3.3v)       ----- 7 Volt div (or 3.3v)
 * M1         ----- D6 (or 3.3v)	----- 21 (or 3.3v)	----- 6 (or 3.3v)			----- 4 (or 3.3v)	----- 11 (or 3.3v)	      ----- PB10 (or 3.3v)      ----- 6 Volt div (or 3.3v)
 * TX         ----- D3 (PullUP)		----- TX2 (PullUP)	----- TX1 (PullUP)			----- 14 (PullUP)	----- 8 (PullUP)	      ----- PA2 TX2 (PullUP)    ----- 4 (PullUP)
 * RX         ----- D4 (PullUP)		----- RX2 (PullUP)	----- RX1 (PullUP)			----- 13 (PullUP)	----- 9 (PullUP)	      ----- PA3 RX2 (PullUP)    ----- 5 Volt div (PullUP)
 * AUX        ----- D5 (PullUP)		----- 18  (PullUP)	----- 2  (PullUP)			----- 0  (PullUP)	----- 2  (PullUP)	      ----- PA0  (PullUP)       ----- 3 (PullUP)
 * VCC        ----- 3.3v/5v			----- 3.3v/5v		----- 3.3v/5v				----- 3.3v/5v		----- 3.3v/5v		      ----- 3.3v/5v             ----- 3.3v/5v
 * GND        ----- GND				----- GND			----- GND					----- GND			----- GND			      ----- GND                 ----- GND
 *
 */

#include "Arduino.h"
#include "LoRa_E220.h"
#include "LoRaUtils.h"

// -------------------------------------------------
#define FREQUENCY_868
// ---------- esp32 pins --------------
LoRa_E220 e220ttl(&Serial2, 18, 21, 19); //  RX AUX M0 M1
const int BOARD_ID = 54321;
const int pin_led = 4;

// -------------------------------------------------
struct DHTReadings {
    float temperature;
    float humidity;
}; 

struct Message
{
	int seq_n;
	int id;
	bool enable;
	DHTReadings dht11;
};
void printParameters(struct Configuration configuration);
void printModuleInformation(struct ModuleInformation moduleInformation);

void setup()
{
  pinMode(pin_led, OUTPUT);
  Serial.begin(9600);
  while (!Serial)
  {
  };
  delay(500);

  Serial.println();

  // Startup all pins and UART
  e220ttl.begin();

  ResponseStructContainer c;
  c = e220ttl.getConfiguration();
  // It's important get configuration pointer before all other operation
  Configuration configuration = *(Configuration *)c.data;
  Serial.println(c.status.getResponseDescription());
  Serial.println(c.status.code);

  printParameters(configuration);

  //	----------------------- DEFAULT TRANSPARENT -----------------------
  configuration.ADDL = 0x03; // First part of address
  configuration.ADDH = 0x00; // Second part

  configuration.CHAN = 18; // Communication channel

  configuration.SPED.uartBaudRate = UART_BPS_9600;       // Serial baud rate
  configuration.SPED.airDataRate = AIR_DATA_RATE_010_24; // Air baud rate
  configuration.SPED.uartParity = MODE_00_8N1;           // Parity bit

  configuration.OPTION.subPacketSetting = SPS_200_00;                  // Packet size
  configuration.OPTION.RSSIAmbientNoise = RSSI_AMBIENT_NOISE_DISABLED; // Need to send special command
  configuration.OPTION.transmissionPower = POWER_22;                   // Device power

  configuration.TRANSMISSION_MODE.enableRSSI = RSSI_ENABLED;                       // Enable RSSI info
  configuration.TRANSMISSION_MODE.fixedTransmission = FT_FIXED_TRANSMISSION; // Enable repeater mode
  configuration.TRANSMISSION_MODE.enableLBT = LBT_DISABLED;                        // Check interference
  configuration.TRANSMISSION_MODE.WORPeriod = WOR_2000_011;                        // WOR timing

  // Set configuration changed and set to not hold the configuration
  ResponseStatus rs = e220ttl.setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);
  Serial.println(rs.getResponseDescription());
  Serial.println(rs.code);

  c = e220ttl.getConfiguration();
  // It's important get configuration pointer before all other operation
  configuration = *(Configuration *)c.data;
  Serial.println(c.status.getResponseDescription());
  Serial.println(c.status.code);

  printParameters(configuration);
  c.close();
}
int seq_n = 0;
bool led = false;
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
      // Print the data received
      Serial.println("-----------NUEVO MENSAJE------------");
      Serial.println(rsc.status.getResponseDescription());
      struct Message message = *(Message *)rsc.data;


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
      int rssi = (int) rsc.rssi;
      int rssi_dbm = - (256 - rssi);
      Serial.println(rssi_dbm);
      Serial.println("-----------------------------------");
      Serial.println();

      digitalWrite(pin_led, (message.enable ?HIGH:LOW));
    }
  }
}
