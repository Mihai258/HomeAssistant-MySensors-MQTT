// distance measurement with ultrasonic sensor HC-SR04, send distance to the gateway
#include <MySensor.h>
#include <SPI.h>

MyTransportNRF24 radio(RF24_CE_PIN, RF24_CS_PIN, RF24_PA_LEVEL_GW);
MyHwATMega328 hw;
MySensor gw(radio, hw);

#define NODE_ID 17

#define CHILD_ID_DIST 1

const byte trig = 3; // attach pin 3 to Trig
const byte echo = 4; //attach pin 4 to Echo

MyMessage msgDist(CHILD_ID_DIST, V_DISTANCE);

const unsigned long tUpdate=1000; // update distance data time interval
unsigned long t0;
long duration, cm, inches; // only cm used here

void setup() {
  gw.begin(NULL, NODE_ID, true);
  gw.sendSketchInfo("Distance", "1.0");
  gw.present(CHILD_ID_DIST, S_DISTANCE);
  pinMode(trig, OUTPUT);
  pinMode(echo,INPUT);
  t0=millis();
}

void loop() {
  if ((millis()-t0) > tUpdate) ServerUpdate();
}

void ServerUpdate() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(5);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  //inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
  if (cm>400) cm=399;
  gw.send(msgDist.set(cm));
  t0=millis();
}

long microsecondsToInches(long microseconds)
{
return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
return microseconds / 29 / 2;
}

