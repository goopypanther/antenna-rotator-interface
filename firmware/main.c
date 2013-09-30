// Antenna rotator interface
// By Jeremy Ruhland
//
// Accepts the following serial input:
// p, +, -, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9

#include <avr/io.h>
#include <util/delay.h>

#define ON() PORTB |= (1<<PB4)
#define OFF() PORTB &= ~(1<<PB4)

uint8_t getByte(void);
void transmitPacket(uint8_t, uint8_t);
void transmitNECbyte(uint8_t);
void logicalOne(void);
void logicalZero(void);

#define ADDRESS 0xAC

int main(void) {
 uint8_t data;

 // Idle PB4, set PB4 as output
 ON();
 DDRB |= (1<<PB4);

 // Loop forever
 for(;;) {
  // Get byte via software serial
  data = getByte();

  // Control statement for parsing input char and transmitting packet
  switch (data) {
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
   transmitPacket(ADDRESS, (data-0x30));
   break;
  case 'p':
  case 'P':
   transmitPacket(ADDRESS, 0x1c);
   break;
  case '+':
   transmitPacket(ADDRESS, 0x10);
   break;
  case '-':
   transmitPacket(ADDRESS, 0x11);
   break;
  default:
   break;
  }
 }
}

// Receive byte via software serial @ 9600bps
// ------------------------------------------
uint8_t getByte(void) {
 uint8_t byte = 0;
 uint8_t i;

 // Wait for start bit, wait 1.5 bits
 while(PINB & (1<<PB0)) {}
 _delay_us(15.61);
 // Shift in serial bit, wait 1 bit, loop over byte
 for(i = 0; i < 8; i++) {
 byte <<= 1;
 byte |= (PINB & (1<<PB0));
 _delay_us(10.41);
 }
 return byte;
}

// Transmit NEC packet containing cmd to device addr
// -------------------------------------------------
void transmitPacket(uint8_t addr, uint8_t cmd) {
 // Preamble
 OFF();
 _delay_ms(9);
 ON();
 _delay_ms(4.5);

 transmitNECbyte(addr);
 transmitNECbyte(cmd);
}

// Transmit NEC byte followed by inverse byte
// ------------------------------------------
void transmitNECbyte(uint8_t msg) {
 uint8_t i;

 // Address byte loop
 for(i = 0x01; i != 0; i<<0) {
  if (msg & i)
   logicalOne();
  else
   logicalZero();
 }

 // Inverse byte transmit loop
 for(i = 0x01; i != 0; i<<0) {
  if (!(msg & i))
   logicalOne();
  else
   logicalZero();
 }
}

// Send logical one NEC signal
// ---------------------------
void logicalOne(void) {
 OFF();
 _delay_us(560);
 ON();
 _delay_ms(2.25);
}

// Send logical zero NEC signal
// ----------------------------
void logicalZero(void) {
 OFF();
 _delay_us(560);
 ON();
 _delay_us(560);
}
