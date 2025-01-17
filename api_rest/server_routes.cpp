#include "globals.h"
#include "cors_module.h"
#include "server_routes.h"
#include "sensors_module.h"

// ----------------------------------------------------------------------------
// Handlers GET
// ----------------------------------------------------------------------------
void handleGetLight(){
  addCorsHeaders();

  json_send_obj.clear();
  
  int light_value = analogRead(LIGHT_SENSOR_PIN);
  json_send_obj["type"]  = "lumiere";
  json_send_obj["value"] = light_value;
  serializeJson(json_send_obj, json_send);

  server.send(200, "application/json", json_send);
}

void handleGetTemperature() {
  addCorsHeaders();

  json_send_obj.clear();

  double resValue = analogRead(THERMISTOR_PIN);
  Serial.print("Thermistor raw: ");
  Serial.println(resValue);
  tft.print("Thermistor raw: ");
  tft.println(resValue);

  float T = A_coef + B_coef * log(resValue) + C_coef * pow(log(resValue), 3);
  float temperatureK = 1.0 / T;
  float temperatureC = temperatureK - 273.15 - 53.0;

  Serial.print("Measured Temperature: ");
  Serial.println(temperatureC);
  tft.print("Measured Temp (C): ");
  tft.println(temperatureC);

  json_send_obj["type"]  = "temperature";
  json_send_obj["value"] = temperatureC;
  serializeJson(json_send_obj, json_send);

  server.send(200, "application/json", json_send);
}

void handleGetLedA(){
  addCorsHeaders();

  json_send_obj.clear();
  json_send_obj["type"]  = "ledA";
  json_send_obj["value"] = ledOnOffA;
  json_send_obj["speed"] = ledIntervalA;
  
  serializeJson(json_send_obj, json_send);
  server.send(200, "application/json", json_send);
}

void handleGetLedB(){
  addCorsHeaders();

  json_send_obj.clear();
  json_send_obj["type"]  = "ledB";
  json_send_obj["value"] = ledOnOffB;
  json_send_obj["speed"] = ledIntervalB;
  
  serializeJson(json_send_obj, json_send);
  server.send(200, "application/json", json_send);
}

void handleGetLed(){
  addCorsHeaders();

  json_send_obj.clear();
  
  json_send_obj["typeA"]   = "ledA";
  json_send_obj["valueA"]  = ledOnOffA;
  json_send_obj["speedA"]  = ledIntervalA;
  json_send_obj["typeB"]   = "ledB";
  json_send_obj["valueB"]  = ledOnOffB;
  json_send_obj["speedB"]  = ledIntervalB;
  
  serializeJson(json_send_obj, json_send);
  server.send(200, "application/json", json_send);
}

void handleGetAuto(){
  addCorsHeaders();

  json_send_obj.clear();
  
  json_send_obj["valueAutoA"]  = ledOnOffA_Auto;
  json_send_obj["speedAutoA"]  = ledIntervalA_Auto;
  json_send_obj["valueAutoB"]  = ledOnOffB_Auto;
  json_send_obj["speedAutoB"]  = ledIntervalB_Auto;
  json_send_obj["seuilL"]      = lightThreshold;
  json_send_obj["seuilT"]      = tempThreshold;
  json_send_obj["auto_temp"]   = autoModeTemp;
  json_send_obj["auto_light"]  = autoModeLight;
  
  serializeJson(json_send_obj, json_send);
  server.send(200, "application/json", json_send);
}

/**
 * @brief Nouvelle route GET: /ledGlobalMode
 * Retourne le mode global actuel : "manual" ou "auto"
 */
void handleGetLedGlobalMode() {
  addCorsHeaders();

  json_send_obj.clear();
  json_send_obj["mode"] = globalUserControl ? "manual" : "auto";
  serializeJson(json_send_obj, json_send);

  server.send(200, "application/json", json_send);
}

// ----------------------------------------------------------------------------
// Handlers POST (LED A / B, auto, thresholds)
// ----------------------------------------------------------------------------
void handlePostLedChangeA() {
  addCorsHeaders();

  if (server.hasArg("value") && server.hasArg("speed")) {
    String value = server.arg("value");
    int speed    = server.arg("speed").toInt();

    ledOnOffA = value;
    ledIntervalA = speed;
    preferences.putString("led_A_on_off", ledOnOffA);
    preferences.putInt("led_A_Interval", ledIntervalA);

    // Mise à jour immédiate de l'état de la LED en mode manuel
    if (globalUserControl) {
      if (ledOnOffA == "on") {
        digitalWrite(LED_PIN_RED, HIGH);
        ledStateA = HIGH;
      } else {
        digitalWrite(LED_PIN_RED, LOW);
        ledStateA = LOW;
      }
      preferences.putInt("led_A_State", ledStateA);
    }

    server.send(200, "application/json", "{}");
  } else {
    server.send(400, "text/plain", "Requête invalide. Paramètres 'value' et 'speed' manquants.");
  }
}

void handlePostLedChangeB() {
  addCorsHeaders();

  if (server.hasArg("value") && server.hasArg("speed")) {
    String value = server.arg("value");
    int speed    = server.arg("speed").toInt();

    ledOnOffB = value;
    ledIntervalB = speed;
    preferences.putString("led_B_on_off", ledOnOffB);
    preferences.putInt("led_B_Interval", ledIntervalB);

    // Mise à jour immédiate de l'état de la LED en mode manuel
    if (globalUserControl) {
      if (ledOnOffB == "on") {
        digitalWrite(LED_PIN_GREEN, HIGH);
        ledStateB = HIGH;
      } else {
        digitalWrite(LED_PIN_GREEN, LOW);
        ledStateB = LOW;
      }
      preferences.putInt("led_B_State", ledStateB);
    }

    server.send(200, "application/json", "{}");
  } else {
    server.send(400, "text/plain", "Requête invalide. Paramètres 'value' et 'speed' manquants.");
  }
}

void handlePostLedChange() {
  addCorsHeaders();

  if ( server.hasArg("valueA") && server.hasArg("speedA") &&
       server.hasArg("valueB") && server.hasArg("speedB") ) {
    String valueA = server.arg("valueA");
    int speedA    = server.arg("speedA").toInt();

    String valueB = server.arg("valueB");
    int speedB    = server.arg("speedB").toInt();

    ledOnOffA = valueA;
    ledIntervalA = speedA;
    preferences.putString("led_A_on_off", ledOnOffA);
    preferences.putInt("led_A_Interval", ledIntervalA);

    ledOnOffB = valueB;
    ledIntervalB = speedB;
    preferences.putString("led_B_on_off", ledOnOffB);
    preferences.putInt("led_B_Interval", ledIntervalB);

    // Mise à jour immédiate en mode manuel
    if (globalUserControl) {
      if (ledOnOffA == "on") {
        digitalWrite(LED_PIN_RED, HIGH);
        ledStateA = HIGH;
      } else {
        digitalWrite(LED_PIN_RED, LOW);
        ledStateA = LOW;
      }
      preferences.putInt("led_A_State", ledStateA);

      if (ledOnOffB == "on") {
        digitalWrite(LED_PIN_GREEN, HIGH);
        ledStateB = HIGH;
      } else {
        digitalWrite(LED_PIN_GREEN, LOW);
        ledStateB = LOW;
      }
      preferences.putInt("led_B_State", ledStateB);
    }

    server.send(200, "application/json", "{}");
  } else {
    server.send(400, "text/plain", "Requête invalide. Paramètres 'valueA', 'speedA', 'valueB', 'speedB' manquants.");
  }
}

void handlePostAutoTemp(){
  addCorsHeaders();

  if (server.hasArg("auto")) {
    String auto_t = server.arg("auto");

    if (auto_t == "on"){
      autoModeTemp = true;
      ledIntervalA = ledIntervalA_Auto;
      ledOnOffA    = ledOnOffA_Auto;
    } else if (auto_t == "off") {
      autoModeTemp = false;
    } else {
      server.send(400, "text/plain", "Paramètre 'auto' invalide. Utilisez 'on' ou 'off'.");
      return;
    }
    
    preferences.putBool("auto_temp", autoModeTemp);
    server.send(200, "application/json", "{}");
  } else {
    server.send(400, "text/plain", "Requête invalide. Paramètre 'auto' manquant.");
  }
}

void handlePostAutoLight(){
  addCorsHeaders();

  if (server.hasArg("auto")) {
    String auto_l = server.arg("auto");

    if (auto_l == "on"){
      autoModeLight = true;
      ledIntervalB  = ledIntervalB_Auto;
      ledOnOffB     = ledOnOffB_Auto;
    } else if (auto_l == "off") {
      autoModeLight = false;
    } else {
      server.send(400, "text/plain", "Paramètre 'auto' invalide. Utilisez 'on' ou 'off'.");
      return;
    }

    preferences.putBool("auto_light", autoModeLight);
    server.send(200, "application/json", "{}");
  } else {
    server.send(400, "text/plain", "Requête invalide. Paramètre 'auto' manquant.");
  }
}

void handlePostAutoTempParams(){
  addCorsHeaders();

  if (server.hasArg("stateAuto") && server.hasArg("speedAuto")) {
    String stateAuto = server.arg("stateAuto");
    int speedAuto    = server.arg("speedAuto").toInt();

    if (stateAuto == "on" || stateAuto == "off") {
      ledOnOffA_Auto = stateAuto;
      ledIntervalA_Auto = speedAuto;
      
      preferences.putString("led_A_on_off_auto", ledOnOffA_Auto);
      preferences.putInt("led_A_Interval_auto", ledIntervalA_Auto);
      
      server.send(200, "application/json", "{}");
    } else {
      server.send(400, "text/plain", "Paramètre 'stateAuto' invalide. Utilisez 'on' ou 'off'.");
    }
  } else {
    server.send(400, "text/plain", "Requête invalide. Paramètres 'stateAuto' et 'speedAuto' manquants.");
  }
}

void handlePostAutoLightParams(){
  addCorsHeaders();

  if (server.hasArg("stateAuto") && server.hasArg("speedAuto")) {
    String stateAuto = server.arg("stateAuto");
    int speedAuto    = server.arg("speedAuto").toInt();

    if (stateAuto == "on" || stateAuto == "off") {
      ledOnOffB_Auto = stateAuto;
      ledIntervalB_Auto = speedAuto;
      
      preferences.putString("led_B_on_off_auto", ledOnOffB_Auto);
      preferences.putInt("led_B_Interval_auto", ledIntervalB_Auto);
      
      server.send(200, "application/json", "{}");
    } else {
      server.send(400, "text/plain", "Paramètre 'stateAuto' invalide. Utilisez 'on' ou 'off'.");
    }
  } else {
    server.send(400, "text/plain", "Requête invalide. Paramètres 'stateAuto' et 'speedAuto' manquants.");
  }
}

void handlePostThresholdTemp(){
  addCorsHeaders();

  if (server.hasArg("threshold")) {
    double threshold = server.arg("threshold").toDouble();
    tempThreshold = threshold;
    preferences.putDouble("temp_threshold", tempThreshold);
    server.send(200, "application/json", "{}");
  } else {
    server.send(400, "text/plain", "Requête invalide. Paramètre 'threshold' manquant.");
  }
}

void handlePostThresholdLight(){
  addCorsHeaders();

  if (server.hasArg("threshold")) {
    int threshold = server.arg("threshold").toInt();
    lightThreshold = threshold;
    preferences.putInt("light_threshold", lightThreshold);
    server.send(200, "application/json", "{}");
  } else {
    server.send(400, "text/plain", "Requête invalide. Paramètre 'threshold' manquant.");
  }
}

/**
 * @brief Nouvelle route POST: /ledGlobalMode
 * Paramètre: mode=auto ou mode=manual
 * => si mode=manual => globalUserControl = true
 * => si mode=auto   => globalUserControl = false
 */
void handlePostLedGlobalMode() {
  addCorsHeaders();
  if (server.hasArg("mode")) {
    String mode = server.arg("mode"); // "auto" ou "manual"
    if (mode == "manual") {
      globalUserControl = true;
      preferences.putBool("globalUserControl", true);
      Serial.println("Mode global défini sur MANUEL => l'utilisateur contrôle les LEDs");
      tft.println("Mode global: MANUEL");
    } else if (mode == "auto") {
      globalUserControl = false;
      preferences.putBool("globalUserControl", false);
      Serial.println("Mode global défini sur AUTOMATIQUE => les capteurs contrôlent les LEDs");
      tft.println("Mode global: AUTOMATIQUE");
    } else {
      server.send(400, "text/plain", "Mode invalide. Utilisez 'auto' ou 'manual'.");
      return;
    }
    server.send(200, "application/json", "{}");
  } else {
    server.send(400, "text/plain", "Requête invalide. Paramètre 'mode' manquant.");
  }
}

// ----------------------------------------------------------------------------
// Déclaration des routes serveur
// ----------------------------------------------------------------------------
void setupServerRoutes(){
  // Gérer OPTIONS pour toutes les routes
  server.on("/temperature",   HTTP_OPTIONS, handleCors);
  server.on("/light",         HTTP_OPTIONS, handleCors);
  server.on("/ledA",          HTTP_OPTIONS, handleCors);
  server.on("/ledB",          HTTP_OPTIONS, handleCors);
  server.on("/led",           HTTP_OPTIONS, handleCors);
  server.on("/ledAutoParams", HTTP_OPTIONS, handleCors);
  server.on("/ledChangeA",    HTTP_OPTIONS, handleCors);
  server.on("/ledChangeB",    HTTP_OPTIONS, handleCors);
  server.on("/ledChange",     HTTP_OPTIONS, handleCors);
  server.on("/autoManTemp",   HTTP_OPTIONS, handleCors);
  server.on("/autoManLight",  HTTP_OPTIONS, handleCors);
  server.on("/autoManTempParams",  HTTP_OPTIONS, handleCors);
  server.on("/autoManLightParams", HTTP_OPTIONS, handleCors);
  server.on("/thresholdTemp",  HTTP_OPTIONS, handleCors);
  server.on("/thresholdLight", HTTP_OPTIONS, handleCors);
  server.on("/ledGlobalMode",  HTTP_OPTIONS, handleCors);

  // Handlers GET
  server.on("/temperature",   HTTP_GET, handleGetTemperature);
  server.on("/light",         HTTP_GET, handleGetLight);
  server.on("/ledA",          HTTP_GET, handleGetLedA);
  server.on("/ledB",          HTTP_GET, handleGetLedB);
  server.on("/led",           HTTP_GET, handleGetLed);
  server.on("/ledAutoParams", HTTP_GET, handleGetAuto);
  server.on("/ledGlobalMode", HTTP_GET, handleGetLedGlobalMode);

  // Handlers POST
  server.on("/ledChangeA",         HTTP_POST, handlePostLedChangeA);
  server.on("/ledChangeB",         HTTP_POST, handlePostLedChangeB);
  server.on("/ledChange",          HTTP_POST, handlePostLedChange);
  server.on("/autoManTemp",        HTTP_POST, handlePostAutoTemp);
  server.on("/autoManLight",       HTTP_POST, handlePostAutoLight);
  server.on("/autoManTempParams",  HTTP_POST, handlePostAutoTempParams);
  server.on("/autoManLightParams", HTTP_POST, handlePostAutoLightParams);
  server.on("/thresholdTemp",      HTTP_POST, handlePostThresholdTemp);
  server.on("/thresholdLight",     HTTP_POST, handlePostThresholdLight);
  server.on("/ledGlobalMode",      HTTP_POST, handlePostLedGlobalMode);

  // NotFound => handleCors (OPTIONS ou 404)
  server.onNotFound(handleCors);

  server.begin();
}
