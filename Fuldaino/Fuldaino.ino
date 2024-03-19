/*
 * Code fuer Badget zum Loetkurs an der HS-Fulda am Fachbereich AI
 * 
 * 8x8 LED Matrix
 * Atmega328p mit internem 8MHz Takt
 * 
 * benoetigt die "Breadboard Arduino" Library
 * siehe dazu:
 * https://github.com/oshlab/Breadboard-Arduino
 * 
 *
 * von Yves E.
 */
#include<avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

const char testText[] = {"FuldAIno Girl's Day 2023  "};

const uint16_t AnimationTimeout = 1000;

void setup() 
{
  Serial.begin(115200);
  Serial.println("Start FuldAIno");
  initScanning();
  pushText(testText, sizeof(testText)-1);
}

void loop()
{
  static bool invertieren = false;
 
  invertDisplay(invertieren);
  Serial.println("Display invertieren = " + String(invertieren));
   
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
  //Herz
  Serial.println("Herz");
  setPixel(1,0);
  setPixel(2,0);
  setPixel(5,0);
  setPixel(6,0);
  setPixel(0,1);
  setPixel(3,1);
  setPixel(4,1);
  setPixel(7,1);
  setPixel(0,2);
  setPixel(7,2);
  setPixel(0,3);
  setPixel(7,3);
  setPixel(1,4);
  setPixel(6,4);
  setPixel(2,5);
  setPixel(5,5);
  setPixel(3,6);
  setPixel(4,6);

  delay(3000);
  clearDisplay();
  for(uint8_t i=0; i<2; i++)
  {
    //Smiley
    //traurig
    Serial.println("trauriges Smiley");
    setPixel(1,1);
    setPixel(6,1);
    setPixel(2,1);
    setPixel(5,1);
    setPixel(1,2);
    setPixel(6,2);
    setPixel(2,2);
    setPixel(5,2);
    setPixel(2,5);
    setPixel(3,5);
    setPixel(4,5);
    setPixel(5,5);
    setPixel(1,6);
    setPixel(6,6);
    setPixel(0,7);
    setPixel(7,7);
    delay(AnimationTimeout);
    clearDisplay();
    //neutral
    Serial.println("neutrales Smiley");
    setPixel(1,1);
    setPixel(6,1);
    setPixel(2,1);
    setPixel(5,1);
    setPixel(1,2);
    setPixel(6,2);
    setPixel(2,2);
    setPixel(5,2);
    setPixel(1,5);
    setPixel(2,5);
    setPixel(3,5);
    setPixel(4,5);
    setPixel(5,5);
    setPixel(6,5);
    delay(AnimationTimeout);
    clearDisplay();
    //froh
    Serial.println("frohes Smiley");
    setPixel(1,1);
    setPixel(6,1);
    setPixel(2,1);
    setPixel(5,1);
    setPixel(1,2);
    setPixel(6,2);
    setPixel(2,2);
    setPixel(5,2);
    setPixel(0,5);
    setPixel(7,5);
    setPixel(1,6);
    setPixel(6,6);
    setPixel(2,7);
    setPixel(3,7);
    setPixel(4,7);
    setPixel(5,7);
    delay(AnimationTimeout);
    clearDisplay();
  }
  clearDisplay();
  //Scrolltext
  Serial.print("Start Textscrolling: ");
  Serial.println(testText);
  while(!scrollMessage()) { 
  }
  invertieren = !invertieren;
}
