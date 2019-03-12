#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS D4
// to be changed
// the number that you connect with ds18b20
// the number can be activated is case by case
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
ESP8266WiFiMulti WiFiMulti;

void setup() {
  Serial.begin(115200);
  delay(10);

  // to be changed
  // We start by connecting to a WiFi network
  // Wifi name and the password
  WiFiMulti.addAP("helloworld", "jmjmjm1234");

  Serial.println();
  Serial.println();
  Serial.print("Wait for WiFi... ");

  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(500);
  Serial.println("Dallas Temperature");
  sensors.begin();
}


void loop() {
  const uint16_t port = 80;
  const char * host = "api.thingspeak.com"; // ip or dns
  String apiKey = "MGT3HJLAUHRHOVDX"; // to be changed
  // make account on thingspeak
  // write apikey of channel

  WiFiClient client;

  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    return;
  }
  sensors.requestTemperatures();
  Serial.print("Temperature for Device 1 is: ");
  String temp = String(sensors.getTempCByIndex(0));
  Serial.println(temp);
  delay(500);
  String url = "/update?api_key=" + apiKey + "&field1=" + temp;
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  delay(60000); // should request after 15 seconds
  int timeout = millis() + 20000;
  while (client.available() == 0) {
    if (timeout - millis() < 0) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
}

