# Architettura della piattaforma
La piattaforma si compone di due componenti principali: un Flight Controller basato su ESP32 e un Server che opera in ambiente desktop Windows. Entrambi i componenti interagiscono tramite rete WiFi, utilizzando protocolli come mDNS per la scoperta dei dispositivi e HTTP per la trasmissione dei dati. Questa architettura garantisce un sistema integrato e scalabile per il controllo di un aeromobile, offrendo funzionalità di monitoraggio remoto e raccolta di dati.

## Componenti principali
- **Flight Controller:** gestisce le operazioni di volo, tra cui acquisizione dei dati dai sensori, elaborazione in tempo reale e controllo degli attuatori.
- **Server:** fornisce funzionalità di logging remoto, monitoraggio e interfaccia utente.
- **Comunicazione ESP32-Server:** utilizza WiFi per sincronizzazione e scambio dati.

![Diagramma a blocchi del sistema](diagrams/system_block_diagram.png)
*Figura 1: Diagramma a blocchi del sistema.*

![Diagramma delle interazioni ESP32-Server](diagrams/esp32_interactions.png)
*Figura 2: Diagramma delle interazioni ESP32-Server.*

---

## Architettura del server
Il server è progettato per ricevere e gestire i dati provenienti dal Flight Controller, offrendo un'interfaccia per l'analisi e la gestione remota.

### Componenti principali del server
- **Moduli di logging:** raccolgono e archiviano dati diagnostici inviati dalla ESP32.
- **Comunicazione:** implementa protocolli come HTTP e mDNS per la scoperta e lo scambio di dati.
- **Interfaccia utente:** consente la visualizzazione dei dati in tempo reale tramite browser.

![Diagramma delle interazioni del server](diagrams/server_lifecycle_interactions.png)
*Figura 3: Diagramma delle interazioni del server.*

---

## Architettura del software di volo
Il software di volo, in esecuzione su ESP32, è responsabile della stabilità e del controllo dell'aeromobile. L'architettura è orientata agli oggetti per garantirne la modularità.

### Blocchi funzionali del software
#### Input:
- Acquisizione e validazione dei dati dai sensori IMU e dal ricevitore.
- Mappatura dei dati grezzi in formati utilizzabili per il controllo.

#### Elaborazione:
- Verifica e gestione degli errori.
- Aggiornamento dello stato e delle modalità operative.
- Calcolo PID per stabilizzazione giroscopica e controllo attitudine.

#### Output:
- Scrittura dei valori sugli attuatori per il controllo dei motori.
- Aggiornamento dei LED per feedback visivo.

#### Logging:
- Raccolta e trasmissione di dati diagnostici al server.
- Formattazione dei messaggi di log.

#### Comunicazione:
- Configurazione della rete WiFi.
- Scoperta del server tramite mDNS.
- Sincronizzazione dei dati in tempo reale.

---

### Classi principali
- **Componenti fisiche:** Actuator, Receiver, IMU, LED.
- **Componenti logiche:** Aircraft, FlightController, PIDController, SystemController.

---

### Diagrammi principali
![Diagramma a blocchi del software del Flight Controller (ESP32)](diagrams/esp32_block_diagram.png)
*Figura 4: Diagramma a blocchi del software del Flight Controller (ESP32)

![Diagramma UML delle classi del Flight Controller (ESP32)](diagrams/esp32_class_diagram.png)
*Figura 5: Diagramma UML delle classi del Flight Controller (ESP32).*

![Diagramma di flusso della configurazione iniziale del Flight Controller](diagrams/esp32_setup_flowchart.png)
*Figura 6: Diagramma di flusso della configurazione iniziale del Flight Controller.*

![Diagramma di flusso del loop principale del Flight Controller](diagrams/esp32_loop_flowchart.png)
*Figura 7: Diagramma di flusso del loop principale del Flight Controller.*

![Diagramma di flusso dei task asincroni del Flight Controller](diagrams/esp32_asynctasks_flowchart.png)
*Figura 8: Diagramma di flusso dei task asincroni del Flight Controller.*
