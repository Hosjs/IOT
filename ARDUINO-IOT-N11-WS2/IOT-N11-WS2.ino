#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <DHT.h>

const char* ssid = "Tuan Anh";
const char* password = "Tuananh1995";

// DHT Sensor Configuration
#define DHTPIN D4        // GPIO pin where the DHT sensor is connected
#define DHTTYPE DHT11    // DHT11 or DHT22
DHT dht(DHTPIN, DHTTYPE);

WebSocketsServer webSocket(81);

const int ledPin = D1;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  dht.begin();
}

void loop() {
  webSocket.loop();

  // Read sensor data and send it over WebSocket
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (!isnan(h) && !isnan(t)) {
    String data = "Temp: " + String(t) + "°C, Humidity: " + String(h) + "%";
    webSocket.broadcastTXT(data);
  }
  delay(2000); // Update every 2 seconds
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_TEXT) {
    String message = String((char*)payload);
    if (message == "ON") {
      digitalWrite(ledPin, HIGH);
    } else if (message == "OFF") {
      digitalWrite(ledPin, LOW);
    }
  }
}
