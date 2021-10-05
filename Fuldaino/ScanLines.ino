/*
 * Displayaktualisierung
 * alle 8ms = 125Hz
 */

#define TIMER_INTERVAL_MS  1  //* 8 = 8ms
const uint16_t TIMER1_COUNT = TIMER_INTERVAL_MS / ((float)1 / 8000 * 256) / 8;

const uint8_t FRAMEBUFFER_SIZE = 8;
volatile static uint8_t framebuffer[FRAMEBUFFER_SIZE];

//Timer1 ISR
ISR(TIMER1_COMPA_vect)
{
  scanLines();
  //hier koennten noch weitere KURZE Befehle ausgefuehrt werden
}
//wechselt regelmaessig die Zeile
void scanLines() {
  static uint8_t line = 0;

  if(line < 7) {
    line++;
  } else {
    line = 0;
  }

  uint8_t lineOutput = (128 >> line);
  uint8_t columns = framebuffer[line];

  setOutputs(lineOutput, columns);
}
void setOutputs(uint8_t lineOutput, uint8_t columns) {
//PinBelegung FuldAIno
//zuerst Zeilentreiber deaktivieren um Flackern/Artefakte zu vermeiden
  PORTB &= 0b00111000;
  PORTD &= 0b00011111;
  //nun die neue Zeile in den Spaltentreiber laden
  PORTB &= 0b11000111;
  PORTC &= 0b11100000;
  PORTB |= (0b00111000 & ((columns & 0b00000111) << 3));
  PORTC |= (0b00011111 & ((columns & 0b11111000) >> 3));
  //zuletzt den Zeilentreiber wieder einschalten
  PORTB |= (0b11000111 & (((lineOutput & 0b11100000) >> 5) | ((lineOutput & 0b00000011) << 6)));
  PORTD |= (0b11100000 & ((lineOutput & 0b00011100) << 3)); 
}
//den kompletten Framebuffer ueberschreiben
void changeFramebuffer(char* newBuffer)
{
  for(uint8_t i=0; i<8; i++)
  {
    framebuffer[i] = newBuffer[i];
  }
}
//eine Zeile des Framebuffers aendern
void changeFramebufferLine(char line, uint8_t lineNumber)
{
  framebuffer[lineNumber] = line;  
}
//setzt ein Pixel
void setPixel(uint8_t x, uint8_t y)
{
  if((x < 8) || (y < 8)) {
    framebuffer[y] |= (0x01 << (x));
  }
}
//loescht ein Pixel
void clearPixel(uint8_t x, uint8_t y)
{
  if((x < 8) || (y < 8)) {
    framebuffer[y] &= ~(0x01 << (x));
  }
}
//Zustand eines Pixels beziehen
bool getPixel(uint8_t x, uint8_t y) {
  bool pixelState = false;

  if((x < 8) || (y < 8)) {
    pixelState = (framebuffer[y] >> (x)) & 0x01;
  }
  return pixelState;
}
//setup
void initScanning() {
  DDRB = 0xFF;
  DDRC = 0b00011111;
  DDRD = 0b11100000;
//alle Ports loeschen
  PORTB = 0;
  PORTC = 0b11100000;
  PORTD = 0b00011111;
//framebuffer loeschen
  for(uint8_t i = 0; i < FRAMEBUFFER_SIZE; i++){
    framebuffer[i] = 0;
  }
//Timer1 konfigurieren
  noInterrupts();
  TCCR1A = 0; //set TCCRXA register to 0
  TCCR1B = 0; //set TCCRXB register to 0
  TCNT1 = 0; //reset counter value
    
  OCR1A = TIMER1_COUNT; //set compare match register of timer1
    
  TCCR1B |= (1 << WGM12); //enable timer1 CTC mode
  //1:1024
  TCCR1B |= (1 << CS12); //set prescaler bits
  //Timer einschalten
  TIMSK1 |= (1 << OCIE1A); //enable timer1 compare interrupt  
  interrupts();
}

/*
Pinmapping:

Panel  - Controller

C0  - PB3
C1  - PB4
C2  - PB5
C3  - PC0
C4  - PC1
C5  - PC2
C6  - PC3
C7  - PC4

R0  - PB2
R1  - PB1
R2  - PB0
R3  - PD7
R4  - PD6
R5  - PD5
R6  - PB7
R7  - PB6
*/
