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
  Serial.println("Slave 15 Readings:");

  float id15 = modbusLib.readRegisterValue(15,id_addy, dataFormat::Real4);
  float flowrate15 = modbusLib.readRegisterValue(15,flowrate_addy, dataFormat::Real4);
  float soundspeed15 = modbusLib.readRegisterValue(15,soundspeed_addy, dataFormat::Real4);
  float tomtos15 = modbusLib.readRegisterValue(15,tomtos_addy, dataFormat::Real4);
  float netaccum15 = modbusLib.readRegisterValue(15,netaccum_addy, dataFormat::Real4);
  float scale15 = modbusLib.readRegisterValue(15,scale_addy, dataFormat::Real4);

  float up15 = 100*(float)modbusLib.readRegisterValue(15,up_addy, dataFormat::Integer)/4095;
  float down15 = 100*(float)modbusLib.readRegisterValue(15,down_addy, dataFormat::Integer)/4095;

  int qual15 = modbusLib.readRegisterValue(15,qual_addy, dataFormat::Integer);

  Serial.println("Internal Diameter of slave 15: " + (String)id15);
  Serial.println("Flowrate of slave 15: " + (String)flowrate15);
  Serial.println("Soundspeed of slave 15: " + (String)soundspeed15);
  Serial.println("Tomtos of slave 15: " + (String)tomtos15);
  Serial.println("Net Accumulator of slave 15: " + (String)netaccum15);
  Serial.println("Scale of slave 15: " + (String)scale15);
  Serial.println("Upstream of slave 15: " + (String)up15);
  Serial.println("Downstream of slave 15: " + (String)down15);
  Serial.println("Quality of slave 15: " + (String)qual15);

  Serial.println();
  
  delay(5000);

}