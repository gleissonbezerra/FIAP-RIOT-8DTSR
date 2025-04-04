#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""

#include <BlynkSimpleEsp32.h>

#include "DHT.h"

#define DHTPIN 13

#define DHTTYPE DHT22 

DHT dht(DHTPIN, DHTTYPE);

BlynkTimer timer;

void setup()
{
  dht.begin();

  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, "Wokwi-GUEST", "");

  timer.setInterval(1000L, myTimerEvent); //timer 1x por segundo

}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
 
  float t = dht.readTemperature();

  if (isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print(F("Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));

}

void loop()
{
  Blynk.run();
  timer.run();
}