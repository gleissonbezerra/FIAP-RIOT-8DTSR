#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""

#include <BlynkSimpleEsp32.h>

void setup()
{
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, "Wokwi-GUEST", "");
}

void loop()
{
  Blynk.run();
}