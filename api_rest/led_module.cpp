#include "globals.h"
#include "led_module.h"
#include "sensors_module.h"

void updateLedStateA() {
  if (globalUserControl) {
    // Mode Manuel
    if (ledOnOffA == "on") {
      digitalWrite(LED_PIN_RED, HIGH);
      ledStateA = HIGH;
    } else {
      digitalWrite(LED_PIN_RED, LOW);
      ledStateA = LOW;
    }
    preferences.putInt("led_A_State", ledStateA);
    return;
  }

  // Mode Automatique
  bool shouldTurnOn = false;
  if(autoModeTemp && ledOnOffA_Auto == "on") {
    shouldTurnOn = verifyThresholdActivation(true, false);
  }

  if (shouldTurnOn && ledStateA == LOW) {
    digitalWrite(LED_PIN_RED, HIGH);
    ledStateA = HIGH;
    preferences.putInt("led_A_State", ledStateA);
    previousLoopTimeA = millis();
  }
  else if (!shouldTurnOn && ledStateA == HIGH) {
    digitalWrite(LED_PIN_RED, LOW);
    ledStateA = LOW;
    preferences.putInt("led_A_State", ledStateA);
    previousLoopTimeA = millis();
  }
}

void updateLedStateB() {
  if (globalUserControl) {
    // Mode Manuel
    if (ledOnOffB == "on") {
      digitalWrite(LED_PIN_GREEN, HIGH);
      ledStateB = HIGH;
    } else {
      digitalWrite(LED_PIN_GREEN, LOW);
      ledStateB = LOW;
    }
    preferences.putInt("led_B_State", ledStateB);
    return;
  }

  // Mode Automatique
  bool shouldTurnOn = false;
  if(autoModeLight && ledOnOffB_Auto == "on") {
    shouldTurnOn = verifyThresholdActivation(false, true);
  }

  if (shouldTurnOn && ledStateB == LOW) {
    digitalWrite(LED_PIN_GREEN, HIGH);
    ledStateB = HIGH;
    preferences.putInt("led_B_State", ledStateB);
    previousLoopTimeB = millis();
  }
  else if (!shouldTurnOn && ledStateB == HIGH) {
    digitalWrite(LED_PIN_GREEN, LOW);
    ledStateB = LOW;
    preferences.putInt("led_B_State", ledStateB);
    previousLoopTimeB = millis();
  }
}
