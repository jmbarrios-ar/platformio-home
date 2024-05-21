#include <Arduino.h>

// Corte de energía: Pines para detección
const int relecorte = 4;  // nro pin del relé
const int ledcorteenergia = 5;    // nro de pin para led del relé
int releState = 0;  // variable para lectura de estado del relé

void setup() {
  Serial.begin(115200); 
//  Serial.println(F("DHT1 Conectado!"));
//  dht.begin();  // Inicializa el sensor DHT
  pinMode(relecorte, INPUT); // se inicializa el pin del relé indicador de corte de energía
  pinMode(ledcorteenergia, OUTPUT);  // se inicializa el pin del led indicador de corte de energía
}

void loop() {
  releState = digitalRead(relecorte);  // lectura del estado del valor del relé de Corte de Energía
  Serial.println(releState);
  if (releState == HIGH) {    // revisa si el relé pasa de NC a NA. Si es así, el estado del relé es HIGH
    digitalWrite(ledcorteenergia, HIGH);  // LED de Corte de Energía encendido
    Serial.println("Se cortó la energía electrica");
    // aqui va el envio de msje de Telegram
    // aqui se Publica el Topico para MQTT
  } else {
    digitalWrite(ledcorteenergia, LOW);  // LED de Corte de Energía apagado
    Serial.println("Se restauró a la normalidad la energía electrica");
    // aqui va el envio de msje de Telegram
    // aqui se Publica el Topico para MQTT
  }
  //Función para leer temperatura y humedad
}