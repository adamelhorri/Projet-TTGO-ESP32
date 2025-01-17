#ifndef CORS_MODULE_H
#define CORS_MODULE_H

/**
 * @brief Ajoute les en-têtes CORS standard à la réponse HTTP en cours.
 */
void addCorsHeaders();

/**
 * @brief Gère les requêtes OPTIONS (CORS), renvoie un statut 200 si c'est une requête OPTIONS.
 */
void handleCors();

#endif
