#include "globals.h"
#include "display_module.h"
#include "sensors_module.h" // Pour lire la température/lumière si besoin direct

void drawSensorDataCircle(float temp, int light, uint16_t color) {
  tft.fillScreen(TFT_BLACK);

  tft.setRotation(0);
  int16_t x = 64, y = 65;
  int16_t width = 100, height = 100;
  int16_t radius = 10; // Rayon pour les coins arrondis
  
  // Dessiner un rectangle avec des coins arrondis pour la température
  tft.fillRoundRect(x - width / 2, y - height / 2, width, height, radius, TFT_ORANGE);

  int16_t y2 = y + height + 10;

  // Dessiner un rectangle avec des coins arrondis pour la lumière
  tft.fillRoundRect(x - width / 2, y2 - height / 2, width, height, radius, TFT_ORANGE);
  
  tft.setRotation(1);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  tft.setTextWrap(true);

  // Afficher les données de température
  tft.setCursor(25, 60);
  tft.printf("%.2f C", temp); 

  // Afficher les données de lumière
  tft.setCursor(150, 60);
  tft.printf("%d", light);
}

void displaySensorData() {
  if (millis() - previousLoopTimeDisplay >= displayInterval) {
    // Lecture Thermistor
    double resValue = analogRead(THERMISTOR_PIN);
    float T = A_coef + B_coef * log(resValue) + C_coef * pow(log(resValue), 3);
    float temperatureK = 1.0 / T;
    float temperatureC = temperatureK - 273.15 - 53.0;

    // Lecture Photoresistor
    int lightValue = analogRead(LIGHT_SENSOR_PIN);
    uint16_t color = TFT_BLUE;

    drawSensorDataCircle(temperatureC, lightValue, color);
    previousLoopTimeDisplay = millis();
  }
}
