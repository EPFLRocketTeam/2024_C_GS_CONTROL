import socket
import json
import sys
from threading import Thread, Lock
from flask import Flask, jsonify

app = Flask(__name__)


class SimpleTCPClient:
    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.sock = None
        self.running = False
        self.data_store = {}  # Store last received values: {field_id: value}
        self.data_lock = Lock()  # Thread-safe access to data_store

    def connect(self):
        """Establish connection to the server."""
        try:
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.sock.connect((self.host, self.port))
            print(f"Connected to {self.host}:{self.port}")
            self.running = True
            return True
        except Exception as e:
            print(f"Failed to connect: {e}")
            return False

    def send_message(self, message):
        """Send a JSON message to the server."""
        if not self.sock:
            print("No socket connection.")
            return
        try:
            json_msg = json.dumps(message) if isinstance(
                message, dict) else message
            self.sock.sendall(json_msg.encode('utf-8'))
            print(f"Sent: {json_msg}")
        except Exception as e:
            print(f"Failed to send message: {e}")

    def receive_data(self):
        """Receive and print data from the server in a loop."""
        buffer = ""
        while self.running and self.sock:
            try:
                data = self.sock.recv(1024).decode('utf-8')
                if not data:
                    break
                buffer += data
                while '}' in buffer and '{' in buffer:
                    start = buffer.find('{')
                    end = buffer.rfind('}') + 1
                    if start < end:
                        json_str = buffer[start:end]
                        try:
                            parsed = json.loads(json_str)
                            print(f"Received: {json.dumps(parsed, indent=2)}")
                            if 'payload' in parsed:
                                payload = parsed['payload']
                                self.handle_payload(payload)
                        except json.JSONDecodeError as e:
                            print(f"JSON decode error: {e}")
                        buffer = buffer[end:]
                    else:
                        break
            except Exception as e:
                print(f"Receive error: {e}")
                break
        print("Connection closed.")

    def handle_payload(self, payload):
        """Store the received field values."""
        if isinstance(payload, dict):
            with self.data_lock:
                for key, value in payload.items():
                    # Clean the value: remove units like [°], [m], etc.
                    cleaned_value = self.clean_value(value)
                    self.data_store[key] = cleaned_value
                    print(f"  Stored Field {key}: {
                        cleaned_value} (original: {value})")

    def clean_value(self, value):
        """Remove unit notation from values."""
        if isinstance(value, str):
            # Remove anything in brackets and trim whitespace
            import re
            cleaned = re.sub(r'\s*\[.*?\]\s*', '', value).strip()
            # Try to convert to float if possible
            try:
                return float(cleaned)
            except ValueError:
                return cleaned
        return value

    def get_field_value(self, field_id):
        """Get the last received value for a field."""
        with self.data_lock:
            return self.data_store.get(str(field_id))

    def get_all_fields(self):
        """Get all stored field values."""
        with self.data_lock:
            return self.data_store.copy()

    def start_receiving(self):
        """Start a thread to receive data."""
        if self.sock:
            receive_thread = Thread(target=self.receive_data, daemon=True)
            receive_thread.start()

    def disconnect(self):
        """Disconnect and stop."""
        self.running = False
        if self.sock:
            self.sock.close()
            print("Disconnected.")

    def subscribe_to_field(self, field_id):
        """Subscribe to a specific field."""
        subscribe_msg = {
            "header": "subscribe",
            "Authorization": "ClientSecretKey1",
            "payload": {
                "field": field_id
            },
        }
        self.send_message(subscribe_msg)


# Global client instance
client = None


@app.route('/api/field/<field_id>', methods=['GET'])
def get_field(field_id):
    """Get the last value for a specific field."""
    if client is None:
        return jsonify({"error": "Client not initialized"}), 500

    value = client.get_field_value(field_id)
    if value is None:
        return jsonify({"error": f"No data received for field {field_id}"}), 404

    return jsonify({
        "field_id": field_id,
        "value": value
    })


@app.route('/api/location', methods=['GET'])
def get_location():
    """Get the last value for a specific field."""
    if client is None:
        return jsonify({"error": "Client not initialized"}), 500

    lon = client.get_field_value(55)
    lat = client.get_field_value(56)

    return jsonify({
        "lon": lon,
        "lat": lat
    })


@app.route('/api/speed', methods=['GET'])
def get_speed():
    """Get the last value for a specific field."""
    if client is None:
        return jsonify({"error": "Client not initialized"}), 500

    speed = client.get_field_value(61)

    return jsonify({
        "speed": speed,
    })


@app.route('/api/fields', methods=['GET'])
def get_all_fields():
    """Get all stored field values."""
    if client is None:
        return jsonify({"error": "Client not initialized"}), 500

    all_data = client.get_all_fields()
    return jsonify({
        "fields": all_data,
        "count": len(all_data)
    })


@app.route('/api/subscribe/<int:field_id>', methods=['POST'])
def subscribe(field_id):
    """Subscribe to a new field."""
    if client is None:
        return jsonify({"error": "Client not initialized"}), 500

    client.subscribe_to_field(field_id)
    return jsonify({
        "message": f"Subscribed to field {field_id}"
    })


@app.route('/api/health', methods=['GET'])
def health():
    """Health check endpoint."""
    return jsonify({
        "status": "running",
        "connected": client.running if client else False
    })


def run_flask(port=5000):
    """Run Flask server."""
    print(f"Starting Flask server on port {port}")
    app.run(host='0.0.0.0', port=port, debug=False, use_reloader=False)


if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python client.py <host> <port> [flask_port]")
        sys.exit(1)

    host = sys.argv[1]
    port = int(sys.argv[2])
    flask_port = int(sys.argv[3]) if len(sys.argv) > 3 else 5000

    client = SimpleTCPClient(host, port)

    if not client.connect():
        sys.exit(1)

    client.start_receiving()
    client.subscribe_to_field(55)
    client.subscribe_to_field(56)
    client.subscribe_to_field(61)

    # Start Flask server in a separate thread
    flask_thread = Thread(target=run_flask, args=(flask_port,), daemon=True)
    flask_thread.start()

    print(f"\nFlask API available at http://localhost:{flask_port}")
    print("API Endpoints:")
    print(f"  - GET  /api/field/<field_id>  - Get specific field value")
    print(f"  - GET  /api/fields            - Get all field values")
    print(f"  - POST /api/subscribe/<field_id> - Subscribe to a field")
    print(f"  - GET  /api/health            - Health check")
    print("\nEnter commands:")
    print("  - subscribe <field_id> : Subscribe to a field")
    print("  - quit                 : Exit\n")

    try:
        while client.running:
            user_input = input("> ").strip()
            if user_input.lower() == 'quit':
                break
            elif user_input.lower().startswith('subscribe '):
                try:
                    field_id = int(user_input.split()[1])
                    client.subscribe_to_field(field_id)
                except (IndexError, ValueError):
                    print("Usage: subscribe <field_id>")
            elif user_input:
                try:
                    msg = json.loads(user_input)
                    client.send_message(msg)
                except json.JSONDecodeError:
                    print("Invalid JSON. Use 'subscribe <field_id>' or valid JSON.")
    except KeyboardInterrupt:
        pass
    finally:
        client.disconnect()
