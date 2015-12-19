// motion sensor
#include <MySensor.h>
#include <SPI.h>
MyTransportNRF24 radio(RF24_CE_PIN, RF24_CS_PIN, RF24_PA_LEVEL_GW);
MyHwATMega328 hw;
MySensor gw(radio, hw);

#define NODE_ID 11

#define CHILD_ID_PIR 1

#define PIR_PIN 2

MyMessage msgPir(CHILD_ID_PIR, V_TRIPPED);

byte StatePIR=0;
byte oldStatePIR=0;
const unsigned long tUpdate=120000; // update interval if no movement detected
unsigned long t0;

void setup() {
  gw.begin(NULL, NODE_ID, true);
  gw.sendSketchInfo("Motion Sensor", "1.0");
  pinMode(PIR_PIN, INPUT_PULLUP);
  gw.present(CHILD_ID_PIR, S_MOTION);
  t0=millis();

}

void loop() {
  gw.process();
  StatePIR=digitalRead(PIR_PIN);
  if (StatePIR != oldStatePIR) {
    oldStatePIR=StatePIR;
    ServerUpdate();
  }
  if ((millis()-t0) > tUpdate) ServerUpdate();
  
}

void ServerUpdate() {
  gw.send(msgPir.set(StatePIR ? "ON" : "OFF"));
  t0=millis();
  
}

