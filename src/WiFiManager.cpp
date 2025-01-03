/**
 * @file WiFiManager.cpp
 * @brief Implementazione della classe WiFiManager.
 *
 * Questa classe gestisce l'Access Point WiFi per il sistema, consentendo la creazione
 * di una rete wireless dedicata e l'accesso tramite un indirizzo IP locale.
 */

#include "WiFiManager.h"

/**
 * @brief Costruttore della classe WiFiManager.
 *
 * Inizializza l'oggetto WiFiManager con i dettagli della rete AP.
 *
 * @param ssid SSID della rete AP.
 * @param password Password della rete AP.
 */
WiFiManager::WiFiManager(const char* ssid, const char* password) : ssid(ssid), password(password) {}

/**
 * @brief Avvia l'Access Point.
 *
 * Configura e avvia l'Access Point WiFi utilizzando l'SSID e la password forniti.
 * Stampa l'indirizzo IP assegnato per consentire l'accesso alla rete.
 */
void WiFiManager::startAccessPoint() {
    WiFi.softAP(ssid, password);
    Serial.println("Access Point avviato!");
    Serial.print("Indirizzo IP: ");
    Serial.println(WiFi.softAPIP());
}

/**
 * @brief Ottiene l'indirizzo IP dell'Access Point.
 *
 * Restituisce l'indirizzo IP assegnato all'Access Point sotto forma di stringa.
 *
 * @return Stringa con l'indirizzo IP dell'AP.
 */
String WiFiManager::getLocalIP() {
    return WiFi.softAPIP().toString();
}
