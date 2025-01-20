# Architettura del sistema

## Panoramica
Il sistema è composto da diverse classi che collaborano per gestire il controllo di volo. Questo documento descrive le relazioni principali tra le classi.

---

## Diagramma delle classi
![Diagramma delle classi](diagram.png) - Da realizzare

---

## Relazioni principali

1. **Receiver**:
   - Legge i dati dal pilota tramite il protocollo iBus.
   - Decodifica i segnali ricevuti e li converte in una struttura dati `ReceiverData`
   - Fornisce i dati a `SystemController` e `FlightController`.

2. **IMU**:
   - Fornisce i dati dell'assetto e delle velocità angolari.
   - I dati sono utilizzati da `FlightController` per il calcolo dei PID.

3. **LED e RGB_LED**:
   - Gestiscono la segnalazione visiva dello stato del sistema.
   - Supportano modalità statiche e lampeggianti.

4. **PIDcontrol**:
   - Fornisce algoritmi PID per il controllo numerico a `FlightController`.
   - Consente il tuning dei parametri PID in tempo reale.

5. **Quaternions**:
   - Contiene funzioni per manipolare quaternioni.
   - Fornisce i metodi a `FlightController` per i calcoli dei setpoint.

6. **FlightController**:
   - Elabora i dati di input da `Receiver` e `IMU`.
   - Applica algoritmi PID per calcolare le uscite necessarie a controllare gli attuatori.

7. **SystemController**:
   - Gestisce lo stato del sistema, comprese le transizioni tra arm/disarm e le modalità operative.
   - Verifica le condizioni di sicurezza e attiva modalità failsafe in caso di necessità.
   - Coordina `FlightController` e  `Aircraft` per garantire un funzionamento sicuro ed efficace.

8. **Aircraft**:
   - Contiene le istanze di `ESC`, `Motors`, `LED`, `IMU`, e `Receiver`.
   - Fornisce un'interfaccia per leggere i sensori e scrivere sugli attuatori.

9. **Logger**:
   - Registra eventi di sistema, errori e stati diagnostici.
   - Invio remoto dei log tramite `WiFiManager` per analisi e debugging.

10. **WiFiManager**:
   - Configura e gestisce la connessione WiFi.
   - Controlla periodicamente la disponibilità di un server remoto.
   - Supporta la trasmissione dei log.

---

## Flusso di dati
1. Input dal pilota: `Receiver` legge i comandi dal pilota e li converte in dati comprensibili dal sistema.
2. Sensori: `IMU` fornisce dati di assetto e velocità angolare.
3. Monitoraggio: `SystemController` verifica lo stato del sistema e coordina le modalità operative.
4. Elaborazione: `FlightController` calcola i PID in base ai dati di `Receiver` e `IMU`.
5. Output agli attuatori: `Aircraft` invia i comandi agli attuatori (ESC, motori, LED).
6. Logging: `Logger` registra i dati e li trasmette tramite `WiFiManager` al server remoto.

---
