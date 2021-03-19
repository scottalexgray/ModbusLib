#pragma once

/* ModbusLib library by Scott Alexander Gray
 */
#include "Particle.h" //required

enum dataFormat //enum for deciding what type of data conversion will be needed for each register
{
  Integer = 0,
  LongInteger = 1,
  Real4 = 3
};



// This is your main class that users will import into their application
class ModbusLib
{
public:
  /**
 * Constructor that takes sets up references to various serial streams and rates ETC
 * @arg deviceSerialNumber [Mandatory] serial number where to send requests and receive responses
 * @arg deviceBRate [Mandatory] baud rate of deivce
 * @arg debugSerial [Mandatory] debug serial port reference
 */
  ModbusLib(int deviceSerialNumber, int deviceBRate, int txPin, bool isDebugEnabled); //constructor for debugging
  ModbusLib(int deviceSerialNumber, int deviceBRate); //constructor for normal operation

  
  void begin(); //to be called in startup in main  

  double readRegisterValue(int slaveNetworkAddress, uint16_t registerAddress, dataFormat conversionMethod);

  void SetDebugMode(bool var);
  

private:
  
  //static constants
  static const int requestLen = 8; //length in bytes
  static const int responseLen = 9; //length in bytes

  //device variables
  USARTSerial deviceSerialPort = Serial1; //serial where requests are made and responses are read from
  int deviceSerialNum = 1;
  int deviceBaudRate = 9600; //baudrate at which to communicate
  int txEnablePin = 7; //D7

  //debug variables  
  bool debugEnabled = false;

  //waiting for response variables
  uint32_t readTimeoutMillis = 100;
  uint32_t readStartTime = 0;

  //modbus functions
  static const uint8_t readHoldingRegistersFunction = 0x03;

  //Functions
  void sendRequest(uint8_t request[]); 
  uint8_t * waitForResponse();

  //Utility Functions
  void ClearSerialReadBuffer(); //this clears the incoming buffer (takes 10ms) to reduce errors

  float real4Conversion(uint8_t message[9]);
  int intConversion(uint8_t message[9]);
  long longConversion(uint8_t message[9]);


  uint8_t GetCrcLo(uint8_t message[]);
  uint8_t GetCrcHi(uint8_t message[]);
};
