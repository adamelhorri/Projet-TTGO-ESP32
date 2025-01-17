#ifndef SENSORS_MODULE_H
#define SENSORS_MODULE_H

/**
 * @brief Vérifie si le seuil est dépassé pour la température ou la lumière (mode automatique).
 * @param tempCheck   true si l'on veut vérifier le seuil de température
 * @param lightCheck  true si l'on veut vérifier le seuil de lumière
 * @return true si le seuil est dépassé, false sinon
 */
bool verifyThresholdActivation(bool tempCheck, bool lightCheck);

#endif
