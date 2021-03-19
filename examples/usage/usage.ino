/*
 * Project ModbusLibTesting
 * Description:
 * Author:
 * Date:
 */
SYSTEM_MODE(SEMI_AUTOMATIC);

#include <ModbusLib.h>

//communication settings
int deviceSerialNum = 4;
int deviceBaudRate = 9600;
int txEnablePin = D7;

//device settings



ModbusLib modbusLib(deviceSerialNum, deviceBaudRate, txEnablePin, true);

//Register Addresses
uint16_t id_addy = 221; //real4
uint16_t flowrate_addy = 1; //real4
uint16_t soundspeed_addy = 7; //real4
uint16_t tomtos_addy = 97; //real4
uint16_t netaccum_addy = 113; //real4
uint16_t scale_addy = 1451; //real4
//the below are from the manual, not the flowmeter code
uint16_t up_addy = 91; //integer (0-2047)
uint16_t down_addy = 92; //integer (0-2047) //flowmeter code

uint16_t qual_addy = 90; //integer (00-99)

void setup() 
{  
  modbusLib.begin();
  modbusLib.SetDebugMode(false);
}



void loop() 
{
  GetReadings(15);
  GetReadings(16);
  GetReadings(17);
  GetReadings(18);
  delay(5000);
}

void GetReadings(int deviceAddress)
{
  Serial.println("Slave " + (String)deviceAddress + " Readings:");

  float id15 = modbusLib.readRegisterValue(deviceAddress,id_addy, Real4);
  float flowrate15 = modbusLib.readRegisterValue(deviceAddress,flowrate_addy, Real4);
  float soundspeed15 = modbusLib.readRegisterValue(deviceAddress,soundspeed_addy, Real4);
  float tomtos15 = modbusLib.readRegisterValue(deviceAddress,tomtos_addy, Real4);
  float netaccum15 = modbusLib.readRegisterValue(deviceAddress,netaccum_addy, Real4);
  float scale15 = modbusLib.readRegisterValue(deviceAddress,scale_addy, Real4);
  float up15 = 100*(float)modbusLib.readRegisterValue(deviceAddress,up_addy, Integer)/4095;
  float down15 = 100*(float)modbusLib.readRegisterValue(deviceAddress,down_addy, Integer)/4095;
  int qual15 = modbusLib.readRegisterValue(deviceAddress,qual_addy, Integer);

  Serial.println("Internal Diameter of slave " + (String)deviceAddress + ": " + (String)id15);
  Serial.println("Flowrate of slave " + (String)deviceAddress + ": "  + (String)flowrate15);
  Serial.println("Soundspeed of slave " + (String)deviceAddress + ": "  + (String)soundspeed15);
  Serial.println("Tomtos of slave " + (String)deviceAddress + ": "  + (String)tomtos15);
  Serial.println("Net Accumulator of slave " + (String)deviceAddress + ": "  + (String)netaccum15);
  Serial.println("Scale of slave " + (String)deviceAddress + ": "  + (String)scale15);
  Serial.println("Upstream of slave " + (String)deviceAddress + ": "  + (String)up15);
  Serial.println("Downstream of slave " + (String)deviceAddress + ": "  + (String)down15);
  Serial.println("Quality of slave " + (String)deviceAddress + ": "  + (String)qual15);

  Serial.println();
}