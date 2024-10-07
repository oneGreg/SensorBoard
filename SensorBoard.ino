/*#https://github.com/Gbertaz/NonBlockingDallas/blob/master/examples/AdditionalFunctions/AdditionalFunctions.ino*/
/**
 * @brief Usage/Test of additional functions for lib NonBlockingDallas
 * 
 * @author oOpen <git@logisciel.com>
 * 
 * License: MIT
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files(the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions :
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

//DS1820 != DS18B20 
#include "library/config.h"
#include "library/version.h"
#include "library/build_info.h"
int getVersion(){
  return version;
}

char * getBuildInfo(){
  return build_info;
}
void printConfig(){
  Serial.print(F("DEBUG;"));
  Serial.println("printConfig");

  INFO();
  Serial.print(F("BAUD_RATE:"));
  Serial.println(BAUD_RATE);
  INFO();
  Serial.print(F("ONE_WIRE_BUS:"));  
  Serial.println(ONE_WIRE_BUS);
  INFO();
  Serial.print(F("TIME_INTERVAL:"));
  Serial.println(TIME_INTERVAL);
  INFO();
}
void INFO(){
  Serial.print(F("INFO;"));
}

// ----------------------------------------

#include <OneWire.h>
#include <DallasTemperature.h>
#include <NonBlockingDallas.h>

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature dallasTemp(&oneWire);
NonBlockingDallas temperatureSensors(&dallasTemp);

bool oneTimeFnCalled = false;
long  receivingCounter = 0;

void handleTemperatureChange(int deviceIndex, int32_t temperatureRAW)
{
  Serial.print(F("DEBUG;"));
  Serial.println("handleTemperatureChange");

  receivingCounter=receivingCounter+1;
  Serial.print(F("deviceIndex="));
  Serial.print(deviceIndex);
  Serial.print(F(" | RAW="));
  Serial.print(temperatureRAW);
  Serial.print(F(" | "));
  Serial.print(temperatureSensors.rawToCelsius(temperatureRAW));
}

//Invoked when the sensor reading fails
void handleDeviceDisconnected(int deviceIndex)
{
  Serial.print(F("DEBUG;"));
  Serial.println("handleDeviceDisconnected");

  Serial.print(F("Disconnected deviceIndex="));
  Serial.println(deviceIndex);
}

//Invoked at every VALID sensor reading. "valid" parameter will be removed in a feature version
void handleIntervalElapsed(int deviceIndex, int32_t temperatureRAW)
{


  receivingCounter=receivingCounter+1;
  #ifdef DEBUG
  Serial.print(F("DEBUG;"));
  Serial.println(F("receivingCounter: "));
  #endif
  
  Serial.print(F("DATA;"));

  #ifdef LOG_MILLIS
  Serial.print(millis()/1000);
  Serial.print(';');
  #endif
  Serial.print(deviceIndex);
  Serial.print(F(";"));
  Serial.print(temperatureRAW);
  Serial.print(F(";"));
  float temprature = temperatureSensors.rawToCelsius(temperatureRAW);

  temprature = round(temprature);
  int temp = (temprature * 1) / 1;
  Serial.print(temprature);
  //Serial.println(F("°C | "));
  Serial.println();
  /*
   *  DO SOME AMAZING STUFF WITH THE TEMPERATURE
   */
}

void setup() {  
  Serial.begin(BAUD_RATE);
  while (!Serial)
    ;
  //    
  Serial.println(""); 
  Serial.println(""); 
  Serial.println(""); 
  Serial.println(""); 
  INFO();    
  Serial.println("SensorBoard"); 
  //
  INFO();  
  Serial.print("Version="); 
  Serial.println(getVersion()); 
  //
  INFO();
  Serial.print("BuildInfo="); 
  Serial.println(getBuildInfo());
  //
  printConfig();
  //  
  Serial.print(F("DEBUG;"));
  Serial.println("begin of setup");



  // resolution_12
  temperatureSensors.begin(NonBlockingDallas::resolution_9, TIME_INTERVAL);
  //temperatureSensors.onTemperatureChange(handleTemperatureChange);
  temperatureSensors.onIntervalElapsed(handleIntervalElapsed);
  temperatureSensors.onDeviceDisconnected(handleDeviceDisconnected);

  //temperatureSensors.requestTemperature();


  Serial.println("end of setup");

}

void loop()
{  
 temperatureSensors.update();

 // only call one time this function after tow seconds
 if(oneTimeFnCalled == false && millis() > MAGIC_2000) {
  oneTimeFnCalled = true;
  testAdditionalsFunctions();
 }
 
 // non blockking delay for checkking receivingCounter
 /*
 delay(20);
 if(receiving){
  Serial.println("+");
 } else {
  Serial.println("-");
 };
 receiving = false;
 */
 /*
 if (temperatureSensors.indexExist(temperatureSensors.getSensorsCount() - 1))
    Serial.println(F(" YES")); // YES if there is one sensor at least
  else
    Serial.println(F(" NO"));
}
*/
}
void testAdditionalsFunctions()
{

  Serial.println();
  Serial.println();
  #ifdef DEBUG
  Serial.print(F("DEBUG;"));
  Serial.print(F("DeviceCount: "));
  #endif

  INFO();
  Serial.print(F("SensorsCount="));
  Serial.println(temperatureSensors.getSensorsCount());

  DeviceAddress deviceAddresses[temperatureSensors.getSensorsCount()];
  String stringAddresses[temperatureSensors.getSensorsCount()];
  for (size_t i = 0; i < temperatureSensors.getSensorsCount(); i++)
  {
    INFO();
    Serial.print(F("[Sensor index="));
    Serial.print(i);
    Serial.println(F("]"));
    INFO();
    Serial.print(temperatureSensors.getTemperatureRAW(i));
    Serial.print(F(" °C="));
    Serial.print(temperatureSensors.getTemperatureC(i));
    temperatureSensors.getDeviceAddress(i, deviceAddresses[i]);
    Serial.print(F(" * convertDeviceAddressToString(deviceAddresses["));
    Serial.print(i);
    Serial.print(F("]) ==> "));    
    Serial.println(temperatureSensors.convertDeviceAddressToString(deviceAddresses[i]));
    INFO();
    Serial.println(F(" * Store address String representation in stringAddresses[]"));
    INFO();
    stringAddresses[i] = temperatureSensors.getAddressString(i);
    Serial.print(F(" * Sensor index="));
    Serial.print(i);
    Serial.print(F(" ==> "));
    Serial.println(stringAddresses[i]);
    /*
    Serial.println(F(" * Convert address String representation to DeviceAdress with convertDeviceAddressStringToDeviceAddress(stringAddresses[i], tmpAddress)"));
    DeviceAddress tmpAddress;
    temperatureSensors.convertDeviceAddressStringToDeviceAddress(stringAddresses[i], tmpAddress);
    Serial.print(F(" * get back converted to strig temperatureSensors.convertDeviceAddressToString(tmpAddress) ==> "));
    Serial.println(temperatureSensors.convertDeviceAddressToString(tmpAddress));
    Serial.print(F(" * Compare converted String with DeviceAddress using compareTowDeviceAddresses(tmpAddress, deviceAddresses[i]) ==> "));
    if (temperatureSensors.compareTowDeviceAddresses(tmpAddress, deviceAddresses[i]))
      Serial.println(F("Match !"));
    else
      Serial.println(F("Don't match !")); // never call here


    */
    Serial.println();
  }

  /**
    * Validate range by DeviceAddress
    */
  INFO();
  Serial.print(F("validateAddressesRange(deviceAddresses, 2) ==> "));
  if (temperatureSensors.validateAddressesRange(deviceAddresses, 2))
    Serial.println(F(" Yes"));
  else
    Serial.println(F(" NO"));

  INFO();
  Serial.print(F("validateAddressesRange(deviceAddresses, 2, false) ==> "));
  if (temperatureSensors.validateAddressesRange(deviceAddresses, 2, false))
    Serial.println(F(" Yes"));
  else
    Serial.println(F(" NO"));

  // Add +1 fake/missing address
  DeviceAddress rangeInvalide[temperatureSensors.getSensorsCount() + 1];
  // copy your valide addresses
  memcpy(
      rangeInvalide,
      deviceAddresses,
      sizeof(deviceAddresses[0]) * temperatureSensors.getSensorsCount());
  // create a fake address
  for (size_t i = 0; i < 8; i++)
    rangeInvalide[temperatureSensors.getSensorsCount()][i] = i;

  INFO();
  Serial.print(F("validateAddressesRange(rangeInvalide, temperatureSensors.getSensorsCount()+1) ==> "));
  if (temperatureSensors.validateAddressesRange(rangeInvalide, temperatureSensors.getSensorsCount() + 1))
    Serial.println(F(" Yes"));
  else
    Serial.println(F(" NO"));

  INFO();
  Serial.print(F("validateAddressesRange(rangeInvalide, temperatureSensors.getSensorsCount()+1, false) ==> "));
  if (temperatureSensors.validateAddressesRange(rangeInvalide, temperatureSensors.getSensorsCount() + 1, false))
    Serial.println(F(" Yes"));
  else
    Serial.println(F(" NO"));

  /**
    * Validate range by string
    */


  INFO();
  Serial.print(F("validateAddressesRange(stringAddresses, temperatureSensors.getSensorsCount()) ==> "));
  if (temperatureSensors.validateAddressesRange(stringAddresses, temperatureSensors.getSensorsCount()))
    Serial.println(F(" Yes"));
  else
    Serial.println(F(" NO"));

  INFO();
  Serial.print(F("validateAddressesRange(stringAddresses, temperatureSensors.getSensorsCount(), false) ==> "));
  if (temperatureSensors.validateAddressesRange(stringAddresses, temperatureSensors.getSensorsCount(), false))
    Serial.println(F(" Yes"));
  else
    Serial.println(F(" NO"));

  // Add +1 fake/missing address
  String invalideStringAddresses[temperatureSensors.getSensorsCount() + 1];
  // copy your valide addresses
  for (size_t i = 0; i < temperatureSensors.getSensorsCount(); i++)
    invalideStringAddresses[i] = stringAddresses[i];
  // create a fake address
  invalideStringAddresses[temperatureSensors.getSensorsCount()] = "0123456789abcdef";

  INFO();
  Serial.print(F("validateAddressesRange(invalideStringAddresses, temperatureSensors.getSensorsCount()+1) ==> "));
  if (temperatureSensors.validateAddressesRange(invalideStringAddresses, temperatureSensors.getSensorsCount() + 1))
    Serial.println(F(" Yes"));
  else
    Serial.println(F(" NO"));

  INFO();
  Serial.print(F("validateAddressesRange(invalideStringAddresses, temperatureSensors.getSensorsCount()+1, false) ==> "));
  if (temperatureSensors.validateAddressesRange(invalideStringAddresses, temperatureSensors.getSensorsCount() + 1, false))
    Serial.println(F(" Yes"));
  else
    Serial.println(F(" NO"));

  /**
    * Map know sensors position index
    */

  int8_t mapPositionIndex[temperatureSensors.getSensorsCount()];
  INFO();
  Serial.print(F("mapIndexPositionOfDeviceAddressRange(deviceAddresses, temperatureSensors.getSensorsCount(), mapPositionIndex) ==> "));
  temperatureSensors.mapIndexPositionOfDeviceAddressRange(deviceAddresses, temperatureSensors.getSensorsCount(), mapPositionIndex);
  for (size_t i = 0; i < temperatureSensors.getSensorsCount(); i++)
  {
    
    Serial.print(F(" [index="));
    Serial.print(i);
    Serial.print(F(" position="));
    Serial.print(mapPositionIndex[i]);
    Serial.print(F("]"));
  }
  INFO();
  Serial.println();

  // use invalide range
  int8_t invalideMapPositionIndex[temperatureSensors.getSensorsCount() + 1];
  INFO();
  Serial.print(F("mapIndexPositionOfDeviceAddressRange(rangeInvalide, temperatureSensors.getSensorsCount()+1, invalideMapPositionIndex) ==> "));
  temperatureSensors.mapIndexPositionOfDeviceAddressRange(rangeInvalide, temperatureSensors.getSensorsCount() + 1, invalideMapPositionIndex);
  for (size_t i = 0; i < temperatureSensors.getSensorsCount() + 1; i++)
  {
    Serial.print(F(" [index="));
    Serial.print(i);
    Serial.print(F(" position="));
    Serial.print(invalideMapPositionIndex[i]);
    Serial.print(F("]"));
  }
  Serial.println();

  // make a reversed invalide range
  DeviceAddress reversedRangeInvalide[temperatureSensors.getSensorsCount() + 1];
  for (size_t i = 0; i < temperatureSensors.getSensorsCount() + 1; i++)
    for (size_t j = 0; j < 8; j++)
      reversedRangeInvalide[i][j] = rangeInvalide[temperatureSensors.getSensorsCount() - i][j];

  int8_t reversedInvalideMapPositionIndex[temperatureSensors.getSensorsCount() + 1];
  INFO();
  Serial.print(F("mapIndexPositionOfDeviceAddressRange(reversedRangeInvalide, temperatureSensors.getSensorsCount() + 1, reversedInvalideMapPositionIndex) ==> "));
  temperatureSensors.mapIndexPositionOfDeviceAddressRange(reversedRangeInvalide, temperatureSensors.getSensorsCount() + 1, reversedInvalideMapPositionIndex);
  for (size_t i = 0; i < temperatureSensors.getSensorsCount() + 1; i++)
  {
    Serial.print(F(" [index="));
    Serial.print(i);
    Serial.print(F(" position="));
    Serial.print(reversedInvalideMapPositionIndex[i]);
    Serial.print(F("]"));
  }
  Serial.println();
  Serial.println();
  Serial.println();
}

