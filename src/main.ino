#define BLYNK_TEMPLATE_ID "TMPL6H8Fu8hT7"
#define BLYNK_TEMPLATE_NAME "Test IoT"
#define BLYNK_AUTH_TOKEN "zI01DtH6HMNFQHg-dBu06FHtmtaAtrPA"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "Wokwi-GUEST";
char pass[] = "";

BLYNK_WRITE (V0) {
  int pinValue = param.asInt();
  digitalWrite(2, pinValue);

  Serial.print("V0 Slider value is: ");
  Serial.println((pinValue));
}

void setup () {
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop () {
  Blynk.run();
}