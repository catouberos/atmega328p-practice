#include <avr/io.h>

int main(void) {
  DDRD &= ~(1 << DDD2);  // PORTD2 as input
  DDRB |= (1 << DDB5);   // PORTB5 as output

  DDRB |= (1 << DDB0);   // DEBUG: PORTB0 as output

  // clock 0
  TCCR0A |= (1 << WGM01); // default: CTC mode
  TCCR0B |= (1 << CS01);
  TCCR0B |= (1 << CS00); // prescaler of 64
  OCR0A = 249; // 500Hz
  TIMSK0 = (1 << OCIE0A); // enable compare match A interrupt
  
  // clock 1
  TCCR1B = 0; // default: normal mode
  TCCR1B |= (1 << CS10);
  TCCR1B |= (1 << CS11); // prescaler of 64
  OCR1A = 24999; // 5Hz
  TIMSK1 = (1 << OCIE1A); // enable compare match A interrupt

  // int0 trigger on logic change
  EICRA |= (1 << ISC11); // falling-edge
  EIMSK |= (1 << INT0);  // enable interrrupt
  
  sei();

  while (1) {
  }
}

ISR (INT0_vect) {
  // on button, toggle clock 0 and 1 between normal mode/CTC mode
  TCCR0A ^= (1 << WGM01);
  TCCR1B ^= (1 << WGM12);
  PORTB ^= (1 << PORTB0);
}

ISR (TIMER0_COMPA_vect) {
  PORTB ^= (1 << PORTB5);
}

ISR (TIMER1_COMPA_vect) {
  PORTB ^= (1 << PORTB5);
}