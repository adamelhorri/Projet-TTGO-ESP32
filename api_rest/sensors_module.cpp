#include "globals.h"
#include "sensors_module.h"

bool verifyThresholdActivation(bool tempCheck, bool lightCheck) {
  if (tempCheck) {
      double resValue = analogRead(THERMISTOR_PIN);
      float T = A_coef + B_coef * log(resValue) + C_coef * pow(log(resValue), 3);
      float temperatureK = 1.0 / T;
      float temperatureC = temperatureK - 273.15 - 53.0;

      Serial.printf("Température calculée: %.2f°C\n", temperatureC);

      if(tempThreshold >= temperatureC){
        return true;
      }
  }

  if (lightCheck) {
      int lightValue = analogRead(LIGHT_SENSOR_PIN);
      Serial.printf("Lumière mesurée: %d\n", lightValue);

      if(lightThreshold >= lightValue){
        return true;
      }
  }

  return false;
}
