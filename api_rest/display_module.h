#ifndef DISPLAY_MODULE_H
#define DISPLAY_MODULE_H

/**
 * @brief Met à jour périodiquement l'affichage des valeurs de température et de lumière sur l'écran.
 */
void displaySensorData();

/**
 * @brief Dessine deux "rectangles arrondis" pour afficher la température et la lumière.
 * @param temp  La valeur de température
 * @param light La valeur de luminosité
 * @param color Couleur du texte ou fond (non utilisé en l'état, mais gardé pour cohérence)
 */
void drawSensorDataCircle(float temp, int light, uint16_t color);

#endif
