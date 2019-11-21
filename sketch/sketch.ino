#include <WiFi.h>
#include <PubSubClient.h>

#include "config.h"

#define LED 2

WiFiClient wifiClient;
PubSubClient client(wifiClient);

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  Serial.begin(115200);

  connectToWifi();
  connectToMQTTBroker();
  client.subscribe(mqttTopic);
  client.setCallback(callback);
}

void callback(char* topic, byte* payload, unsigned int length) {
  if ((char)payload[0] == '1') {
    digitalWrite(LED, HIGH);    
  } else {
    digitalWrite(LED, LOW);
  }
  return;
}

void connectToWifi() {
  Serial.println("Connecting to wifi network");
   //  Resolve bug de conexão
  WiFi.setSleep(false);
  WiFi.begin(wifiSSID, wifiPassword);

  while (WiFi.status() != WL_CONNECTED) {      
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connection sucefully stablished to network!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void connectToMQTTBroker() {
  client.setServer(mqttServer, mqttPort);
  
  while (!client.connected()) {
    Serial.println("Connecting to MQTT broker");
    if (client.connect("esp32Client", mqttUser, mqttPassword)) {
      Serial.println("Connection sucefully stablished to MQTT broker!");
      Serial.println("Waiting for incoming messages.");      
      client.subscribe(mqttTopic);
      client.setCallback(callback);
    
    } else {
      Serial.println("Connection failed!");
      Serial.print("Status code: ");
      Serial.print(client.state());
      Serial.println();
      delay(2000);
    }
  }
}

void loop() {
  client.loop();
}

//  digitalWrite(7, HIGH);
//  delay(3000);
//  digitalWrite(7, LOW);
//  delay(3000);
