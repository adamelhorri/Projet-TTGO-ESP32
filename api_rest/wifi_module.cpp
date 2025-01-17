#include "globals.h"
#include "wifi_module.h"

bool connectToWifi() {
  // Lit à nouveau les valeurs stockées (au cas où elles aient changé par BLE)
  wifi_ssid     = preferences.getString("ssid", "Adam");
  wifi_password = preferences.getString("password", "adamadam");

  WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());

  Serial.print("Connexion à ");
  Serial.println(wifi_ssid);
  tft.println("Connexion à " + wifi_ssid);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 6) {  
    delay(5000);
    Serial.println("Connexion en cours...");
    tft.println("Connexion en cours...");
    attempts++;
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Échec de la connexion Wi-Fi");
    tft.println("Échec de la connexion Wi-Fi");
    return false;
  }

  Serial.println("Connexion Wi-Fi réussie");
  tft.println("Connexion Wi-Fi réussie");

  // Configuration IP fixe (optionnel)
  IPAddress staticIP(192, 168, 1, 54);
  IPAddress gateway(192, 168, 1, 1); 
  IPAddress subnet(255, 255, 255, 0); 

  if(!WiFi.config(staticIP, gateway, subnet)) {
    Serial.println("Échec de la configuration de l'IP fixe");
    tft.println("Échec de la configuration de l'IP fixe");
  }

  Serial.print("Adresse IP: ");
  Serial.println(WiFi.localIP());
  tft.print("Adresse IP: ");
  tft.println(WiFi.localIP());

  // Charger le mode global depuis les préférences
  globalUserControl = preferences.getBool("globalUserControl", false);

  return true;
}
