// Get analog value from defined Analog_PIN and send it to the gateway
#include <MySensor.h>
#include <SPI.h>
MyTransportNRF24 radio(RF24_CE_PIN, RF24_CS_PIN, RF24_PA_LEVEL_GW);
MyHwATMega328 hw;
MySensor gw(radio, hw);

#define NODE_ID 14

#define CHILD_ID_Analog1 1

#define Analog_PIN A0

int InA1;

MyMessage msgPir(CHILD_ID_Analog1, V_VAR1);

const unsigned long tUpdate=60000; //update interval in mS
unsigned long t0;

void setup() {
  gw.begin(NULL, NODE_ID, true);
  gw.sendSketchInfo("Analog Sensor", "1.0");
  gw.present(CHILD_ID_Analog1, S_CUSTOM);
  t0=millis();

}

void loop() {
  gw.process();
  if ((millis()-t0) > tUpdate) ServerUpdate();
}

void ServerUpdate() {
  InA1=analogRead(Analog_PIN);
  gw.send(msgPir.set(InA1));
  t0=millis();
  
}

