#include "LoRaUtils.h"
#include "Arduino.h" // Necesario para usar Serial

void printParameters(Configuration configuration) {
	Serial.println("----------------------------------------");

	Serial.print(F("HEAD : "));
	Serial.print(configuration.COMMAND, HEX);
	Serial.print(" ");
	Serial.print(configuration.STARTING_ADDRESS, HEX);
	Serial.print(" ");
	Serial.println(configuration.LENGHT, HEX);
	Serial.println(F(" "));
	Serial.print(F("AddH : "));
	Serial.println(configuration.ADDH, HEX);
	Serial.print(F("AddL : "));
	Serial.println(configuration.ADDL, HEX);
	Serial.println(F(" "));
	Serial.print(F("Chan : "));
	Serial.print(configuration.CHAN, DEC);
	Serial.print(" -> ");
	Serial.println(configuration.getChannelDescription());
	Serial.println(F(" "));
	Serial.print(F("SpeedParityBit     : "));
	Serial.print(configuration.SPED.uartParity, BIN);
	Serial.print(" -> ");
	Serial.println(configuration.SPED.getUARTParityDescription());
	Serial.print(F("SpeedUARTDatte     : "));
	Serial.print(configuration.SPED.uartBaudRate, BIN);
	Serial.print(" -> ");
	Serial.println(configuration.SPED.getUARTBaudRateDescription());
	Serial.print(F("SpeedAirDataRate   : "));
	Serial.print(configuration.SPED.airDataRate, BIN);
	Serial.print(" -> ");
	Serial.println(configuration.SPED.getAirDataRateDescription());
	Serial.println(F(" "));
	Serial.print(F("OptionSubPacketSett: "));
	Serial.print(configuration.OPTION.subPacketSetting, BIN);
	Serial.print(" -> ");
	Serial.println(configuration.OPTION.getSubPacketSetting());
	Serial.print(F("OptionTranPower    : "));
	Serial.print(configuration.OPTION.transmissionPower, BIN);
	Serial.print(" -> ");
	Serial.println(configuration.OPTION.getTransmissionPowerDescription());
	Serial.print(F("OptionRSSIAmbientNo: "));
	Serial.print(configuration.OPTION.RSSIAmbientNoise, BIN);
	Serial.print(" -> ");
	Serial.println(configuration.OPTION.getRSSIAmbientNoiseEnable());
	Serial.println(F(" "));
	Serial.print(F("TransModeWORPeriod : "));
	Serial.print(configuration.TRANSMISSION_MODE.WORPeriod, BIN);
	Serial.print(" -> ");
	Serial.println(configuration.TRANSMISSION_MODE.getWORPeriodByParamsDescription());
	Serial.print(F("TransModeEnableLBT : "));
	Serial.print(configuration.TRANSMISSION_MODE.enableLBT, BIN);
	Serial.print(" -> ");
	Serial.println(configuration.TRANSMISSION_MODE.getLBTEnableByteDescription());
	Serial.print(F("TransModeEnableRSSI: "));
	Serial.print(configuration.TRANSMISSION_MODE.enableRSSI, BIN);
	Serial.print(" -> ");
	Serial.println(configuration.TRANSMISSION_MODE.getRSSIEnableByteDescription());
	Serial.print(F("TransModeFixedTrans: "));
	Serial.print(configuration.TRANSMISSION_MODE.fixedTransmission, BIN);
	Serial.print(" -> ");
	Serial.println(configuration.TRANSMISSION_MODE.getFixedTransmissionDescription());

	Serial.println("----------------------------------------");
}

void printModuleInformation(ModuleInformation moduleInformation) {
	Serial.println("----------------------------------------");
	Serial.print(F("HEAD: "));
	Serial.print(moduleInformation.COMMAND, HEX);
	Serial.print(" ");
	Serial.print(moduleInformation.STARTING_ADDRESS, HEX);
	Serial.print(" ");
	Serial.println(moduleInformation.LENGHT, DEC);

	Serial.print(F("Model no.: "));
	Serial.println(moduleInformation.model, HEX);
	Serial.print(F("Version  : "));
	Serial.println(moduleInformation.version, HEX);
	Serial.print(F("Features : "));
	Serial.println(moduleInformation.features, HEX);
	Serial.println("----------------------------------------");
}
