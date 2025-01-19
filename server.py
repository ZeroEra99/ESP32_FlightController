from flask import Flask, jsonify, request, render_template_string
from datetime import datetime

app = Flask(__name__)

# Variabile per salvare i log ricevuti
logs = []

# Endpoint per ottenere i log in formato JSON
@app.route('/get_logs', methods=['GET'])
def get_logs():
    global logs
    return jsonify(logs), 200

# Endpoint per ricevere i log dall'ESP32
@app.route('/receive', methods=['POST'])
def receive_logs():
    global logs
    # Ottieni i dati inviati dall'ESP32
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
            console.log('Log ricevuti dal server:', logs); // Log per debug
            
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

// Esegui l'aggiornamento ogni 20 ms
setInterval(updateLogs, 20);

</script>

</head>
<body>
    <h1>Logs ricevuti dall'ESP32</h1>
    <pre id="logContainer">Caricamento log...</pre>
</body>
</html>

    """
    return render_template_string(template)



if __name__ == '__main__':
    # Avvia il server sul desktop
    app.run(host='0.0.0.0', port=5000)
