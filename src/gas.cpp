#include <Arduino.h>

const byte MQ_PIN = 34;
const byte RED_LED = 13;
const byte GREEN_LED = 27;
const byte BUZZER_PIN = 12;

const int ALARM_ON = 1600;
const int ALARM_OFF = 1400;
const float FILTER_K = 0.4;

const int BUZZ_FREQ = 400;
const int BUZZ_RES = 8; // 8 бит

float filteredValue = 0;
bool isAlarmActive = false;
unsigned long lastUpdate = 0;
unsigned long lastBlink = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(MQ_PIN, INPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  ledcAttach(BUZZER_PIN, BUZZ_FREQ, BUZZ_RES);

  Serial.println("Система запущена...");
  filteredValue = analogRead(MQ_PIN);
}

void loop()
{
  unsigned long currentMillis = millis();

  if (currentMillis - lastUpdate >= 50)
  {
    int rawValue = analogRead(MQ_PIN);
    filteredValue = filteredValue + FILTER_K * (rawValue - filteredValue);
    lastUpdate = currentMillis;

    Serial.print("Сырые данные:");
    Serial.print(rawValue);
    Serial.print(",Очищено:");
    Serial.println(filteredValue);
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
      {

        ledcWrite(BUZZER_PIN, 1);
      }
      else
      {
        ledcWrite(BUZZER_PIN, 0);
      }
    }
  }
  else
  {

    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    ledcWrite(BUZZER_PIN, 0);
  }
}
