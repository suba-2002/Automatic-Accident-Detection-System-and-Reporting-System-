#include "timer.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);

#define rx 7
#define tx 8

SoftwareSerial NodeSerial (rx, tx);

const int IN1 = 5;
const int IN2 = 6;

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();
int acc, bcc, gcc, wcc = 0;
int accident;
int x, y, z;
Timer timer;
int Speed, speeds;
const int LM393 = 2;
int counter = 0;
int C;
int Gas, gas, fl;
int rpm;

void setup()
{
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Vehicle Tracking");
  lcd.setCursor(0, 1);
  lcd.print("    System    ");
  attachInterrupt(digitalPinToInterrupt(LM393), count, RISING);
  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  accel.begin();
  Serial.begin(9600);
  NodeSerial.begin(9600);
  timer.setInterval(1000);
  timer.setCallback(RPM);
  timer.start();
  if (!accel.begin())
  {
    Serial.println("No valid sensor found");
    while (1);
  }
    delay(10000);
}

void count() {
  counter++;
}

void RPM() {
  C = counter ;
  counter = 0;
}

void loop() {
  timer.update();
  sensors_event_t event;
  accel.getEvent(&event);
  x = event.acceleration.x;
  y = event.acceleration.y;
  z = event.acceleration.z;
  speeds = analogRead(A0);
  Speed = map(speeds, 0, 1023, 0, 255);
  rpm = map(Speed, 0, 255, 0, 100);
  Gas = analogRead(A1) / 10.24;

  if ((x >= -2 && x <= 2) && (y >= -2 && y <= 2) && (z >= 6 && z <= 12))
  {
    acc = 0;
  }
  
  else
  {
    acc = 1;
  }
  if (acc == 0)
  {
    bcc++;
    
    delay(1000);
  }
  else if (acc == 1)
  {
    gcc++;
   
    delay(1000);
  }
  accident = bcc + gcc;
  if (accident == 15)
  {
    if (gcc >= 12)
    {
      Serial.println("Accident detected");
      wcc = 1;
   
    }
    else
    {
      Serial.println("No Accident detected");
      wcc = 0;
    }
    bcc = 0;
    gcc = 0;
    accident = 0;
  }

  if (Gas >= 70  )
  {
    Speed = 0;
    rpm = map(Speed, 0, 255, 0, 100);
    analogWrite(IN1, Speed);
    gas = 1;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  Fuel Leakage  ");
    lcd.setCursor(0, 1);
    lcd.print("    Detected    ");
    Serial.print("A"); Serial.print(wcc);
    Serial.print("G"); Serial.print(gas);
    Serial.print("R"); Serial.print(Speed);
    Serial.print("#"); Serial.println();

  }
  else if (wcc == 1)
  {
    Speed = 0;
    rpm = map(Speed, 0, 255, 0, 100);
    analogWrite(IN1, Speed);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("    Accident    ");
    lcd.setCursor(0, 1);
    lcd.print("  Has  occured  ");
    Serial.print("A"); Serial.print(wcc);
    Serial.print("G"); Serial.print(gas);
    //  Serial.print("S"); Serial.print(Speed);
    Serial.print("R"); Serial.print(Speed);
    Serial.print("#"); Serial.println();

  }
  else
  {
    if (Speed > 215) {
      fl++;
    }
    else if (Speed < 215)
    {
      fl = 0;
      analogWrite(IN1, Speed);
    }
    if (fl > 10)
    {
      Speed = 153;
      rpm = map(Speed, 0, 255, 0, 100);
      analogWrite(IN1, Speed);
      Serial.print("A"); Serial.print(wcc);
      Serial.print("G"); Serial.print(gas);
      Serial.print("R"); Serial.print(Speed);
      Serial.print("#"); Serial.println();
    }
    else
    {
      rpm = map(Speed, 0, 255, 0, 100);
      analogWrite(IN1, Speed);
      Serial.print("A"); Serial.print(wcc);
      Serial.print("G"); Serial.print(gas);
      Serial.print("R"); Serial.print(Speed);
      Serial.print("#"); Serial.println();
      gas  = 0;
      //A0G0R0#
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" Vehicle  Speed ");
    lcd.setCursor(0, 1);
    lcd.print(rpm);
  }
  delay(1000);
}
