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
        Serial.print("Request Transmitted: ");
        for (int i = 0; i < 8; i++) //always 8 for modbus
        {		
            Serial.print(request[i], HEX);
            Serial.print(" ");		
        }
        Serial.println();
    }

    //Actually Sending Request--------------------------------------------
    digitalWrite(txEnablePin, HIGH); //set the transmit pin high 
    delay(30);

	if(deviceSerialPort.availableForWrite())
	{
		for (int i = 0; i < 8; i++) 
		{
			deviceSerialPort.write(request[i]);		 			
		}        
	}
    deviceSerialPort.flush(); //flush the serial conection which actually sends the command

    digitalWrite(txEnablePin, LOW); //set the transmit pin low    
}


uint8_t * ModbusLib::waitForResponse()
{
    digitalWrite(txEnablePin, LOW); //set the transmit pin low (to receive data)
    delay(30);

    static uint8_t modbusReply[9]; //buffer to store response (could optimise by making length 9)	

	readStartTime = millis(); //the milliseconds since boot

	int index = 0;

	while(millis() < (readStartTime + readTimeoutMillis))
	{
		//SerialMon.println("Stuck here for tick");
		if(deviceSerialPort.available())
		{
			modbusReply[index] = deviceSerialPort.read();			
			index++;
		}
		else
		{
			break;
		}		
	}
    if(Serial.availableForWrite() && debugEnabled)
    {
        Serial.print("Response Received: ");
        for (int i = 0; i < 8; i++) //always 8 for modbus
        {		
            Serial.print(modbusReply[i], HEX);
            Serial.print(" ");		
        }
        Serial.println();
    }
    return modbusReply;	
}


double ModbusLib::readRegisterValue(int slaveNetworkAddress, uint8_t registerAddress, dataFormat conversionMethod) //returning as long as a long integer contains more natural precision than float
{
    //create request
    uint8_t requestBuffer[8];

    requestBuffer[0] = slaveNetworkAddress;//slave addy
    requestBuffer[1] = readHoldingRegistersFunction;//function
    requestBuffer[2] = 0x00;//start addy hi
    requestBuffer[3] = registerAddress; //start addy lo
    requestBuffer[4] = 0x00; //qty addy hi
    requestBuffer[5] = 0x02; //qty addy lo
    requestBuffer[6] = GetCrcLo(requestBuffer); //crc lo
    requestBuffer[7] = GetCrcHi(requestBuffer); //crc hi

    //send request
    sendRequest(requestBuffer);

    //get response
    uint8_t * response = waitForResponse();

    //convert response
    long result = 0; //the converted value

    //Serial.println("Reached Conversion...");

    switch(conversionMethod)
    {
        case Integer: //integer
            
            break;
        case LongInteger: //long integer
            
            break;
        case Real4: //real4 / float            
            return real4Conversion(response);
            break;
    }

    //return zero value
    return result;
}

//settings change functions--------------------------------------------------------------------------------------
void ModbusLib::SetDebugMode(bool var)
{
    debugEnabled = var;
}

//Utility Functions----------------------------------------------------------------------------------------------
float ModbusLib::real4Conversion(uint8_t message[9])
{
    uint8_t data[4];
	data[0] = message[3]; //8 bits
	data[1] = message[4]; //8 bits
	data[2] = message[5]; //8 bits
	data[3] = message[6]; //8 bits

//combine into one value
	int64_t binVal = 0; 
	binVal = binVal | ((uint32_t)data[0] << 24);
	binVal = binVal | ((uint32_t)data[1] << 16);
	binVal = binVal | ((uint32_t)data[2] << 8);
	binVal = binVal | ((uint32_t)data[3] << 0);

//print for checking	
	//SerialMon.print("Data Bits: ");
	//SerialMon.println(binVal, BIN);

//implicit conversion of the c langauge method
	float value = *(float *)&binVal;
	//taking the address of binVal,
	//casts the address value to a pointer to a float (so now it 'is' a float reference)
	//then when we dereference the pointer to get the value it reinterprets it as a float (and since floats are stored in the IEEE 754 way, it returns the correct value)

    Serial.print("Actual Value: ");
	Serial.println(value, DEC);
	Serial.println();


    return value;
}







uint8_t ModbusLib::GetCrcLo(uint8_t message[]) //returns the CRC low byte
{
    int len = 6; //number of message bytes
	uint16_t crc = 0xFFFF; //starting value of CRC

	for (int pos = 0; pos < len; pos++) 
  	{
    	crc ^= (uint16_t)message[pos];          // XOR byte into least sig. byte of crc
  
		for (int i = 8; i != 0; i--) 
		{    // Loop over each bit
			if ((crc & 0x0001) != 0) 
			{      // If the LSB is set
				crc >>= 1;          // Shift right and XOR 0xA001
				crc ^= 0xA001;
			}
			else
			{               // Else LSB is not set
			crc >>= 1;
			}               // Just shift right
		}
	}

	//split crc and append to message
	return (uint8_t)crc; //the uint8_t trims the crc to 8 bits (2 hex values)
	
}

uint8_t ModbusLib::GetCrcHi(uint8_t message[]) //returns the CRC high byte
{
    int len = 6; //number of message bytes
	uint16_t crc = 0xFFFF;

	for (int pos = 0; pos < len; pos++) 
  	{
    	crc ^= (uint16_t)message[pos];          // XOR byte into least sig. byte of crc
  
		for (int i = 8; i != 0; i--) 
		{    // Loop over each bit
			if ((crc & 0x0001) != 0) 
			{      // If the LSB is set
				crc >>= 1;          // Shift right and XOR 0xA001
				crc ^= 0xA001;
			}
			else
			{               // Else LSB is not set
			crc >>= 1;
			}               // Just shift right
		}
	}
	return (uint8_t)(crc>>8); //bit shifts right by 8 bits (1 byte) and then trims to 8 bit integer
}

