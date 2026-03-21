#include <Arduino.h>

const int mqSensorPin = 34;
const int redLed = 13;
const int greenLed = 27;
const int buzzerPin = 12;

int alarmOn = 1500;
int alarmOff = 1200;
bool isAlarmActive = false;

void setup()
{
  Serial.begin(115200);
  pinMode(mqSensorPin, INPUT);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop()
{

  int rawValue = 0;
  for (int i = 0; i < 10; i++)
  {
    rawValue += analogRead(mqSensorPin);
    delay(10);
  }
  int currentLevel = rawValue / 10;

  Serial.print("Газ: ");
  Serial.println(currentLevel);

  if (currentLevel > alarmOn)
  {
    isAlarmActive = true;
  }
  else if (currentLevel < alarmOff)
  {
    isAlarmActive = false;
  }

  if (isAlarmActive)
  {
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);

    analogWrite(buzzerPin, 50);
    delay(200);
    analogWrite(buzzerPin, 0);
    delay(200);
  }
  else
  {

    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, HIGH);
    analogWrite(buzzerPin, 0);
  }

  delay(200);
}