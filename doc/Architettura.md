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
   - Fornisce i dati a `SystemController` e `FlightController`.

2. **IMU**:
   - Fornisce i dati dell'assetto e delle velocità angolari.
   - I dati sono utilizzati da `FlightController` per il calcolo dei PID.

3. **FlightController**:
   - Elabora i dati di input da `Receiver` e `IMU`.
   - Genera gli output per il controllo degli attuatori tramite PID.

4. **SystemController**:
   - Monitora lo stato generale del sistema (es. arm/disarm, modalità operative).
   - Aggiorna le modalità e i comandi per `FlightController`.

5. **Aircraft**:
   - Contiene le istanze di `ESC`, `Motors`, `LED`, `IMU`, e `Receiver`.
   - Funge da interfaccia per leggere i sensori e scrivere sugli attuatori.

6. **WiFiManager**:
   - Gestisce l'accesso remoto ai log tramite un Access Point.
   - Non interagisce direttamente con il loop principale, ma fornisce supporto diagnostico.

---

## Flusso di dati
1. `Receiver` legge i comandi dal pilota.
2. `IMU` legge i dati di assetto.
3. `FlightController` calcola i PID in base ai dati di `Receiver` e `IMU`.
4. `SystemController` verifica lo stato del sistema (es. modalità failsafe).
5. `Aircraft` invia i comandi agli attuatori (ESC, motori, LED).

---
