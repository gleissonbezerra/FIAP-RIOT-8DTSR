#include "Servo.h"
#include "Wire.h"

#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define SERVO_PIN 6 // Digital pin connected to the DHT sensor

#define SAMPLE_TIME 200 // PID interval in miliseconds

#define SET_POINT 50 //
#define START_TEMPERATURE 30

#define VALVE_OFFSET 90
#define WARMUP_RATE 0.05
#define COOLDOWN_RATE 0.01

Servo valveServo; // Servo controller

int currentPos = 0; // Servo position 0 = midpoint

float kp = 5, ki = 2.5, kd = 0.01;

long time, startTime;
long et;

float dt, error = 0, pError = 0, integralError = 0, derivativeError = 0;
float currentTemp = START_TEMPERATURE, pid;


void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  Serial.println("SP, PV");

  valveServo.attach(SERVO_PIN);
  valveServo.write(VALVE_OFFSET + currentPos);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.cp437(true);  // Use full 256 char 'Code Page 437' font

  // Clear the buffer
  display.clearDisplay();


  for ( int i=0; i < 3; ++i)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);

  }

  startTime = millis();
  randomSeed(startTime);
}
 

void loop() 
{
  
  et = millis() - startTime;

  if ( et > SAMPLE_TIME)
  {
    startTime = millis();

    dt = (float)et/1000;
    error = SET_POINT - currentTemp;

    integralError = integralError + (float)error * dt;
    derivativeError = (float)(error - pError)/dt;

    pError = error;

    pid = kp*error + ki * integralError + kd * derivativeError;

    currentPos = VALVE_OFFSET - (int)(pid);
    currentPos = constrain(currentPos,0,180);

    valveServo.write(currentPos);

    currentTemp += (pid * WARMUP_RATE) - (currentTemp - START_TEMPERATURE) * COOLDOWN_RATE;

  }

  if ( random(100) > 85 && (int)(millis()/1000) % 15 == 0 )
  {
    currentTemp += random(20);
    currentTemp -= random(10);
  }

  Serial.print(SET_POINT);
  Serial.print(" ");
  Serial.println(currentTemp);

  display.clearDisplay();
  display.setCursor(35, 20);
  display.print(currentTemp);
  display.display();

  
}
