from flask import Flask, jsonify, request, render_template_string
from datetime import datetime
from zeroconf import ServiceInfo, Zeroconf
import socket
import threading
import os
import signal

app = Flask(__name__)

# Variabile per salvare i log ricevuti
logs = []

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

# Endpoint per ottenere i log in formato JSON
@app.route('/get_logs', methods=['GET'])
def get_logs():
    global logs
    return jsonify(logs), 200

# Endpoint per ricevere i log dall'ESP32
@app.route('/receive', methods=['POST'])
def receive_logs():
    global logs
    log_data = request.data.decode('utf-8')
    logs.append(f"[{datetime.now().strftime('%H:%M:%S')}] {log_data}")
    return "Logs received", 200

# Endpoint per verificare se il server è attivo
@app.route('/ping', methods=['GET'])
def ping():
    return jsonify({"status": "active"}), 200

@app.route('/favicon.ico')
def favicon():
    return '', 204

# Endpoint per cancellare i log
@app.route('/clear_logs', methods=['GET'])
def clear_logs_endpoint():
    global logs
    logs = []
    return "Logs cancellati", 200


# Pagina per visualizzare i log
@app.route('/logs')
def display_logs():
    template = """
    <html>
<head>
    <title>ESP32 Logs</title>
    <script>
async function updateLogs() {
    try {
        const response = await fetch('/get_logs');
        if (response.ok) {
            const logs = await response.json();
            const logContainer = document.getElementById('logContainer');
            logContainer.innerHTML = '';
            logs.forEach(log => {
                const logElement = document.createElement('div');
                logElement.textContent = log;
                logContainer.appendChild(logElement);
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
