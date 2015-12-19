// read the light value from the KY018 sensor and send it to the gateway as Lux value
// refference: http://greppipelinux.synology.me/?p=248
#include <MySensor.h>
#include <SPI.h>
MyTransportNRF24 radio(RF24_CE_PIN, RF24_CS_PIN, RF24_PA_LEVEL_GW);
MyHwATMega328 hw;
MySensor gw(radio, hw);

#define NODE_ID 18

#define CHILD_ID_LIGHT 1

#define LIGHT_PIN A0

MyMessage msgLight(CHILD_ID_LIGHT, V_LIGHT_LEVEL);

const unsigned long tUpdate=60000; // update interval
unsigned long t0;

void setup() {
  gw.begin(NULL, NODE_ID, true);
  gw.sendSketchInfo("Light Sensor", "1.0");
  gw.present(CHILD_ID_LIGHT, S_LIGHT_LEVEL);
  t0=millis();
}

void loop() {
  gw.process();
  if ((millis()-t0) > tUpdate) ServerUpdate();
}

void ServerUpdate() {
  gw.send(msgLight.set(f_KY018(),0));
  t0=millis();
}

float f_KY018() {
       const float RES_0 = 10.0; // R in Kohm sensor
       int t_raw = 0;
       float volts = 0;
       float lux = 0;
 
       t_raw = 1023 - analogRead(LIGHT_PIN);
       volts = t_raw * 0.0048828125;
       lux = 500.0 / (RES_0 * ((5.0 - volts) / volts));
       return(lux);
 } 
