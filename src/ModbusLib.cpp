/* ModbusLib library by Scott Alexander Gray
 */

#include "ModbusLib.h"

//serial or usbserial addded by default
//serial 1 included by default
#include "Serial2/Serial2.h"
#include "Serial4/Serial4.h"
#include "Serial5/Serial5.h"

/**
 * Constructor that takes sets up references to various serial streams and rates ETC
 * @arg deviceSerialNumber [Mandatory] serial number where to send requests and receive responses
 * @arg deviceBRate [Mandatory] baud rate of deivce
 * @arg debugSerial [Mandatory] debug serial port reference
 */
ModbusLib::ModbusLib(int deviceSerialNumber, int deviceBRate, int txPin, bool isDebugEnabled)
{
    //device stuff
    deviceSerialNum = deviceSerialNumber;
    deviceBaudRate = deviceBRate;
    txEnablePin = txPin;

    //debug stuff
    debugEnabled = isDebugEnabled;

}
ModbusLib::ModbusLib(int deviceSerialNumber, int deviceBRate)
{
  // be sure not to call anything that requires hardware be initialized here, put those in begin()
    deviceSerialNum = deviceSerialNumber;
    deviceBaudRate = deviceBRate;

    debugEnabled = false; //not strictly necessary as the default is disabled
}


void ModbusLib::begin()
{
    if(debugEnabled)
    {
        Serial.println("called begin");
        Serial.println("DevicePortNumber: " + deviceSerialNum);
        Serial.println("DevicePort Baud Rate: " + deviceBaudRate);
        Serial.println("Transmit Enable Pin: " + txEnablePin);
    }
    

    //initialise device serial
    switch(deviceSerialNum)
    {
        case 1:
            deviceSerialPort = Serial1;
            break;
        case 2:
            deviceSerialPort = Serial2;
            break;
        case 4:
            deviceSerialPort = Serial4;
            break;
        case 5:
            deviceSerialPort = Serial5;
            break;
    }
    deviceSerialPort.begin(deviceBaudRate);

    //initialise transmit enable pin
    pinMode(txEnablePin, OUTPUT);   
    
}



void ModbusLib::sendRequest(uint8_t request[])
{
    

    //Debugging------------------------------------------------------
    if(Serial.availableForWrite() && debugEnabled)
    {
        Serial.print("Command Transmitted: ");
        for (int i = 0; i < 8; i++) //always 8 for modbus
        {		
            Serial.print(request[i], HEX);
            Serial.print(" ");		
        }
        Serial.println();
    }

    //Actually Sending Request--------------------------------------------
    digitalWrite(txEnablePin, HIGH); //set the transmit pin high 

	if(deviceSerialPort.availableForWrite())
	{
		for (int i = 0; i < 8; i++) 
		{
			deviceSerialPort.write(request[i]);		 			
		}        
	}
    deviceSerialPort.flush(); //flush the serial conection which actually sends the command

    digitalWrite(txEnablePin, LOW); //set the transmit pin low

    //test reponse to ensure the above works---------------------------------------------------
     
    
}


uint8_t * ModbusLib::waitForResponse()
{
    static uint8_t modbusReply[9]; //buffer to store response (could optimise by making length 9)
	
	digitalWrite(txEnablePin, LOW); //set the transmit pin low (to receive data)

	readStartTime = millis(); //the milliseconds since boot

	int index = 0;

	//Serial.print("Response: ");

	while(millis() < (readStartTime + readTimeoutMillis))
	{
		//SerialMon.println("Stuck here for tick");
		if(deviceSerialPort.available())
		{
			modbusReply[index] = deviceSerialPort.read();			
			index++;
			//Serial.print(modbusReply[index-1], HEX);
			//Serial.print(" ");

		}
		else
		{
			break;
		}		
	}

    return modbusReply;
	
}


void ModbusLib::readHoldingRegisters(int deviceNetworkAddress, uint16_t registerAddress, uint8_t registerValues[])
{
    int responseLen = 8;
    uint8_t responseBuffer[responseLen];






}

/*void ModbusLib::SendRequestTest()
{

    uint8_t hardcodedRequest[8] = {15, 0x03, 0x00, 0xDD, 0x00, 0x02, 0x55, 0x1F};

    Serial4.begin(9600);
    delay(200);

    pinMode(7, OUTPUT);    
    digitalWrite(7, HIGH);    

    if(Serial4.availableForWrite())
	{
		for (int i = 0; i < 8; i++) 
		{
			Serial4.write(hardcodedRequest[i]);		 			
		}
	}
    Serial4.flush();

    digitalWrite(7, LOW);
}*/