from flask import Flask, jsonify, request, render_template_string
from datetime import datetime
from zeroconf import Zeroconf, ServiceInfo
import socket

app = Flask(__name__)

# Variabile per salvare i log ricevuti
logs = []

# Endpoint per ottenere i log in formato JSON
@app.route('/get_logs', methods=['GET'])
def get_logs():
    return jsonify(logs), 200

# Endpoint per ricevere i log dall'ESP32
@app.route('/receive', methods=['POST'])
def receive_logs():
    log_data = request.data.decode('utf-8')
    logs.append(f"[{datetime.now().strftime('%H:%M:%S')}] {log_data}")
    return "Logs received", 200

# Endpoint per verificare se il server è attivo
@app.route('/ping', methods=['GET'])
def ping():
    return jsonify({"status": "active"}), 200

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
            logContainer.innerHTML = ''; // Svuota il contenitore prima di aggiornare
            logs.forEach(log => {
                const logElement = document.createElement('div'); // Crea un elemento per ogni log
                logElement.textContent = log;
                logContainer.appendChild(logElement); // Aggiungi al contenitore
            });
        }
    } catch (error) {
        console.error('Errore nell aggiornamento dei log:', error);
    }
}

// Esegui l'aggiornamento ogni 1 secondo
setInterval(updateLogs, 1000);

</script>
</head>
<body>
    <h1>Logs ricevuti dall'ESP32</h1>
    <pre id="logContainer">Caricamento log...</pre>
</body>
</html>
    """
    return render_template_string(template)

# Configurazione mDNS
def start_mdns_service():
    zeroconf = Zeroconf()
    hostname = socket.gethostname()
    ip_address = socket.gethostbyname(hostname)
    service_info = ServiceInfo(
        "_http._tcp.local.",
        "ESP32Server._http._tcp.local.",
        addresses=[socket.inet_aton(ip_address)],
        port=5000,
        properties={},
    )
    zeroconf.register_service(service_info)
    print(f"Server registrato con mDNS: {ip_address}:5000")
    return zeroconf

if __name__ == '__main__':
    # Avvia mDNS e Flask
    zeroconf = start_mdns_service()
    try:
        app.run(host='0.0.0.0', port=5000)
    finally:
        zeroconf.close()
