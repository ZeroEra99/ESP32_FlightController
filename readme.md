# ESP32 Aircraft Flight Controller

Questo progetto implementa un sistema di controllo di volo per aeromobili basato su ESP32, utilizzando programmazione orientata agli oggetti per garantire modularità, manutenibilità e performance elevate. L'architettura include un Flight Controller e un Server che collaborano per garantire stabilità, monitoraggio remoto e raccolta dati.

---

## Funzionalità principali
- **Flight Controller**:
  - Gestisce il controllo in tempo reale dell'aeromobile.
  - Utilizza sensori IMU (Inertial Measurement Unit) per leggere dati inerziali e calcolare l'assetto.
  - Implementa algoritmi PID per la stabilizzazione giroscopica e il controllo dell'attitudine.
  - Implementa logiche di sicurezza per la gestione degli stati di errore
- **Comunicazione WiFi**:
  - Trasmissione e sincronizzazione dati con un server remoto.
- **Logging**:
  - Registra e trasmette dati telemetrici e messaggi diagnistici in tempo reale.

---

## Struttura delle directory

Il progetto è organizzato come segue:

```plaintext
ESP32_AircraftFlightController/
├── include/         # File header (.h) con definizioni e parametri di configurazione
├── src/             # File sorgente (.cpp) con implementazione delle classi e funzioni
├── docs/            # Documentazione e diagrammi
│   ├── architettura.md # Descrizione dell'architettura
│   ├── diagrams/       # Diagrammi del sistema (.puml)
├── README.md        # Documentazione principale
