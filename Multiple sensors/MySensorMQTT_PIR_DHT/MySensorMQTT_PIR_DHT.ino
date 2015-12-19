// motion + temperature DHT sensor
#include <MySensor.h>
#include <SPI.h>
#include <DHT.h> 
MyTransportNRF24 radio(RF24_CE_PIN, RF24_CS_PIN, RF24_PA_LEVEL_GW);
MyHwATMega328 hw;
MySensor gw(radio, hw);

#define NODE_ID 11

#define CHILD_ID_HUM 1
#define CHILD_ID_TEMP 2
#define CHILD_ID_PIR 3

#define PIR_PIN 2
#define HUMIDITY_SENSOR_DIGITAL_PIN 3

MyMessage msgHum(CHILD_ID_HUM, V_HUM);
MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);
MyMessage msgPir(CHILD_ID_PIR, V_TRIPPED);

DHT dht;

byte StatePIR=0;
byte oldStatePIR=0;
const unsigned long tUpdate=120000; // update interval if no motion detected
unsigned long t0;
float Temp, Hum;

void setup() {
  gw.begin(NULL, NODE_ID, true);
  gw.sendSketchInfo("Motion_DHT", "1.0");
  pinMode(PIR_PIN, INPUT_PULLUP);
  dht.setup(HUMIDITY_SENSOR_DIGITAL_PIN); 
  gw.present(CHILD_ID_HUM, S_HUM);
  gw.present(CHILD_ID_TEMP, S_TEMP);
  gw.present(CHILD_ID_PIR, S_MOTION);
  t0=millis();

}

void loop() {
  gw.process();
  StatePIR=digitalRead(PIR_PIN);
  if (StatePIR != oldStatePIR) {
    oldStatePIR=StatePIR;
    gw.send(msgPir.set(StatePIR ? "ON" : "OFF"));
  }
  if ((millis()-t0) > tUpdate) ServerUpdate();
  
}

void ServerUpdate() {
  Hum = dht.getHumidity();
  Temp = dht.getTemperature();
  gw.send(msgTemp.set(Temp, 1));
  gw.send(msgHum.set(Hum, 1));  
  gw.send(msgPir.set(StatePIR ? "ON" : "OFF"));
  t0=millis();
}

void incomingMessage(const MyMessage &message) {

}

