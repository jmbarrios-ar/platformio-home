#include <Arduino.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(D6, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(D6, HIGH);
  delay(1000);
  Serial.println("ENCENDIDO LED AMARILLO");
  digitalWrite(D6, LOW);
  delay(1000);
  Serial.println("APAGADO LED AMARILLO");
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  Serial.println("ENCENDIDO EL BUILTIN");
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  Serial.println("APAGADO EL BUILTIN");
}

