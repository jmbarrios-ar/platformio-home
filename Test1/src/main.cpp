#include <Arduino.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(D6, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(D6, HIGH);
  delay(1000);
  Serial.println("ENCENDIDO");
  digitalWrite(D6, LOW);
  delay(1000);
  Serial.println("APAGADO");
}

