HomeAssistant with MySensors using MQTT

I was starting building this project as starting point of an automation home system with HomeAssistant and MySensors using MQTT protocol.


The Gateway

The Gateway is build with Arduino UNO + Ethernet Shield W5100. The connection with NRF24L+ radio module must be made as described on MySensors site at Ethernet Gateway build section.
The sketch for this gateway is MQTTGateway.ino from MySensors. I have modified it to get DHCP address from the router.
Before compile and upload this sketch you have to:
 - edit MyConfig.h found in .../libraries/MySensors/MyConfig.h folder and remove the commented slashes (//) before "#define SOFTSPI". Afterwards you have to revert to the original by adding the commented slashes (//) before "#define SOFTSPI";
  - modify the "byte TCP_MAC[]" to have unique MAC address inside your network.
After succesfully upload the sketch, you have to:
  - edit MyConfig.h found in .../libraries/MySensors/MyConfig.h folder - revert to the original by adding the commented slashes (//) before "#define SOFTSPI", as mentioned before;
  - assign a static IP address for your MQTT Gateway from your router interface based on the MAC; this IP will be used later on into the HomeAssistant configuration file;
  - change #define MQTT_BROKER_PREFIX to "M" (shorter is better, so I choose one letter).
One of the nicer part of this sketch is that it include also its own broker, so there is no need to install mosquitto.
  
Router

From your router interface assign a static IP address for your MQTT Gateway based on the MAC from MQTTGateway.ino, for example 192.168.1.101. You can check the right assignament by pinging the gateway:
ping 192.168.1.101.
If you choose other address or modify it later, no changes are needed on the gateway side, but you have to update the HomeAssistant configuration.yaml file in the mqtt section.


Sensors

For the sensors I have used the Arduino Nano + NRF24L+ module.
I have started to built single sensor modules, tested each of them individually, then combine several sensors into single module. 
The starting point is the "sensors build area" from MySensors site. 
Start with "Connection the radio", this part is exactly the same as presented on the site.
Then choose the sensor type according to your needs, find the relevant sketch into my repositories. Before upload it check and edit if necessary the:
  - node ID from #define NODE_ID
  - sensor PIN from #define sensor_PIN
  - update interval for the sensor: tUpdate
Usually there is no need to modify anything, just upload the sketch and you are ready! Some cases you need to made some modifications may be:
  - build several sensors of the same type (each of them must have their unique ID);
  - build your own module with choosen sensors;
  - change default PIN for sensor(s), may be for mechanical reason.

If you need any other sensors combination into one module, you can start from individual sensor sketch and add them into one sketch ino file, I hope my examples are clear enough to do this very easy.
If you reach the maximum size of the Arduino Nano memory, you have to disable DEBUG (check on MySensors site how to do this). Next step is to replace the Arduino Nano with UNO or even Mega.


HomeAssistant

If you choose the HomeAssistant controller, install the software as presented on their site at "Getting started" section, start the server with
$ hass --open-ui 
then stop it with Ctrl-C. In this moment you should have a default configuration.yaml file into your directory HOME/.homeassistant. If you do not see any .homeassistant dir, in the file browser go to HOME dir then press Ctrl-H.
You have to edit this configuration.yaml file and add your sensors there. I have one example of the configuration.yaml file that works with most of my single sensors and with many of combined sensors (for some of sensors examples small adjustements may have to be performed to match the sensor topic area: node ID, sensor ID, payload).
At mqtt broker you must also edit the IP address of your MQTT Gateway, in my example it is 192.169.1.101.


Learned from experience

While working and performed tests on this project, I encountered several issues I would like to mention here to pay attention on them:
  - if the gateway is not started before HomeAssistant, the sensors will be not shown into the web interface (this may change in the future with other HomeAssistant versions); so start the Gateway and afterward the HomeAssistant;
  - the payload for binary sensors must be letters or start with letters, not just numbers, for example ON / OFF, not 0 / 1 otherwise they don't work!
  - when starting HomeAssistant, it set all relays to OFF; I think it is possible to change that, I am looking for a solution;
  - the NRF24L+ does not offer large area radio coverage, but enough for one appartment; they are some radio improvement advices on the net;
  - Serial Gateway does not work for me, I understand from other users there could be a problem of pySerial, this was the reason I go for the MQTTGateway instead of SerialGateway; I have payed the price of one more component - the Ethernet Shield W5100 and some work for HomeAssistant configuration because the sensors and switches must be manually inserted, but then get a working system with higher flexibility;

What I have used

They are differences between platforms and software versions, so I mention here my setup that works for me:
  - Controller: HomeAssistant v.0.9.1 installed on LinuxMint 17.3, python 3.4 (default on LinuxMint), pip3
  - Gateway: Arduino Uno + Ethernet Shield W5100 + NRF24L+
  - sensors: Arduino Nano + NRF24L+ + sensor(s)
  - Arduino IDE 1.6.5

  
Refferences

Most part of this project is inspired from the following sites:
http://www.mysensors.org/
https://home-assistant.io/
https://www.arduino.cc/


I hope my work presented here will help other people to quickly implement their own projects and save their time.





