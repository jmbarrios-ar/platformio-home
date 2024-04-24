#include <ESP8266WiFi.h>
#include <PubSubClient.h>
// Definir los parámetros de la red WiFi
const char* ssid = "datacenter";
const char* password = "NOv22$1nicI0";

// Definir los parámetros del broker MQTT
const char* mqtt_server = "170.10.10.68";
const int mqtt_port = 1883; // Puerto predeterminado para MQTT
const char* mqtt_user = "adminmqtt"; 
const char* mqtt_password = "Ia$247";

WiFiClient espClient;
PubSubClient client(espClient);

// Definir el pin del sensor de movimiento PIR
const int pirPin = D7; // Por ejemplo, utilizar el pin GPIO 13

// Definir el tópico MQTT para publicar el estado de la alarma
const char* alarm_topic = "Datacenter/Alarm_01";

void setup_wifi() {
  delay(10);
  // Conectar a la red WiFi
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Conexión WiFi establecida");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  // No necesitamos un callback en este ejemplo
}

void reconnect() {
  // Loop hasta que estemos reconectados
  while (!client.connected()) {
    Serial.print("Intentando conexión MQTT...");

    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("Conectado al broker MQTT");
    } else {
      Serial.print("falló, rc=");
      Serial.print(client.state());
      Serial.println(" Intentando nuevamente en 5 segundos");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(pirPin, INPUT);
  pinMode(D6, OUTPUT);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Leer el estado del sensor de movimiento PIR
  int pirValue = digitalRead(pirPin);

  if (pirValue == HIGH) {
    // Publicar el estado de disparo de la alarma cuando se detecta movimiento
    client.publish(alarm_topic, "Alarma activada - Movimiento detectado");
    digitalWrite(D6, HIGH);
    delay(1000); // Esperar un segundo antes de volver a publicar para evitar múltiples publicaciones consecutivas
  }

  // Puedes agregar más lógica aquí según tus necesidades, como apagar la alarma después de cierto tiempo, etc.
}