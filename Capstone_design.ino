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
String apiKey = "MGT3HJLAUHRHOVDX"; // to be changed
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

void senddata(int flag) {
  WiFiClient client;
  sensors.requestTemperatures();
  Serial.print("Temperature for Device 1 is: ");
  String temp = String(sensors.getTempCByIndex(0));
  Serial.println(temp);
  delay(500);
  const char * host; // ip or dns
  String url;
  uint16_t port;

  if(flag==0){
    host="api.thingspeak.com";
    port = 80;
    url = "/update?api_key=" + apiKey + "&field1=" + temp;
  }
  if(flag==1){
      host = "52.79.239.221";
      port = 8000;
      url = "/log?temp=" + temp; 
  }
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    return;
  }
  if(flag==1) {
    host = "http://52.79.239.221";
  }
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  Serial.println(host);
  delay(1000); // should request after 15 seconds
}

void loop() {
  // make account on thingspeak
  // write apikey of channel

  static long millisSec = millis();

  if (millis() - millisSec > 60000) {
    millisSec = millis();
    senddata(0);

    senddata(1);
  }
}

