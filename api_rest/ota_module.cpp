#include "globals.h"
#include "ota_module.h"

void setupOTA() {
  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH) type = "sketch";
      else type = "filesystem";

      Serial.println("Start updating " + type);
      tft.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
      tft.println("OTA End");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
      tft.fillScreen(TFT_BLACK);
      tft.setCursor(0, 0);
      tft.printf("OTA Progress: %u%%\n", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      tft.printf("OTA Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) {
        Serial.println("Auth Failed");
        tft.println("Auth Failed");
      }
      else if (error == OTA_BEGIN_ERROR) {
        Serial.println("Begin Failed");
        tft.println("Begin Failed");
      }
      else if (error == OTA_CONNECT_ERROR) {
        Serial.println("Connect Failed");
        tft.println("Connect Failed");
      }
      else if (error == OTA_RECEIVE_ERROR) {
        Serial.println("Receive Failed");
        tft.println("Receive Failed");
      }
      else if (error == OTA_END_ERROR) {
        Serial.println("End Failed");
        tft.println("End Failed");
      }
    });
  
  ArduinoOTA.begin();
}
