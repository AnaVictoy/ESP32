
#include <WiFi.h>
const char* ssid = "Nome do Wifi";
const char* password = "senha";
const uint16_t port = 8090;
const char * host = "Ip do servidor";

#ifdef __cplusplus
extern "C" {
#endif
uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
long measurement = 0;
int i;
unsigned long time1,time2;
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("...");
  }

  Serial.print("WiFi connected with IP: ");
  Serial.println(WiFi.localIP());
  pinMode(LED_BUILTIN, OUTPUT);
}
void loop() {
  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("Connection to host failed");
    delay(1000);
    return;
  }

  digitalWrite(LED_BUILTIN, HIGH);
  delay(2000);
  digitalWrite(LED_BUILTIN, LOW);
  time1 = millis();
  client.print((temprature_sens_read() - 32) / 1.8);
  client.read();
  time2 = millis();
  Serial.println(time2-time1);
  measurement = 0;
  client.stop();
  delay(2000);
}
