#include <ESP8266WiFi.h>


#include <PubSubClient.h>

#include "DHTesp.h"


const char* ssid = "CMCC-9Nkm";
const char* password = "Pm54j#Pm";

DHTesp dht;
char HumtAndTempBuff[24];//发送温湿度数据


WiFiClient espClient;
PubSubClient client(espClient);
const char* mqtt_server = "39.105.5.215";//服务器地址
const char*  topic_name = "Eagle_Watch";//订阅的主题
char msg[50];

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();


  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}













void setup() {
  Serial.begin(115200);
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  dht.setup(5, DHTesp::DHT11);        
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  client.connect("SmartHome");//连接MQTT
  client.subscribe(topic_name);

}



void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("SmartHome")) {
      Serial.println("connected");
      client.publish("Eagle_SmartHome", HumtAndTempBuff);
      client.subscribe(topic_name);
      client.setCallback(callback);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(1000);
    }
  }
}

void loop() {
  static uint32_t lastMsg = 0;
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (millis() - lastMsg > 5000) {
    lastMsg=millis();
    float humidity = dht.getHumidity();
    float temperature = dht.getTemperature();
    
    snprintf (HumtAndTempBuff, 24, "Humt:%.1f \%%\Temp:%.1f°C", humidity,temperature);
    Serial.println(HumtAndTempBuff);
    client.publish("Eagle_SmartHome", HumtAndTempBuff);
  }
}
