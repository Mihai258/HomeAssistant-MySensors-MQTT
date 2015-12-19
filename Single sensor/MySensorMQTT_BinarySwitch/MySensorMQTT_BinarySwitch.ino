// get SW_PIN digital status and send it to the gateway as "ON" or "OFF" status
#include <MySensor.h>
#include <SPI.h>
#include <Bounce2.h>

MyTransportNRF24 radio(RF24_CE_PIN, RF24_CS_PIN, RF24_PA_LEVEL_GW);
MyHwATMega328 hw;
MySensor gw(radio, hw);

#define NODE_ID 12

#define CHILD_ID_SW 1

#define SW_PIN 3

MyMessage msgSW(CHILD_ID_SW, V_TRIPPED);

Bounce debouncer = Bounce(); 
int oldValueSW=-1;
int valueSW=0;
const unsigned long tUpdate=120000; //update interval if input do not change
unsigned long t0;

void setup() {
  gw.begin(NULL, NODE_ID, true);
  gw.sendSketchInfo("Switch Sensor", "1.0");
  pinMode(SW_PIN,INPUT);
  digitalWrite(SW_PIN,HIGH);
  debouncer.attach(SW_PIN);
  debouncer.interval(5);
  gw.present(CHILD_ID_SW, S_DOOR);
}

void loop() {
  debouncer.update();
  valueSW = debouncer.read();
 
  if (valueSW != oldValueSW) {
     ServerUpdate();
     oldValueSW = valueSW;
  }
  if ((millis()-t0) > tUpdate) ServerUpdate();
}

void ServerUpdate() {
  gw.process();
  gw.send(msgSW.set(valueSW==HIGH ? "ON" : "OFF"));
  t0=millis();
}

