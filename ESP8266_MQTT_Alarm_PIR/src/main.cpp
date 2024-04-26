#include <ESP8266WiFi.h>
#include <PubSubClient.h>
// Definir los parámetros de la red WiFi
const char* ssid = "247IASbros2.4";
const char* password = "T3reKByo2023$";

// Definir los parámetros del broker MQTT
const char* mqtt_server = "192.168.24.150";
const int mqtt_port = 1883; // Puerto predeterminado para MQTT
const char* mqtt_user = "usermqtt";    // en Datacenter mqtt_user = "adminmqtt"
const char* mqtt_password = "Ia$247";

WiFiClient espClient;
PubSubClient client(espClient);

// Definir el pin del sensor de movimiento PIR
const int pirPin = D7; // Por ejemplo, utilizar el pin GPIO 13

// Definir el tópico MQTT para publicar el estado de la alarma
const char* alarm_topic = "casa/alarm_01";

int pirState = LOW;           // de inicio no hay movimiento
int pirValue = 0;                  // estado del pin

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

void loop() 
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Leer el estado del sensor de movimiento PIR
  int pirValue = digitalRead(pirPin);

  if (pirValue == HIGH) {
    // Publicar el estado de disparo de la alarma cuando se detecta movimiento
    
    digitalWrite(D6, HIGH);
    if (pirState == LOW)  //si previamente estaba apagado
    {
      client.publish(alarm_topic, "Alarma activada - Movimiento detectado");
      Serial.println("Sensor activado");
      pirState = HIGH;
    }
    delay(1000); // Esperar un segundo antes de volver a publicar para evitar múltiples publicaciones consecutivas
  
  else   //si esta desactivado
  {
    digitalWrite(D6, LOW); // LED OFF
    if (pirState == HIGH)  //si previamente estaba encendido
    {
      client.publish(alarm_topic, "Alarma desactivada - Sin Movimiento alguno");
      Serial.println("Sensor parado");
      pirState = LOW;
    }
  }
  // Puedes agregar más lógica aquí según tus necesidades, como apagar la alarma después de cierto tiempo, etc.
  }
}