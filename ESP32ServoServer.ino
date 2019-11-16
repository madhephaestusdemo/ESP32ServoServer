#include "Arduino.h"
#include <ESP32Servo.h>
#include <SimplePacketComs.h>
#include <WiFi.h>
#include <PacketEvent.h>
#include <Esp32WifiManager.h>
#include <Esp32SimplePacketComs.h>
#include <Preferences.h>
#include <wifi/WifiManager.h>
#include <server/NameCheckerServer.h>
#include "src/ServoServer.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include "src/GetIMU.h"
// SImple packet coms implementation useing WiFi
UDPSimplePacket coms;
// WIfi stack managment state machine
WifiManager manager;
//The setup function is called once at startup of the sketch
String * name = new String("kevkat");
Adafruit_BNO055 bno;
GetIMU * sensor;
Preferences preferences;
void setup()
{
	manager.setupAP();
	//manager.setupScan();
	sensor = new GetIMU();
	Serial.println("Loading with name: "+name[0]);

	//Initialise the sensor
	if (bno.begin()) {
		delay(1000);
		bno.setExtCrystalUse(true);
		sensor->startSensor(&bno);
	}else{
		Serial.print(
				"Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
	}

	coms.attach(new NameCheckerServer(name));
	coms.attach(sensor);
	coms.attach(new ServoServer());
}

// The loop function is called in an endless loop
void loop()
{
	sensor->loop();
	manager.loop();
	coms.server();
}
