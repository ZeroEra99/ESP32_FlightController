import subprocess
import os
import requests

# Variabile per tenere traccia del processo del server Flask
server_process = None

# Funzione per avviare il server in un nuovo terminale
def start_server():
    global server_process
    if server_process is None:
        if os.name == 'nt':  # Windows
            command = ['start', 'cmd', '/k', 'python', 'server.py']
        else:  # MacOS/Linux
            command = ['gnome-terminal', '--', 'python3', 'server.py']
        server_process = subprocess.Popen(command, shell=(os.name == 'nt'))
        print("Server avviato in un nuovo terminale.")
    else:
        print("Il server è già in esecuzione.")

# Funzione per arrestare il server tramite l'endpoint /shutdown
def stop_server():
    global server_process
    try:
        response = requests.post('http://127.0.0.1:5000/shutdown')
        if response.status_code == 200:
            print("Server fermato.")
        else:
            print(f"Errore nell'arresto del server: {response.status_code}")
    except requests.exceptions.RequestException as e:
        print(f"Errore di connessione al server: {e}")
    finally:
        if server_process:
            server_process.terminate()
            server_process = None

# Funzione per cancellare i log dal server
def clear_server_logs():
    try:
        response = requests.get('http://127.0.0.1:5000/clear_server_logs')
        if response.status_code == 200:
            print("Logs cancellati dal server.")
        else:
            print(f"Errore nella cancellazione dei log: {response.status_code}")
    except requests.exceptions.RequestException as e:
        print(f"Errore di connessione al server: {e}")

# Funzione per cancellare i log dalla visualizzazione
def clear_display_logs():
    try:
        response = requests.get('http://127.0.0.1:5000/clear_display_logs')
        if response.status_code == 200:
            print("Logs cancellati dalla visualizzazione.")
        else:
            print(f"Errore nella cancellazione dei log: {response.status_code}")
    except requests.exceptions.RequestException as e:
        print(f"Errore di connessione al server: {e}")
        
# Funzione per cancellare i dati dal server
def clear_server_data():
    try:
        response = requests.get('http://127.0.0.1:5000/clear_server_data')
        if response.status_code == 200:
            print("Dati cancellati dal server.")
        else:
            print(f"Errore nella cancellazione dei dati: {response.status_code}")
    except requests.exceptions.RequestException as e:
        print(f"Errore di connessione al server: {e}")

# Funzione per gestire i comandi
def handle_commands():
    while True:
        command = input("Comando: ").strip().lower()
        if command == "start":
            start_server()
        elif command == "stop":
            stop_server()
        elif command == "clear_server_logs":
            clear_server_logs()
        elif command == "clear_display_logs":
            clear_display_logs()
        elif command == "clear_server_data":
            clear_server_data()
        elif command == "status":
            if server_process and server_process.poll() is None:
                print("Il server è attivo.")
            else:
                print("Il server non è attivo.")
        elif command == "quit":
            stop_server()
            print("Chiusura del launcher...")
            break
        else:
            print(f"Comando sconosciuto: {command}")

if __name__ == "__main__":
    print("Comandi disponibili: start, stop, clear_server_logs, clear_display_logs, clear_server_data, status, quit")
    handle_commands()
