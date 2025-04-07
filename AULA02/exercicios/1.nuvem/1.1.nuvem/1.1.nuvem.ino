#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL2yVi5EtYp"
#define BLYNK_TEMPLATE_NAME "APP01"
#define BLYNK_AUTH_TOKEN "fvR0KebzjSKyIl6rH26OJlZ_wHZO8MTB"

#include <BlynkSimpleEsp32.h>

void setup()
{
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, "GNET 2.4", "F23700623D8D24");
}

void loop()
{
  Blynk.run();
}