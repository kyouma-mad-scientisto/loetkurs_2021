static uint16_t length;
static char* text;
static uint32_t msgPointer;
char nextChar[8];

void pushText(const char* inputText, uint16_t inputLength)
{
  length = inputLength;
  text = inputText;
  msgPointer = 0;
}
//muss regelmaessig ausgefuehrt werden um den Text zu scrollen
bool scrollMessage()
{
  static uint32_t scrollWindow[8];    //5 Zeichen max (6 Pixel breit)
  bool scrollingDone = false;
  static uint32_t lastTime = 0;
  const uint32_t scrollTimeout = 100; //Geschwindigkeit in der gescrollt wird

  if((millis() - lastTime) >= scrollTimeout) {
    lastTime = millis();
  //wenn msgPointer = 0, dann Window loeschen
    if(msgPointer == 0) {
      memset(scrollWindow, 0, 8);
    }
  //scrolling
    for(uint8_t i=0; i<8; i++)
    {
      scrollWindow[i] = scrollWindow[i] >> 1; //fuer lesen von links nach rechts
    }
    if((msgPointer % 6) == 0) {
      printChar(text[msgPointer/6]);
      for(uint8_t i=0; i<8; i++)
      {
        scrollWindow[i] |= (uint32_t)nextChar[i] << 26; //neues Zeichen ans Ende einfuegen
      }
    }
    if(msgPointer < (length*6 - 1)) {
      msgPointer++;
    } else {msgPointer = 0; scrollingDone = true;}
    //neuen Inhalt in Framebuffer uebertragen
    for(uint8_t i=0; i<8; i++) {
      changeFramebufferLine((uint8_t)(scrollWindow[i]>>20),i);  //Viewport startet bei Bit 20
    }
  }
  return scrollingDone;
}
//einen Buchstaben in einen Puffer zeichnen
void printChar(char letter)
{
  char bytesIn[8];

  for(int8_t i=4,j=0; i>-1; i--, j++)
  {
    bytesIn[j+3] = pgm_read_byte(&characters[letter - 32][i]);
  }
  memset(bytesIn, 0, 3);
  memset(nextChar, 0, 8);
  for(int i = 0; i < 8; i++) {
      for(int j = 0; j < 8; j++) {
          nextChar[i] = (nextChar[i] << 1) | ((bytesIn[j] >> i) & 0x01);
      }
  }
}
