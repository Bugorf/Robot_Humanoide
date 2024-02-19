/*
 * See documentation at https://nRF24.github.io/RF24
 * See License information at root directory of this library
 * Author: Brendan Doherty (2bndy5)
 */

/**
 * A simple example of sending data from 1 nRF24L01 transceiver to another.
 *
 * This example was written to be used on 2 devices acting as "nodes".
 * Use the Serial Monitor to change each node's behavior.
 */
#include <SPI.h>
#include "printf.h"
#include "RF24.h"


RF24 radio(7, 8); // CE, CSN


const byte address[6] = "00001";

void setup() {

  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    char text[4] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
    
    if (strlen(text) == 4) {
      Serial.println("no");
    } else if (strlen(text) == 2) {
      Serial.println("yes");
    }
    
  }
 
}
