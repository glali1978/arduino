#include <RH_ASK.h>
#include <SPI.h> // kell, de nem használjuk közvetlenül

RH_ASK rf_driver(2000, 11, 12); // 2000 bps, RX pin: 11 (nem használt), TX pin: 12

int pirPin = 2;          // PIR OUT
int ledPin = 13;         // Beépített LED

void setup() {
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);

  // RF driver indítása
  if (!rf_driver.init()) {
    Serial.println("RF modul nem indult el!");
  }
}

void loop() {
  int motion = digitalRead(pirPin);
  digitalWrite(ledPin, motion);

  Serial.println(motion);

  // Üzenet előkészítése (1 vagy 0 karakterként)
  char msg[2];
  sprintf(msg, "%d", motion);

  rf_driver.send((uint8_t *)msg, strlen(msg));
  rf_driver.waitPacketSent(); // Megvárja, amíg elküldi

  delay(200);
}
