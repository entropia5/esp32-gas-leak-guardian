#include <Arduino.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(19, 18, 5, 23, 14, 2);

const byte MQ_PIN = 34;
const byte RED_LED = 13;
const byte GREEN_LED = 27;
const byte BUZZER_PIN = 12;

const int ALARM_ON = 3500;
const int ALARM_OFF = 2700;
const float FILTER_K = 0.2;

float filteredValue = 0;
bool isAlarmActive = false;
unsigned long lastUpdate = 0;
unsigned long lastBlink = 0;
unsigned long lastDisplayUpdate = 0;

void setup()
{
  Serial.begin(115200);

  analogSetAttenuation(ADC_11db);

  lcd.begin(16, 1);
  lcd.setCursor(0, 0);
  lcd.print("  START SYSTEM  ");

  pinMode(MQ_PIN, INPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  ledcAttach(BUZZER_PIN, 300, 8);

  filteredValue = analogRead(MQ_PIN);

  delay(1500);
  lcd.clear();
}

void loop()
{
  unsigned long currentMillis = millis();

  if (currentMillis - lastUpdate >= 50)
  {
    int rawValue = analogRead(MQ_PIN);

    filteredValue = filteredValue + FILTER_K * (rawValue - filteredValue);
    lastUpdate = currentMillis;

    Serial.print("Raw:");
    Serial.print(rawValue);
    Serial.print(" Filtered:");
    Serial.println(filteredValue);
  }

  if (currentMillis - lastDisplayUpdate >= 250)
  {
    lastDisplayUpdate = currentMillis;
    int val = (int)filteredValue;

    lcd.setCursor(0, 0);

    if (isAlarmActive)
    {
      lcd.print("GAS! ");
    }
    else
    {
      lcd.print("Search..");
    }

    lcd.print(val);
    lcd.print("     ");
  }

  if (filteredValue > ALARM_ON)
  {
    isAlarmActive = true;
  }
  else if (filteredValue < ALARM_OFF)
  {
    isAlarmActive = false;
  }

  if (isAlarmActive)
  {
    digitalWrite(GREEN_LED, LOW);
    if (currentMillis - lastBlink >= 300)
    {
      lastBlink = currentMillis;
      bool ledState = !digitalRead(RED_LED);
      digitalWrite(RED_LED, ledState);
      if (ledState)
        ledcWrite(BUZZER_PIN, 1);
      else
        ledcWrite(BUZZER_PIN, 0);
    }
  }
  else
  {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    ledcWrite(BUZZER_PIN, 0);
  }
}