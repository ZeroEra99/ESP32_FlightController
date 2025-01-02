#include "WiFiManager.h"

WiFiManager::WiFiManager(const char* ssid, const char* password)
    : ssid(ssid), password(password), connected(false), lastReconnectAttempt(0) {}

void WiFiManager::connect() {
    WiFi.begin(ssid, password);
    Serial.println("Connessione al Wi-Fi...");
}

void WiFiManager::disconnect() {
    if (WiFi.status() == WL_CONNECTED) {
        WiFi.disconnect();
        connected = false;
        Serial.println("Wi-Fi disconnessa.");
    }
}

void WiFiManager::attemptConnection() {
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(ssid, password);
        Serial.println("Tentativo di connessione al Wi-Fi...");
    }
}

void WiFiManager::handleConnection() {
    if (WiFi.status() == WL_CONNECTED) {
        if (!connected) {
            connected = true;
            Serial.println("Connesso al Wi-Fi.");
            Serial.print("Indirizzo IP: ");
            Serial.println(WiFi.localIP());
        }
    } else {
        if (millis() - lastReconnectAttempt > 5000) { // Riprova ogni 5 secondi
            lastReconnectAttempt = millis();
            attemptConnection();
        }
    }
}

bool WiFiManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

String WiFiManager::getLocalIP() {
    return WiFi.localIP().toString();
}
