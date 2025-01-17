#include "globals.h"
#include "ble_module.h"

// ----------------------------------------------------------------------------
// Callbacks BLE
// ----------------------------------------------------------------------------
class CustomBleServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) override {
      Serial.println("Appareil connecté (BLE)");
      tft.println("Appareil connecté (BLE)");
    }

    void onDisconnect(BLEServer* pServer) override {
      Serial.println("Appareil déconnecté (BLE)");
      tft.println("Appareil déconnecté (BLE)");
    }
};

class CustomBleCharacteristicCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) override {
    String rawValue = pCharacteristic->getValue();
    
    if (rawValue.length() > 0) {
        int delimiterIndex = rawValue.indexOf(',');
        if (delimiterIndex != -1) {
            String firstVal  = rawValue.substring(0, delimiterIndex);
            String secondVal = rawValue.substring(delimiterIndex + 1);

            Serial.println("SSID: " + firstVal);
            Serial.println("Password: " + secondVal);

            tft.println("Received BLE credentials:");
            tft.print("SSID: ");
            tft.println(firstVal);
            tft.print("Password: ");
            tft.println(secondVal);

            wifi_ssid     = firstVal;
            wifi_password = secondVal;

            preferences.putString("ssid", firstVal);
            preferences.putString("password", secondVal);

            extern bool connectToWifi(); // Déclaré dans wifi_module.h
            if (connectToWifi()) {
                preferences.putBool("coble", true);
                pCharacteristic->setValue("Ok");
            } else {
                pCharacteristic->setValue("Échec");
            }
        } else {
            Serial.println("Format invalide. Attendu deux valeurs séparées par une virgule.");
            tft.println("Format BLE invalide reçu.");
        }
    } else {
        Serial.println("Valeur reçue vide.");
        tft.println("Valeur BLE reçue vide.");
    }
  }
};

void initializeBluetooth() {
  Serial.println("Démarrage du BLE...");
  tft.println("Démarrage du BLE...");

  BLEDevice::init("Esp32");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new CustomBleServerCallbacks());

  BLEService *pService = pServer->createService(BLE_SERVICE_UUID);

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
      BLE_CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ |
      BLECharacteristic::PROPERTY_WRITE |
      BLECharacteristic::PROPERTY_NOTIFY
  );
  pCharacteristic->setCallbacks(new CustomBleCharacteristicCallbacks());
  pCharacteristic->setNotifyProperty(true);

  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(BLE_SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  Serial.println("BLE en cours de publicité... prêt à se connecter.");
  tft.println("BLE en cours de publicité... prêt à se connecter.");
}
