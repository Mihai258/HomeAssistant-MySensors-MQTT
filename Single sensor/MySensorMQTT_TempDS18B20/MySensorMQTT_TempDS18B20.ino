// temperature sensor with DS18B20 (3)
#include <MyTransportNRF24.h>
#include <MySensor.h>
#include <SPI.h>
#include <DallasTemperature.h>
#include <OneWire.h>
MyTransportNRF24 radio(RF24_CE_PIN, RF24_CS_PIN, RF24_PA_LEVEL_GW);
MyHwATMega328 hw;
MySensor gw(radio, hw);

#define NODE_ID 13

#define CHILD_ID_T1 1
#define CHILD_ID_T2 2
#define CHILD_ID_T3 3

#define ONE_WIRE_BUS 2
#define TEMPERATURE_PRECISION 12
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

MyMessage msgT1(CHILD_ID_T1, V_TEMP);
MyMessage msgT2(CHILD_ID_T2, V_TEMP);
MyMessage msgT3(CHILD_ID_T3, V_TEMP);

/* 
Addresses must be changed according to your sensors, otherwise it doesn't work!
This can be done using the sketch Tester.pde from Arduino Library for Dallas Temperature ICs, which can be found at https://github.com/milesburton/Arduino-Temperature-Control-Library
*/
DeviceAddress addrTemp1 = { 0x28, 0xFF, 0x57, 0x98, 0x81, 0x15, 0x02, 0x17 };
DeviceAddress addrTemp2 = { 0x28, 0xFF, 0xF1, 0x55, 0xA2, 0x15, 0x04, 0xB1 };
DeviceAddress addrTemp3 = { 0x28, 0xFF, 0xF1, 0x55, 0xA2, 0x15, 0x04, 0xB2 };

float Temp1, Temp2, Temp3;
const unsigned long tUpdate=60000; // update interval
unsigned long t0;

void setup() {
  gw.begin(NULL, NODE_ID, true);
  gw.sendSketchInfo("Temp", "1.0");
  gw.present(CHILD_ID_T1, S_TEMP);
  gw.present(CHILD_ID_T2, S_TEMP);
  gw.present(CHILD_ID_T3, S_TEMP);
  sensors.begin();
  sensors.setResolution(addrTemp1, TEMPERATURE_PRECISION);
  sensors.setResolution(addrTemp2, TEMPERATURE_PRECISION);
  sensors.setResolution(addrTemp3, TEMPERATURE_PRECISION);  
}

void loop() {
  gw.process();
  if ((millis()-t0) > tUpdate) ServerUpdate();
}

void ServerUpdate() {
  sensors.requestTemperatures();
  Temp1 = sensors.getTempC(addrTemp1);
  Temp2 = sensors.getTempC(addrTemp2);  
  Temp3 = sensors.getTempC(addrTemp3); 
  
  gw.send(msgT1.set(Temp1,1));
  gw.send(msgT2.set(Temp2,1));
  gw.send(msgT3.set(Temp3,1));
  
  t0=millis();
}
void incomingMessage(const MyMessage &message) {

}
