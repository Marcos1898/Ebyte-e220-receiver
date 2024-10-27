// LoRaSettings.cpp
#include "LoRaSettings.h"

void configureLoRa(Configuration &configuration) {
    configuration.ADDL = 0x03; // Dirección baja
    configuration.ADDH = 0x00; // Dirección alta

    configuration.CHAN = 18; // Canal de comunicación

    // Configuración de velocidad
    configuration.SPED.uartBaudRate = UART_BPS_9600;    // Velocidad de UART
    configuration.SPED.airDataRate = AIR_DATA_RATE_010_24; // Velocidad de transmisión
    configuration.SPED.uartParity = MODE_00_8N1;        // Paridad

    // Configuración de opciones
    configuration.OPTION.subPacketSetting = SPS_200_00;                 // Tamaño de paquete
    configuration.OPTION.RSSIAmbientNoise = RSSI_AMBIENT_NOISE_DISABLED; // Ruido RSSI
    configuration.OPTION.transmissionPower = POWER_13;                   // Potencia de transmisión

    // Configuración del modo de transmisión
    configuration.TRANSMISSION_MODE.enableRSSI = RSSI_ENABLED;                   // Activar RSSI
    configuration.TRANSMISSION_MODE.fixedTransmission = FT_FIXED_TRANSMISSION;   // Modo fijo
    configuration.TRANSMISSION_MODE.enableLBT = LBT_DISABLED;                    // Desactivar LBT
    configuration.TRANSMISSION_MODE.WORPeriod = WOR_2000_011;                    // Período WOR
}
