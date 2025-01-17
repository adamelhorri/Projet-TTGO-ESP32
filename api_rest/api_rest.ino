/**
 * @file api_rest_corrected_final.ino
 *
 * @mainpage Doxygen Projet d'IoT 
 *
 * @section description Description
 * Projet sur un ESP32 avec mode de contrôle global (manuel/auto) des LEDs.
 *
 * @section circuit Circuit
 * - LED Rouge connectée au pin 17.
 * - LED Verte connectée au pin 33.
 * - Thermistor connectée au pin 39.
 * - Photoresistor connectée au pin 36.
 *
 * @section libraries Bibliothèques
 * - TFT_eSPI (https://github.com/Bodmer/TFT_eSPI)
 * - WiFi (https://www.arduino.cc/reference/en/libraries/wifi/)
 * - ArduinoJson (https://arduinojson.org/)
 * - Arduino ESP32 OTA (https://github.com/esp8266/Arduino/tree/master/libraries/ArduinoOTA)
 * - Preferences (inclus dans l'ESP32 core)
 * - BLE (inclut dans l'ESP32 core)
 *
 * @section author Auteur
 * - Réécrit par Adam El Horri et Thomas Razafimbahoaka .
 */

#include "globals.h"          // Les includes et variables globales
#include "ble_module.h"       // Fonctions d'initialisation BLE
#include "wifi_module.h"      // Fonctions de connexion WiFi
#include "server_routes.h"    // Handlers et configuration du serveur HTTP
#include "ota_module.h"       // Configuration d'OTA
#include "display_module.h"   // Fonctions d'affichage sur l'écran TFT
#include "sensors_module.h"   // Fonctions de lecture des capteurs
#include "led_module.h"       // Fonctions de mise à jour des LEDs

void setup() {
  // Initialisation écran TFT
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1);
  tft.setCursor(10, 10);

  // Initialisation de la communication série
  Serial.begin(115200);

  // Configuration des pins LED
  pinMode(LED_PIN_RED, OUTPUT);
  pinMode(LED_PIN_GREEN, OUTPUT);

  // Valeurs par défaut des LEDs
  digitalWrite(LED_PIN_RED, LOW);
  digitalWrite(LED_PIN_GREEN, LOW);

  // Initialisation des préférences
  preferences.begin("my-app-iot", false);

  // Lecture des préférences pour récupérer SSID / MDP, etc.
  wifi_ssid     = preferences.getString("ssid", "Adam");
  wifi_password = preferences.getString("password", "adamadam");
  globalUserControl = preferences.getBool("globalUserControl", false);

  Serial.print("Stored SSID: ");
  Serial.println(wifi_ssid);
  Serial.print("Stored PASSWORD: ");
  Serial.println(wifi_password);

  tft.println("Stored SSID: " + wifi_ssid);
  tft.println("Stored PASSWORD: " + wifi_password);

  // Connexion WiFi
  bool wifiConnectionStatus = connectToWifi();  
  Serial.print("WiFi connection status: ");
  Serial.println(wifiConnectionStatus);
  tft.println("WiFi connection status: " + String(wifiConnectionStatus));

  // Si la connexion WiFi est réussie, démarrer le serveur
  if (wifiConnectionStatus) {
    setupServerRoutes();
  }
  else {
    // Sinon, on lance le BLE pour configurer le WiFi
    Serial.println("Initialisation Bluetooth...");
    tft.println("Initialisation Bluetooth...");
    initializeBluetooth();
  }

  // Configuration OTA
  setupOTA();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    // Gestion des mises à jour OTA
    ArduinoOTA.handle();

    // Actualisation de l'affichage
    displaySensorData();

    // Mise à jour de l'état des LEDs (mode manuel ou auto)
    updateLedStateA();
    updateLedStateB();

    // Gestion des requêtes HTTP entrantes
    server.handleClient();
  } else {
    Serial.println("Erreur: Pas de connexion WiFi");
    tft.println("Erreur: Pas de connexion WiFi");

    // Tentative de reconnexion WiFi
    if (connectToWifi()) {
      setupServerRoutes();
    }
  }
}
