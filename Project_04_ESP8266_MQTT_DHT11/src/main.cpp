#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// Conexión WiFi
const char* ssid = "nombre_de_tu_red_wifi";
const char* password = "contraseña_de_tu_red_wifi";

// Información del servidor MQTT
const char* mqtt_server = "dirección_ip_del_broker_mqtt";
const int mqtt_port = 1883;
const char* mqtt_user = "usuario_mqtt";
const char* mqtt_password = "contraseña_mqtt";

// Pin del sensor DHT11
#define DHTPIN D1
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Pin del LED
const int ledPin = D5;

// Umbral de temperatura para encender el LED
const float temperatura_umbral = 25.0;

// Cliente MQTT
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Conectado a la red WiFi");
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje recibido desde el tópico: ");
  Serial.println(topic);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Intentando conexión MQTT...");
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("Conectado al servidor MQTT");
      client.subscribe("temperatura_humedad");
    } else {
      Serial.print("fallo, rc=");
      Serial.print(client.state());
      Serial.println(" Intentando nuevamente en 5 segundos");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  dht.begin();
  pinMode(ledPin, OUTPUT);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Lectura de la temperatura y humedad
  float temperatura = dht.readTemperature();
  float humedad = dht.readHumidity();

  if (isnan(temperatura) || isnan(humedad)) {
    Serial.println("Fallo al leer el sensor DHT");
    return;
  }

  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" °C");
  Serial.print("Humedad: ");
  Serial.print(humedad);
  Serial.println(" %");

  // Publicar valores en el tópico MQTT
  char tempString[8];
  dtostrf(temperatura, 6, 2, tempString);
  client.publish("temperatura_humedad", tempString);

  // Encender el LED si la temperatura supera el umbral
  if (temperatura > temperatura_umbral) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  delay(5000); // Esperar 5 segundos antes de volver a leer el sensor
}

