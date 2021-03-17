/*
 * Project usage
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

}


void loop() 
{
  //send request
  uint8_t hardcodedRequest[8] = {15, 0x03, 0x00, 0xDD, 0x00, 0x02, 0x55, 0x1F};
  modbusLib.sendRequest(hardcodedRequest);

  //get response
  uint8_t * response = modbusLib.waitForResponse();

  //print response to check
  Serial.print("Response: ");
  for(int i = 0; i< 9; i++)
  {
    Serial.print(*(response + i), HEX);
    Serial.print(" ");
  }
  Serial.println();
  
  delay(5000);

}