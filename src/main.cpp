#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <WiFi.h>
#include <MQTT.h>

const char ssid[] = "KakaPupu";
const char password[] = "nhuphuong17121997";

WiFiClient net;
MQTTClient client;

int scanTime = 5; 

BLEScan* pBLEScan;
unsigned long lastMillis = 0;

void connect(){

  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED){
  Serial.print(".");
  delay(1000);
  }
 
  Serial.print("\nconnecting...");
  while (!client.connect("arduinoESP32")) {
  Serial.print(".");
  delay(1000);
  }

  Serial.println("\nconnected!");
  client.subscribe("/clientSub");
  }

  void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  }
 

  class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
      void onResult(BLEAdvertisedDevice advertisedDevice){
       Serial.printf("Advertised Device: %s \n",advertisedDevice.toString().c_str());
    }
};


void setup(){
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED){
   delay(500);
   Serial.print(".");
   }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  
  Serial.println(WiFi.localIP());{
    client.begin("mqtt.eclipse.org", net);
    client.onMessage(messageReceived);
    connect();
    }
 
  Serial.println("Scanning...");
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); 
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); 
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);

}
void loop() 
{
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.print("Devices found: ");
  Serial.println(foundDevices.getCount());
  Serial.println("Scan done!");
  pBLEScan->clearResults(); 
  delay(2000);
  
client.loop();//kiểm tra lại kết nối
 delay(10); 

if (!client.connected()){
  connect();
  }
  
if (millis() - lastMillis > 6000){
    lastMillis = millis();
    client.publish("/clientPub", "foundDevices.getCount()");
  }
}