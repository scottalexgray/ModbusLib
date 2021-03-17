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


void setup() 
{

  //delay(5000);
  modbusLib.begin();
  modbusLib.SetDebugMode(false);

}


void loop() 
{
  double id15 = modbusLib.readRegisterValue(15,0xDD, dataFormat::Real4);
  Serial.println("Internal Diameter of slave 15: " + (String)id15);

  double id16 = modbusLib.readRegisterValue(16,0xDD, dataFormat::Real4);
  Serial.println("Internal Diameter of slave 16: " + (String)id16);

  double id17 = modbusLib.readRegisterValue(17,0xDD, dataFormat::Real4);
  Serial.println("Internal Diameter of slave 17: " + (String)id17);

  double id18 = modbusLib.readRegisterValue(18,0xDD, dataFormat::Real4);
  Serial.println("Internal Diameter of slave 18: " + (String)id18);
  
  delay(5000);

}