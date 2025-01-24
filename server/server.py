from flask import Flask, jsonify, request, render_template_string
from datetime import datetime
from zeroconf import ServiceInfo, Zeroconf
import socket
import threading
import os
import signal

app = Flask(__name__)

# Variabili per salvare i log e i dati ricevuti
server_logs = []   # Log disponibili su /get_logs
display_logs = []  # Log visibili su /logs

server_data = []     # Dati numerici disponibili su /get_data_logs

# Variabile per il controllo dell'arresto
shutdown_signal = False

# Funzione per registrare il servizio mDNS
def start_mdns_service():
    hostname = socket.gethostname()
    local_ip = socket.gethostbyname(hostname)
    service_info = ServiceInfo(
        "_http._tcp.local.",
        "ESP32Server._http._tcp.local.",
        addresses=[socket.inet_aton(local_ip)],
        port=5000,
        properties={},
        server=f"{hostname}.local."
    )
    zeroconf = Zeroconf()
    zeroconf.register_service(service_info)
    return zeroconf, service_info

# Endpoint per ricevere i log dall'ESP32
@app.route('/receive_logs', methods=['POST'])
def receive_logs():
    global display_logs, server_logs
    log_data = request.data.decode('utf-8')
    timestamped_log = f"[{datetime.now().strftime('%H:%M:%S')}] {log_data}"
    display_logs.append(timestamped_log)
    server_logs.append(timestamped_log)
    return "Logs received", 200

# Endpoint per ottenere i log in formato JSON
@app.route('/get_logs', methods=['GET'])
def get_logs():
    global server_logs
    return jsonify(server_logs), 200

# Endpoint per visualizzare i log
@app.route('/get_display_logs', methods=['GET'])
def get_display_logs():
    global display_logs
    return jsonify(display_logs), 200

# Pagina per visualizzare i log
@app.route('/display_logs')
def display_logs_endpoint():
    template = """
    <html>
<head>
    <title>ESP32 Logs</title>
    <script>
async function updateLogs() {
    try {
        const response = await fetch('/get_display_logs');
        if (response.ok) {
            const logs = await response.json();
            const logContainer = document.getElementById('logContainer');
            logContainer.innerHTML = '';
            logs.forEach(log => {
                const logElement = document.createElement('div');
                logElement.textContent = log;
                logContainer.prepend(logElement);
            });
        }
    } catch (error) {
        console.error('Errore nell aggiornamento dei log:', error);
    }
}
setInterval(updateLogs, 200);
</script>
</head>
<body>
    <h1>Logs ricevuti dall'ESP32</h1>
    <pre id="logContainer">Caricamento log...</pre>
</body>
</html>
    """
    return render_template_string(template)

# Endpoint per cancellare i log dal server
@app.route('/clear_server_logs', methods=['GET'])
def clear_logs():
    global server_logs
    server_logs = []
    return "Logs cancellati dal server", 200

# Endpoint per cancellare i log dalla visualizzazione
@app.route('/clear_display_logs', methods=['GET'])
def clear_display_logs():
    global display_logs
    display_logs = []
    return "Logs cancellati dalla visualizzazione", 200

# Endpoint per ricevere i dati numerici dall'ESP32
@app.route('/receive_data', methods=['POST'])
def receive_data_logs():
    global data_logs
    try:
        data = request.get_json()
        if isinstance(data, list):
            server_data.append(data)
            return "Data logs received", 200
        else:
            return "Invalid data format", 400
    except Exception as e:
        return f"Error processing data: {str(e)}", 500

# Endpoint per ottenere i dati numerici in formato JSON
@app.route('/get_data', methods=['GET'])
def get_data_logs():
    global server_data
    return jsonify(server_data), 200

# Endpoint per cancellare i dati numerici dal server
@app.route('/clear_server_data', methods=['GET'])
def clear_server_data():
    global server_data
    server_data = []
    return "Dati cancellati dal server.", 200

# Endpoint per verificare se il server è attivo
@app.route('/ping', methods=['GET'])
def ping():
    return jsonify({"status": "active"}), 200

@app.route('/favicon.ico')
def favicon():
    return '', 204

# Endpoint per arrestare il server
@app.route('/shutdown', methods=['POST'])
def shutdown():
    global shutdown_signal
    shutdown_signal = True
    return "Server arrestato", 200

# Funzione per avviare il server Flask
def run_server():
    app.run(host='0.0.0.0', port=5000)

if __name__ == '__main__':
    zeroconf, service_info = start_mdns_service()
    server_thread = threading.Thread(target=run_server)
    server_thread.start()

    try:
        while not shutdown_signal:
            pass
    finally:
        zeroconf.unregister_service(service_info)
        zeroconf.close()
        print("Server arrestato.")
        os.kill(os.getpid(), signal.SIGTERM)
