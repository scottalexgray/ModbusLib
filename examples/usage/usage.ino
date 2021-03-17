// Example usage for ModbusLib library by Scott Alexander Gray.

#include "ModbusLib.h"

// Initialize objects from the lib
ModbusLib modbusLib;

void setup() {
    // Call functions on initialized library objects that require hardware
    modbusLib.begin();
}

void loop() {
    // Use the library's initialized objects and functions
    modbusLib.process();
}
