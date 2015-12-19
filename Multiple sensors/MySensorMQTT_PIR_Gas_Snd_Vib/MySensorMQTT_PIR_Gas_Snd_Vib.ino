// get status from several inputs and send them to the gateway
#include <MySensor.h>
#include <SPI.h>
MyTransportNRF24 radio(RF24_CE_PIN, RF24_CS_PIN, RF24_PA_LEVEL_GW);
MyHwATMega328 hw;
MySensor gw(radio, hw);

#define NODE_ID 19

#define CHILD_ID_PIR 1
#define CHILD_ID_BINARY 2
#define CHILD_ID_GAS 3
#define CHILD_ID_SOUND 4
#define CHILD_ID_VIBRATION 5

#define PIR_PIN 2
#define BINARY_PIN 3
#define MQ_PIN A0
#define SOUND_PIN 5
#define VIBRATION_PIN 6

MyMessage msgPir(CHILD_ID_PIR, V_TRIPPED);
MyMessage msgBin(CHILD_ID_BINARY, V_STATUS);
MyMessage msgGas(CHILD_ID_GAS, V_VAR1);
MyMessage msgSnd(CHILD_ID_SOUND, V_TRIPPED);
MyMessage msgVib(CHILD_ID_VIBRATION, V_TRIPPED);

byte StatePIR=0;
byte oldStatePIR=0;
byte StateBINARY=0;
byte oldStateBINARY=0;
byte StateWATER=0;
byte oldStateWATER=0;
byte StateSOUND=0;
byte oldStateSOUND=0;
byte StateVIBRATION=0;
byte oldStateVIBRATION=0;
int InAnalog1;
int valMQ=0;
int refMQ=0; // refference value will be the lowest value read from the sensor

const unsigned long tUpdate=60000; // update the digital input status at this interval
unsigned long tSetup=30000; // warming time
unsigned long t0;

void setup() {
  gw.begin(NULL, NODE_ID, true);
  gw.sendSketchInfo("MultiSensor", "1.0");
  
  pinMode(PIR_PIN, INPUT_PULLUP);
  pinMode(BINARY_PIN, INPUT_PULLUP);
  pinMode(SOUND_PIN, INPUT_PULLUP);
  pinMode(VIBRATION_PIN, INPUT_PULLUP);
  
  gw.present(CHILD_ID_PIR, S_MOTION);
  gw.present(CHILD_ID_BINARY, S_MOTION);
  gw.present(CHILD_ID_GAS, S_CUSTOM);
  gw.present(CHILD_ID_SOUND, S_MOTION);
  gw.present(CHILD_ID_VIBRATION, S_MOTION);
  
  t0=millis();
}

void loop() {
  gw.process();
  // digital PIN status update if they change:
  StatePIR=digitalRead(PIR_PIN);
  if (StatePIR != oldStatePIR) {
    oldStatePIR=StatePIR;
    gw.send(msgPir.set(StatePIR ? "ON" : "OFF"));
  }
  
  StateBINARY=digitalRead(BINARY_PIN);
  if (StateBINARY != oldStateBINARY) {
    oldStateBINARY=StateBINARY;
    gw.send(msgBin.set(StateBINARY ? "ON" : "OFF"));
  }
  
  StateSOUND=digitalRead(SOUND_PIN);
  if (StateSOUND != oldStateSOUND) {
    oldStateSOUND=StateSOUND;
    gw.send(msgSnd.set(StateSOUND ? "ON" : "OFF"));
  }
  
  StateVIBRATION=digitalRead(VIBRATION_PIN);
  if (StateVIBRATION != oldStateVIBRATION) {
    oldStateVIBRATION=StateVIBRATION;
    gw.send(msgVib.set(StateVIBRATION ? "ON" : "OFF"));
  }

  if ((millis()-t0) > tUpdate) ServerUpdate();
  
}

void ServerUpdate() {
  gw.send(msgPir.set(StatePIR ? "ON" : "OFF"));
  gw.send(msgBin.set(StateBINARY ? "ON" : "OFF"));
  gw.send(msgSnd.set(StateSOUND ? "ON" : "OFF"));
  gw.send(msgVib.set(StateVIBRATION ? "ON" : "OFF"));

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
  
  t0=millis();
}

