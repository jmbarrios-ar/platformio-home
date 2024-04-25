#include <Arduino.h>

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
const char* alarm_topic = "Datacenter/Alarm_02"; // Definir el tópico MQTT para publicar el estado de la alarma

WiFiClient espClient;
PubSubClient client(espClient);

// the number of the pushbutton pin
const int buttonPin = D7; 
// the number of the LED pin
const int ledPin = D6;
// variables will change:
int buttonState = 0;  // variable for reading the pushbutton status

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
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    client.publish(alarm_topic, "Pulsador apretado - Hay movimiento"); // Publicar el estado de disparo de la alarma cuando se detecta movimiento
    digitalWrite(ledPin, HIGH); // turn LED on:
    delay(2000); // Esperar segundos antes de volver a publicar para evitar múltiples publicaciones consecutivas
  } else {
    client.publish(alarm_topic, "Pulsador no apretado - No hay movimiento");
    digitalWrite(ledPin, LOW); // turn LED off:
  }

  
  // Puedes agregar más lógica aquí según tus necesidades, como apagar la alarma después de cierto tiempo, etc.
}