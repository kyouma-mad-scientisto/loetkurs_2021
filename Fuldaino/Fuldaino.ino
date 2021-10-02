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
#include <util/delay.h>
#include <avr/pgmspace.h>

const char testText[] = {"FuldAIno Loetkurs 2021  "};

void setup() 
{
  Serial.begin(115200);
  Serial.println("Start FuldAIno");
  initScanning();
  pushText(testText, sizeof(testText)-1);
}

void loop()
{
  //Demomuster
  Serial.println("Start Demo 1");
  for(uint8_t k=0; k<2; k++) {
    for(uint8_t i=0; i < 8; i++) {
      for(uint8_t j=0; j < 8; j++){
        if(getPixel(j,i)) {
          clearPixel(j,i);
        } else {setPixel(j,i);}
        delay(50);
      }
    }
  }
  //Scrolltext
  Serial.println("Start Textscrolling");
  while(!scrollMessage()) { 
  }
}
