#include "globals.h"

// --- Stockage persistant ---
Preferences preferences;

// --- Identifiants WiFi ---
String wifi_ssid     = "Bernard2";      
String wifi_password = "naberdia";      

// --- Pins Capteurs & LEDs ---
const int LIGHT_SENSOR_PIN   = 36; 
const int THERMISTOR_PIN     = 39; 
const int LED_PIN_RED        = 17; 
const int LED_PIN_GREEN      = 33; 

// --- Paramètres LEDs (mode manuel) ---
int ledIntervalA     = 1000;
int ledIntervalB     = 1000;
int displayInterval  = 2000;
const int blinkDuration = 500;

// --- Seuils (température / lumière) ---
double tempThreshold  = 18.5;
int lightThreshold    = 900;  

// --- Paramètres LEDs (mode automatique) ---
int ledIntervalA_Auto = 1000;
int ledIntervalB_Auto = 1000;
byte ledStateA_Auto   = LOW;
byte ledStateB_Auto   = LOW;
String ledOnOffA_Auto = "on";
String ledOnOffB_Auto = "on";

// --- Activation mode automatique (par LED) ---
bool autoModeTemp  = true;
bool autoModeLight = true;

// --- État courant des LEDs ---
byte ledStateA   = LOW;
byte ledStateB   = LOW;
String ledOnOffA = "on";
String ledOnOffB = "on";

// --- Variables de timing ---
unsigned long previousLoopTimeA       = 0;
unsigned long previousLoopTimeB       = 0;
unsigned long previousLoopTimeDisplay = 0;

// --- Coefficients SteinHart-Hart ---
const float A_coef = 1.217243477e-3;
const float B_coef = 2.165973985e-4;
const float C_coef = 1.815361374e-7;

// --- TFT & Serveur HTTP ---
TFT_eSPI tft;
WebServer server(80);

// --- JSON ---
DynamicJsonDocument json_send_obj(200);
char json_send[200];

// --- Variable Globale pour le Mode de Contrôle des LEDs
bool globalUserControl = false;
