# ESP32 Aircraft Flight Controller

## Descrizione
**ESP32 Aircraft Flight Controller** è un software progettato per controllare un aereo radiocomandato utilizzando un microcontrollore ESP32. Il sistema integra diversi componenti hardware per fornire stabilizzazione automatica, modalità di assistenza e controllo manuale.

## Funzionalità principali
- **Stabilizzazione del volo** tramite controlli PID.
- **Modalità assistita**:
  - Stabilizzazione tramite giroscopio.
  - Controllo dell'assetto.
- **Modalità failsafe** per la gestione di errori critici.
- **Access Point Wi-Fi** per il monitoraggio remoto e il logging.
- **Gestione dei LED** per visualizzare lo stato del sistema.

---

## Struttura delle directory
Il progetto è organizzato come segue:

```plaintext
ESP32_AircraftFlightController/
├── include/         # Contiene tutti i file header (.h)
├── src/             # Contiene tutti i file sorgente (.cpp)
├── doc/             # Documentazione e diagrammi
│   └── Architecture.md # Descrizione dell'architettura del sistema
├── README.md        # Documentazione principale
