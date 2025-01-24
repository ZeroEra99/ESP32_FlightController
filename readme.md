# ESP32 Aircraft Flight Controller


# ESP32 Aircraft Flight Controller


Questo progetto implementa un sistema di controllo di volo per aeromobili basato su ESP32, progettato nell'ambito di una tesi sperimentale presso l'Università degli Studi di Cagliari. Il sistema è progettato per garantire modularità e manutentabilità. L'architettura è composta da due componenti principali: un Flight Controller che gestisce l'aeromodello, e un Server remoto per il monitoraggio e la raccolta dati.

---

## Funzionalità principali

### Flight Controller (ESP32)
- **Input**:
  - Acquisizione dei dati dai sensori IMU (Inertial Measurement Unit).
  - Lettura dei segnali dal ricevitore per il controllo remoto.
- **Elaborazione**:
  - Verifica e gestione degli errori di sistema.
  - Aggiornamento degli stati e delle modalità operative (manuale, stabilizzazione giroscopica, controllo dell'attitudine, ecc.).
  - Algoritmi PID per la stabilizzazione e il controllo del volo.
- **Output**:
  - Controllo in tempo reale di attuatori come motori, servomotori ed ESC.
  - Feedback visivo tramite LED per indicare lo stato del sistema.
- **Logging**:
  - Raccolta e trasmissione di dati telemetrici e diagnostici in tempo reale al server.
- **Comunicazione**:
  - Configurazione della rete WiFi e sincronizzazione con il server tramite protocolli mDNS e HTTP.

### Server (Windows)
- **Logging e monitoraggio**:
  - Ricezione e archiviazione di dati diagnostici e telemetrici dal Flight Controller.
- **Interfaccia utente**:
  - Fornisce un accesso tramite browser per visualizzare i dati raccolti in tempo reale.

---

## Requisiti

### Requisiti Hardware

#### Flight Controller (ESP32)
- **ESP32** con supporto WiFi.
- Sensore **IMU** (Adafruit BNO055).
- Ricevitore e trasmettitore RC (Compatibili con protocollo iBus).
- Servomotori ed ESC (Electronic Speed Controllers) per il controllo delle superfici aerodinamiche e del motore.
- LED per il feedback visivo.

#### Server (Windows)
- Sistema desktop o laptop con:
  - **Connessione WiFi stabile.**
  - **Sistema operativo Windows 10 o superiore.**

---

### Requisiti Software

#### Flight Controller (ESP32)
- **Ambiente di sviluppo**: PlatformIO con supporto a framework ESP-IDF e Arduino.
- Librerie necessarie:
  - **ESP32Servo**: per il controllo degli attuatori.
  - **Adafruit_Unified_Sensor**, **Adafruit_BNO055** e **SPI** per la gestione dei sensori inerziali.

#### Server (Windows)
- **Python 3.10 o superiore**.
- Librerie Python richieste (installabili tramite `pip`):
  - **Flask**: per il backend del server.
  - **datetime**: per la gestione dei timestamp.
  - **zeroconf**: per la scoperta del server tramite mDNS.
  - **socket**: per la comunicazione con il Flight Controller.

---

## Documentazione
La documentazione completa è disponibile nella directory `docs/` e include:
- **architettura.md**: Descrizione dell'architettura della piattaforma.
- **Diagrams**: Diagrammi `.puml` e `.png` per rappresentare l'architettura e i flussi.

---

## Struttura delle directory

```plaintext
ESP32_AircraftFlightController/
├── include/         # File header (.h) con definizioni e parametri di configurazione
├── src/             # File sorgente (.cpp) con implementazione delle classi e funzioni
├── server/          # Codice Python del server remoto
├── docs/            # Documentazione e diagrammi
│   ├── architettura.md # Descrizione dettagliata dell'architettura
│   ├── diagrams/       # Diagrammi del sistema (.puml e .png)
├── README.md        # Questo file
```

---

## Autore

Il progetto è stato realizzato da **Federico Giugni**  
Email: [f.giugni@studenti.unica.it](mailto:f.giugni@studenti.unica.it)

### Università
- **Università degli Studi di Cagliari**
- **Facoltà di Scienze**

### Relatore
- **Prof. Livio Pompianu**
- **Dipartimento di Matematica e Informatica**
- **Email:** [pompianu.livio@unica.it](mailto:pompianu.livio@unica.it)

### Anno Accademico
- **2024/2025**

---
