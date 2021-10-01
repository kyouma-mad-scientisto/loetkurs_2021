/*
 * Code fuer Badget zum Loetkurs an der HS-Fulda am Fachbereich AI
 * 
 * 8x8 LED Matrix
 * Atmega328p mit internem 8MHz Takt
 * 
 * benoetigt "Arduino on a Breadboard" Library
 * 
 * 
 * 
 */
#include<avr/interrupt.h>
#include <avr/io.h>

void setup() 
{
  Serial.begin(115200);
  Serial.println("Start");
  initScanning();
}

void loop()
{
  //Demo 1
  for(uint8_t i=0; i < 8; i++) {
    for(uint8_t j=0; j < 8; j++){
      if(getPixel(j,i)) {
        clearPixel(j,i);
      } else {setPixel(j,i);}
      delay(50);
    }
  }
}
