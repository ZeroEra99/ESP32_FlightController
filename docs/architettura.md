# Architettura della piattaforma
La piattaforma si compone di due componenti principali: un Flight Controller basato su ESP32 e un Server che opera in ambiente desktop Windows. Entrambi i componenti interagiscono tramite rete WiFi, utilizzando protocolli come mDNS per la scoperta dei dispositivi e HTTP per la trasmissione dei dati. Questa architettura garantisce un sistema integrato e scalabile per il controllo di un aeromobile, offrendo funzionalità di monitoraggio remoto e raccolta di dati.

## Componenti principali
- **Flight Controller:** gestisce le operazioni di volo, tra cui acquisizione dei dati dai sensori, elaborazione in tempo reale e controllo degli attuatori.
- **Server:** fornisce funzionalità di logging remoto, monitoraggio e interfaccia utente.
- **Comunicazione ESP32-Server:** utilizza WiFi per sincronizzazione e scambio dati.

*In figura: Diagramma a blocchi del sistema.*
![Diagramma a blocchi del sistema](Diagrams/system_block_diagram.png)

---

## Architettura del server
Il server è progettato per ricevere e gestire i dati provenienti dal Flight Controller, offrendo un'interfaccia per l'analisi e la gestione remota.

### Componenti principali del server
- **Moduli di logging:** raccolgono e archiviano dati diagnostici inviati dalla ESP32.
- **Comunicazione:** implementa protocolli come HTTP e mDNS per la scoperta e lo scambio di dati.
- **Interfaccia utente:** consente la visualizzazione dei dati in tempo reale tramite browser.

![Diagramma del lifecycle del server](Diagrams/server_lifecycle_interactions.png)
*In figura: Diagramma del lifecycle del server.*

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
![Diagramma delle operazioni del blocco 'Elaborazione'](Diagrams/esp32_flightcontroller_lifecycle.png)
*Diagramma delle operazioni del blocco 'Elaborazione'.*

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

### Altri diagrammi
![Diagramma a blocchi](Diagrams/esp32_block_diagram.png)
*Diagramma a blocchi.*

![Diagramma UML](Diagrams/esp32_class_diagram.png)
*Diagramma UML.*

![Diagramma di flusso del Setup](Diagrams/esp32_setup_flowchart.png)
*Diagramma di flusso del Setup.*

![Diagramma di flusso dei task asincroni](Diagrams/esp32_asynctasks_flowchart.png)
*Diagramma di flusso dei task asincroni.*

