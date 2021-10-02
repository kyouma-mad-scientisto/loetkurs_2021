#define C0 PORTB |= (0x01 << 3)
#define C1 PORTB |= (0x01 << 4)
#define C2 PORTB |= (0x01 << 5)
#define C3 PORTC |= (0x01 << 0)
#define C4 PORTC |= (0x01 << 1)
#define C5 PORTC |= (0x01 << 2)
#define C6 PORTC |= (0x01 << 3)
#define C7 PORTC |= (0x01 << 4)

#define R0 PORTB |= (0x01 << 2)
#define R1 PORTB |= (0x01 << 1)
#define R2 PORTB |= (0x01 << 0)
#define R3 PORTD |= (0x01 << 7)
#define R4 PORTD |= (0x01 << 6)
#define R5 PORTD |= (0x01 << 5)
#define R6 PORTB |= (0x01 << 7)
#define R7 PORTB |= (0x01 << 6)

#define C0c PORTB &= ~(0x01 << 3)
#define C1c PORTB &= ~(0x01 << 4)
#define C2c PORTB &= ~(0x01 << 5)
#define C3c PORTC &= ~(0x01 << 0)
#define C4c PORTC &= ~(0x01 << 1)
#define C5c PORTC &= ~(0x01 << 2)
#define C6c PORTC &= ~(0x01 << 3)
#define C7c PORTC &= ~(0x01 << 4)

#define R0c PORTB &= ~(0x01 << 2)
#define R1c PORTB &= ~(0x01 << 1)
#define R2c PORTB &= ~(0x01 << 0)
#define R3c PORTD &= ~(0x01 << 7)
#define R4c PORTD &= ~(0x01 << 6)
#define R5c PORTD &= ~(0x01 << 5)
#define R6c PORTB &= ~(0x01 << 7)
#define R7c PORTB &= ~(0x01 << 6)

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
  PORTB &= 00111000;
  PORTD &= 00011111;

  if(columns & 1) {C0;} else {C0c;}
  if(columns & 2) {C1;} else {C1c;}
  if(columns & 4) {C2;} else {C2c;}
  if(columns & 8) {C3;} else {C3c;}
  if(columns & 16) {C4;} else {C4c;}
  if(columns & 32) {C5;} else {C5c;}
  if(columns & 64) {C6;} else {C6c;}
  if(columns & 128) {C7;} else {C7c;}
  
  if(lineOutput & 128) {R0;} else {R0c;}
  if(lineOutput & 64) {R1;} else {R1c;}
  if(lineOutput & 32) {R2;} else {R2c;}
  if(lineOutput & 16) {R3;} else {R3c;}
  if(lineOutput & 8) {R4;} else {R4c;}
  if(lineOutput & 4) {R5;} else {R5c;}
  if(lineOutput & 2) {R6;} else {R6c;}
  if(lineOutput & 1) {R7;} else {R7c;}

/*
  //nun die neue Zeile in den Spaltentreiber laden
  PORTB &= 11000111;
  PORTC &= 11100000;
  PORTB |= (00111000 & ((columns & 00000111) << 3));
  PORTC |= (00011111 & ((columns & 11111000) >> 3));
  //zuletzt den Zeilentreiber wieder einschalten
  PORTB |= (11000111 & (((lineOutput & 11100000) >> 5) | ((lineOutput & 00000011) << 6)));
  PORTD |= (11100000 & ((lineOutput & 00011100) << 3)); */
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
  DDRC = 0xFF;
  DDRD = 0xFF;
//alle Ports loeschen
  PORTB = 0;
  PORTC = 0;
  PORTD = 0;
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
