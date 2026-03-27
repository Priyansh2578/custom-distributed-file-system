from flask import Flask, request, jsonify
import socket

app = Flask(__name__)

def send_to_cdfs(command):
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.settimeout(2)
        s.connect(('127.0.0.1', 8080))
        s.send(command.encode())
        response = s.recv(1024).decode()
        s.close()
        return response
    except Exception as e:
        return f"Error: {str(e)}"

@app.route('/ping', methods=['GET'])
def ping():
    return jsonify({"response": send_to_cdfs("ping")})

@app.route('/set', methods=['POST'])
def set_key():
    data = request.json
    key = data.get('key')
    value = data.get('value')
    if not key or not value:
        return jsonify({"error": "Key and value required"}), 400
    return jsonify({"response": send_to_cdfs(f"SET {key} {value}")})

@app.route('/get/<key>', methods=['GET'])
def get_key(key):
    return jsonify({"response": send_to_cdfs(f"GET {key}")})

@app.route('/delete/<key>', methods=['DELETE'])
def delete_key(key):
    return jsonify({"response": send_to_cdfs(f"DELETE {key}")})

@app.route('/')
def index():
    return '''
    <!DOCTYPE html>
    <html>
    <head>
        <title>CDFS API Interface</title>
        <style>
            body { font-family: Arial; padding: 20px; background: #f0f0f0; }
            .container { max-width: 600px; margin: auto; background: white; padding: 20px; border-radius: 10px; }
            input, button { padding: 10px; margin: 5px; width: 100%; }
            pre { background: #333; color: #0f0; padding: 10px; border-radius: 5px; }
        </style>
    </head>
    <body>
        <div class="container">
            <h1>CDFS API Interface</h1>
            
            <h3>Ping</h3>
            <button onclick="ping()">Send Ping</button>
            
            <h3>SET Key-Value</h3>
            <input id="setKey" placeholder="Key">
            <input id="setValue" placeholder="Value">
            <button onclick="setKV()">SET</button>
            
            <h3>GET Value</h3>
            <input id="getKey" placeholder="Key">
            <button onclick="getKV()">GET</button>
            
            <h3>DELETE Key</h3>
            <input id="delKey" placeholder="Key">
            <button onclick="delKV()">DELETE</button>
            
            <h3>Response:</h3>
            <pre id="response">Waiting...</pre>
        </div>
        
        <script>
            async function ping() {
                let res = await fetch('/ping');
                let data = await res.json();
                document.getElementById('response').innerText = JSON.stringify(data, null, 2);
            }
            
            async function setKV() {
                let key = document.getElementById('setKey').value;
                let val = document.getElementById('setValue').value;
                let res = await fetch('/set', {
                    method: 'POST',
                    headers: {'Content-Type': 'application/json'},
                    body: JSON.stringify({key, value: val})
                });
                let data = await res.json();
                document.getElementById('response').innerText = JSON.stringify(data, null, 2);
            }
            
            async function getKV() {
                let key = document.getElementById('getKey').value;
                let res = await fetch('/get/' + key);
                let data = await res.json();
                document.getElementById('response').innerText = JSON.stringify(data, null, 2);
            }
            
            async function delKV() {
                let key = document.getElementById('delKey').value;
                let res = await fetch('/delete/' + key, {method: 'DELETE'});
                let data = await res.json();
                document.getElementById('response').innerText = JSON.stringify(data, null, 2);
            }
        </script>
    </body>
    </html>
    '''

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)