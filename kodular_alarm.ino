#include <WiFi.h>  // Untuk ESP32
// #include <ESP8266WiFi.h> // Untuk ESP8266
#include <MQTT.h>
//#include <NusabotSimpleTimer.h>

const char ssid[] = "..";
const char pass[] = "...";

WiFiClient net;
MQTTClient client;
//NusabotSimpleTimer timer;

const int buttonPin = 2;
bool Tbpres = false;

void subscribe(String &topic, String &payload){
  // if(topic == "nusabot2/adf"){
  //   if(payload == "true"){
  //     digitalWrite(25, 1);
  //     Serial.println("Lampu Menyala");
  //   } else {
  //     digitalWrite(25, 0);
  //     Serial.println("Lampu Mati");
  //   }
  // }
}

void publish(){//
int statusTb = digitalRead(buttonPin);

  if (statusTb == HIGH && !Tbpres) {
    Tbpres = true;
    if (client.connected()) {
      client.publish("nusabot2/sensor", ("Alarm berbunyi"));
    }
  } 
  
  else if (statusTb == LOW && Tbpres) {
    Tbpres = false;
    if (client.connected()) {
      client.publish("nusabot2/sensor", ("Alarm mati"));
    }
  }

  client.loop(); 
}

void connect(){     // Menghubungkan ke WiFi dan Broker
  Serial.print("Menghubungkan ke WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
Serial.println("Berhasil Terhubung ke WiFi");
 //menghubungkan ke broker
  Serial.print("Menghubungkan ke Broker"); 
  while (!client.connect("nusa")) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Terhubung ke Broker");
  client.subscribe("nusabot2/data");
}


void setup() {
  WiFi.begin(ssid, pass);
  client.begin("broker.emqx.io", net);
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);

  // timer.setInterval(10, publish);
  client.onMessage(subscribe);

  connect();
}

void loop() {
  // timer.run();
  publish();
  client.loop();
}
