// get MQ2 sensor input and send it to the gateway
// before use the MQ sensor you may keep them powered for at least 24h
// when powered on, MQ sensors need some time to warm-up, I give them here 10 minutes
#include <MySensor.h>
#include <SPI.h>
MyTransportNRF24 radio(RF24_CE_PIN, RF24_CS_PIN, RF24_PA_LEVEL_GW);
MyHwATMega328 hw;
MySensor gw(radio, hw);

#define NODE_ID 16

#define CHILD_ID_GAS 1

#define MQ_PIN A0

MyMessage msgGas(CHILD_ID_GAS, V_VAR1);

int InAnalog1;
int valMQ=0;
int refMQ=0; // refference value will be the lowest value read from the sensor
const unsigned long tUpdate=60000; // update interval data
unsigned long tSetup=600000; // warming time; 
unsigned long t0;

void setup() {
  gw.begin(NULL, NODE_ID, true);
  gw.sendSketchInfo("GAS Sensor", "1.0");
  gw.present(CHILD_ID_GAS, S_CUSTOM);
  t0=millis();

}

void loop() {
  gw.process();
  if ((millis()-t0) > tUpdate) ServerUpdate();
}

void ServerUpdate() {
  InAnalog1=analogRead(MQ_PIN);
  if (tSetup) {
    refMQ=InAnalog1;
    if ((millis()-t0) > tSetup) tSetup=0; //warming time passed
  }
  else {
    if (InAnalog1<refMQ) refMQ=InAnalog1; // if read value is lower that actual, change the 0 refference with the actual one
    valMQ=InAnalog1-refMQ;
    t0=millis();
  }
  gw.send(msgGas.set(valMQ));
}

