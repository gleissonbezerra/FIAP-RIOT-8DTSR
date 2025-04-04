#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""

#include <BlynkSimpleEsp32.h>

#define LED 12

BLYNK_WRITE(V0)
{
  // lê um valor que chegou através do DataStream V0
  int value = param.asInt();

  //Imprime o valor na serial
  Serial.println(value);

}

void setup()
{
  pinMode(LED, OUTPUT);

  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, "Wokwi-GUEST", "");
}

void loop()
{
  Blynk.run();
}