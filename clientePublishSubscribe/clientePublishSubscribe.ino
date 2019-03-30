#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>

const char* ssid = "Nome do Wifi";
const char* password = "senha";
const char* mqtt_server = "IP do servidor";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
Adafruit_BME280 bme;
float temperature = 0;
float humidity = 0;

#ifdef __cplusplus
extern "C" {
#endif
uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
long measurement = 0;
int i;
double temp = 0;
unsigned long time1,time2;
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(LED_BUILTIN, OUTPUT);
}

void callback(char* topic, byte* message, unsigned int length) {
  time2=millis();
  Serial.println(time2-time1);//Imprimi o tempo 
}
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("esp32")) {
      Serial.println("connected");
      client.subscribe("esp32/temperature");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    temp = (temprature_sens_read() - 32) / 1.8;
    char tempString[8];
    dtostrf(temp, 1, 2, tempString);
    time1=millis();
    client.publish("esp32/temperature", tempString);
  }
}
