/* ModbusLib library by Scott Alexander Gray
 */

#include "ModbusLib.h"



/**
 * Constructor.
 */
ModbusLib::ModbusLib()
{
  // be sure not to call anything that requires hardware be initialized here, put those in begin()
}

/**
 * Example method.
 */
void ModbusLib::begin()
{
    // initialize hardware
    Serial.println("called begin");
}

/**
 * Example method.
 */
void ModbusLib::process()
{
    // do something useful
    Serial.println("called process");
    doit();
}

/**
* Example private method
*/
void ModbusLib::doit()
{
    Serial.println("called doit");
}
