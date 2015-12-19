// temperature sensor using DHT 11 or DHT22
#include <MyTransportNRF24.h>
#include <MySensor.h>
#include <SPI.h>
#include <DHT.h>  

MyTransportNRF24 radio(RF24_CE_PIN, RF24_CS_PIN, RF24_PA_LEVEL_GW);
MyHwATMega328 hw;
MySensor gw(radio, hw);

#define NODE_ID 15

#define CHILD_ID_HUM 1
#define CHILD_ID_TEMP 2
#define HUMIDITY_SENSOR_DIGITAL_PIN 3

MyMessage msgHum(CHILD_ID_HUM, V_HUM);
MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);

DHT dht;

float Temp, Hum;
const unsigned long tUpdate=60000; // update interval
unsigned long t0;

void setup() {
  gw.begin(NULL, NODE_ID, true);
  gw.sendSketchInfo("TempDHT", "1.0");
  gw.present(CHILD_ID_HUM, S_HUM);
  gw.present(CHILD_ID_TEMP, S_TEMP);
  dht.setup(HUMIDITY_SENSOR_DIGITAL_PIN); 
}

void loop() {
  gw.process();
  if ((millis()-t0) > tUpdate) ServerUpdate();
}

void ServerUpdate() {
  Hum = dht.getHumidity();
  Temp = dht.getTemperature();
  gw.send(msgTemp.set(Temp, 1));
  gw.send(msgHum.set(Hum, 1));
  t0=millis();
}
void incomingMessage(const MyMessage &message) {

}
