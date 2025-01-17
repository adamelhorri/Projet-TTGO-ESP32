#ifndef GLOBALS_H
#define GLOBALS_H

// --- Librairies ---
#include <WebServer.h>
#include <WiFi.h>
#include <TFT_eSPI.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <ArduinoOTA.h>
#include <math.h>

// --- Définition service BLE ---
#define BLE_SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define BLE_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

// --- Extern Preferences ---
extern Preferences preferences; 

// --- Identifiants WiFi (par défaut) ---
extern String wifi_ssid;     
extern String wifi_password; 

// --- Pins Capteurs & LEDs ---
extern const int LIGHT_SENSOR_PIN;
extern const int THERMISTOR_PIN;
extern const int LED_PIN_RED;
extern const int LED_PIN_GREEN;

// --- Paramètres LEDs (mode manuel) ---
extern int ledIntervalA;     
extern int ledIntervalB;     
extern int displayInterval;
extern const int blinkDuration;

// --- Seuils (température / lumière) ---
extern double tempThreshold;
extern int lightThreshold;

// --- Paramètres LEDs (mode automatique) ---
extern int ledIntervalA_Auto;
extern int ledIntervalB_Auto;
extern byte ledStateA_Auto;
extern byte ledStateB_Auto;
extern String ledOnOffA_Auto;
extern String ledOnOffB_Auto;

// --- Activation mode automatique (par LED) ---
extern bool autoModeTemp;  
extern bool autoModeLight; 

// --- État courant des LEDs ---
extern byte ledStateA;
extern byte ledStateB;
extern String ledOnOffA;
extern String ledOnOffB;

// --- Variables de timing ---
extern unsigned long previousLoopTimeA;
extern unsigned long previousLoopTimeB;
extern unsigned long previousLoopTimeDisplay;

// --- Coefficients SteinHart-Hart ---
extern const float A_coef;
extern const float B_coef;
extern const float C_coef;

// --- TFT & Serveur HTTP ---
extern TFT_eSPI tft;
extern WebServer server;

// --- JSON ---
extern DynamicJsonDocument json_send_obj;
extern char json_send[200];

// --- Variable Globale pour le Mode de Contrôle des LEDs
extern bool globalUserControl; 
// false => "auto" (capteurs décident)
// true  => "manual" (utilisateur décide via REST)

#endif
