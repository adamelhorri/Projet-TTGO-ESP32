# Projet d’IoT avec ESP32 TTGO : Contrôle de LEDs (Mode Manuel/Auto), Mesures de Température et de Luminosité

**Auteurs :**  
- Adam EL HORRI  
- Tomas Razafimbahoaka  

---

## Table des Matières
1. [Aperçu du Projet](#aperçu-du-projet)  
2. [Caractéristiques et Fonctionnalités](#caractéristiques-et-fonctionnalités)  
3. [Architecture Logicielle](#architecture-logicielle)  
4. [Matériel et Schéma de Connexion](#matériel-et-schéma-de-connexion)  
5. [Prérequis et Bibliothèques](#prérequis-et-bibliothèques)  
6. [Installation et Déploiement](#installation-et-déploiement)  
7. [API REST & Endpoints](#api-rest--endpoints)  
8. [Fonctionnalités BLE](#fonctionnalités-ble)  
9. [Mises à Jour Over-The-Air (OTA)](#mises-à-jour-over-the-air-ota)  
10. [Exploitation des Données (Display & Logs)](#exploitation-des-données-display--logs)  
11. [Structure du Projet](#structure-du-projet)  
12. [Développement Futur et Améliorations](#développement-futur-et-améliorations)  
13. [Crédits et Références](#crédits-et-références)  

---

## 1. Aperçu du Projet
Ce projet présente une solution IoT complète déployée sur un **ESP32 TTGO**, permettant :
- Le **contrôle de LEDs** en **mode manuel** ou **mode automatique** (basé sur des capteurs).
- La **mesure** de la **température** (thermistor) et de la **luminosité** (photoresistor).
- La configuration du **WiFi** via **BLE** en cas d'absence de connexion.
- L’accès à une **API REST** pour piloter et superviser l’application à distance.
- Le **support OTA** pour mettre à jour le firmware sans fil.

### Visualisation du branchement :
![image](https://github.com/user-attachments/assets/c66edc82-ba1e-45b9-bd61-019e90bb24aa)

---

## 2. Caractéristiques et Fonctionnalités
1. **Contrôle Global des LEDs** : 
   - Mode manuel (on/off) ou mode auto (seuil de température/luminosité).
2. **Persistance** : 
   - Sauvegarde du mode global, SSID/Mot de passe WiFi, seuils, etc., via **Preferences** (EEPROM).
3. **API REST** : 
   - Routes HTTP (GET/POST) pour récupérer les données capteurs et commander les LEDs.
4. **BLE** : 
   - Configuration simplifiée des identifiants WiFi depuis un smartphone ou un autre appareil BLE.
5. **OTA (Over-the-Air)** : 
   - Mises à jour firmware simplifiées, suivi de la progression via la **série** et l’**écran**.
6. **Affichage TFT** : 
   - Visualisation en temps réel de la température, luminosité, état du WiFi, logs de debug, etc.

---

## 3. Architecture Logicielle
Le code est découpé de façon **modulaire** :
- **`api_rest_corrected_final.ino`** : Point d’entrée principal (fonctions `setup()` et `loop()`).
- **Modules** (en paires `.h/.cpp`) :
  - **BLE** pour la configuration,
  - **WiFi** pour la connexion réseau,
  - **CORS** pour les headers HTTP,
  - **OTA** pour les mises à jour,
  - **Display** pour l’écran TFT,
  - **Sensors** pour la lecture Thermistor/Photoresistor,
  - **LED** pour le contrôle des LEDs (manuel/auto),
  - **Server Routes** pour l’API REST (routes GET/POST).

Chaque module est **indépendant** et communique via des **variables globales** centralisées.

---

## 4. Matériel et Schéma de Connexion
- **Carte TTGO (ESP32)** :  
  - Pin 17 : **LED Rouge**  
  - Pin 33 : **LED Verte**  
  - Pin 39 : **Thermistor**  
  - Pin 36 : **Photoresistor**  

Le schéma varie selon les modèles TTGO, assurez-vous de consulter la **documentation** du fabricant pour valider les correspondances de pins si nécessaire.

---

## 5. Prérequis et Bibliothèques
1. [**ESP32 Core**](https://github.com/espressif/arduino-esp32) pour Arduino.  
2. **TFT_eSPI** : Affichage sur écran TFT ([Repo GitHub](https://github.com/Bodmer/TFT_eSPI)).  
3. **WiFi** : Bibliothèque Arduino intégrée (pour ESP32).  
4. [**ArduinoJson**](https://arduinojson.org/) : Gérer l’encodage/décodage JSON.  
5. [**ArduinoOTA**](https://github.com/esp8266/Arduino/tree/master/libraries/ArduinoOTA) : Mises à jour Over-the-Air.  
6. **Preferences** : Bibliothèque incluse dans le core ESP32.  
7. **BLE** : Bibliothèque incluse dans le core ESP32 (`<BLEDevice.h>` etc.).  

---

## 6. Installation et Déploiement
1. **Cloner** ou **télécharger** ce repository.  
2. Placer tous les fichiers dans un même dossier (nommé par exemple `projet_iot`).  
3. Ouvrir **`api_rest_corrected_final.ino`** dans l’IDE Arduino ou PlatformIO.  
4. **Installer** les bibliothèques manquantes via le **Library Manager**.  
5. Sélectionner la **carte** : *TTGO T-Display*, *TTGO T1* ou *ESP32 Dev Module* (selon votre modèle).  
6. **Compiler** puis **téléverser** sur la carte.  

---

## 7. API REST & Endpoints
Tous les endpoints sont exposés sur le **port 80** par défaut.

- **GET `/temperature`** : Renvoie la température mesurée en Celsius.  
- **GET `/light`** : Renvoie la valeur brute du capteur de luminosité.  
- **GET `/ledA`**, **GET `/ledB`**, **GET `/led`** : État actuel et vitesse de clignotement des LEDs.  
- **GET `/ledAutoParams`** : Récupère la configuration auto (seuils, mode On/Off, etc.).  
- **GET `/ledGlobalMode`** : Indique si le mode de contrôle est `auto` ou `manual`.

- **POST `/ledChangeA`** : Définit l’état (on/off) et la vitesse de LED A.  
- **POST `/ledChangeB`** : Définit l’état (on/off) et la vitesse de LED B.  
- **POST `/ledChange`** : Définit l’état et la vitesse des deux LEDs en même temps.  
- **POST `/autoManTemp`**, **POST `/autoManLight`** : Active/Désactive le mode automatique pour Temp/Light.  
- **POST `/autoManTempParams`**, **POST `/autoManLightParams`** : Définit l’état on/off et la vitesse en mode auto.  
- **POST `/thresholdTemp`**, **POST `/thresholdLight`** : Définit les seuils de température et de luminosité.  
- **POST `/ledGlobalMode`** : Bascule globalement entre `auto` et `manual`.  

Toutes les routes acceptent des **paramètres** en **`x-www-form-urlencoded`** ou JSON.  

---

## 8. Fonctionnalités BLE
Si le WiFi n’est pas configuré ou hors de portée, l’ESP32 démarre en **mode BLE** :
- **Service UUID** : `4fafc201-1fb5-459e-8fcc-c5c9c331914b`  
- **Characteristic UUID** : `beb5483e-36e1-4688-b7f5-ea07361b26a8`  
- Il suffit d’écrire dans cette Characteristic une chaîne de type :  
  ```
  SSID,Password
  ```
- Le code se charge ensuite de **sauvegarder** ces identifiants dans la **Preferences** et tente une **connexion WiFi**.

---

## 9. Mises à Jour Over-The-Air (OTA)
- Le **OTA** est automatiquement activé si l’appareil est connecté en WiFi.  
- Les différentes étapes (progression, erreurs, etc.) sont affichées sur le **moniteur série** et sur l’**écran TFT**.
- Pour réaliser la mise à jour, il suffit d’utiliser un outil comme **espota.py** ou l’**IDE Arduino** (port réseau).

---

## 10. Exploitation des Données (Display & Logs)
- Un **écran TFT** (géré par **TFT_eSPI**) affiche régulièrement la température et la luminosité sous forme de **rectangles arrondis**.
- Les **logs** de débogage (température mesurée, état WiFi, etc.) s’affichent également sur le **Moniteur Série**.

---

## 11. Structure du Projet

```bash
projet_iot/
├── api_rest_corrected_final.ino   # Point d'entrée principal (setup/loop)
├── globals.h                      # Toutes les variables globales, #define, librairies, etc.
├── globals.cpp                    # Initialisation des variables globales
├── ble_module.h
├── ble_module.cpp
├── wifi_module.h
├── wifi_module.cpp
├── cors_module.h
├── cors_module.cpp
├── ota_module.h
├── ota_module.cpp
├── display_module.h
├── display_module.cpp
├── sensors_module.h
├── sensors_module.cpp
├── led_module.h
├── led_module.cpp
├── server_routes.h
├── server_routes.cpp
```

- **`globals.*`** : Déclarations/définitions de toutes les variables et objets globaux (WiFi, tft, etc.).  
- **`ble_module.*`** : Initialisation BLE et callbacks (lecture SSID/Mot de passe).  
- **`wifi_module.*`** : Connexion WiFi, configuration statique d’IP, etc.  
- **`cors_module.*`** : Gestion CORS (headers) pour l’API.  
- **`ota_module.*`** : Configuration et callbacks OTA (progression, erreurs).  
- **`display_module.*`** : Fonctions pour dessiner à l’écran (température, luminosité).  
- **`sensors_module.*`** : Logique de lecture et calcul (Thermistor + Photoresistor).  
- **`led_module.*`** : Mise à jour des LEDs selon le mode global/auto.  
- **`server_routes.*`** : Gestion de toutes les routes REST (GET/POST).  

---

## 12. Développement Futur et Améliorations
- **Visualisation** : Generation d'une application Flutter permettant d'utiliser l'API au maximum de son potentiel
- **Optimisation** des calculs SteinHart-Hart (bibliothèque dédiée?).  
- **Sécurité** : Chiffrement BLE, authentification pour l’API REST.  
- **Dashboards** : Intégration d’un client web local ou d’une plateforme Cloud (ex: ThingsBoard, AWS IoT).  
- **Statistiques** : Sauvegarde et analyse des données capteurs (base de données, etc.).  

---

## 13. Crédits et Références
- **Adam EL HORRI** et **Tomas Razafimbahoaka** pour le développement et la rédaction du code.  
- [**ArduinoJson**](https://arduinojson.org/) pour la gestion du JSON.  
- [**TFT_eSPI**](https://github.com/Bodmer/TFT_eSPI) pour l’affichage écran.  
- [**ESP32 Arduino Core**](https://github.com/espressif/arduino-esp32) qui inclut WiFi, BLE, Preferences.  
- [**ArduinoOTA**](https://github.com/esp8266/Arduino/tree/master/libraries/ArduinoOTA) pour les mises à jour Over-the-Air.  

---



**Bon développement IoT !**
