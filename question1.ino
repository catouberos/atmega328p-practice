#include <avr/io.h>

int pressed = 0;
int count = 0;

int main(void) {
  DDRD &= ~(1 << DDD2);  // PORTD2 as input
  DDRB |= (1 << DDB5);   // PORTB5 as output

  // clock 0
  TCCR0A |= (1 << WGM01);  // CTC mode
  TCCR0B |= (1 << CS02);   // prescaler of 256
  OCR0A = 249;             // 500Hz
  TIMSK0 = (1 << OCIE0A);  // enable compare match A interrupt

  // clock 1
  TCCR1B ^= (1 << WGM12);  // CTC mode
  TCCR1B |= (1 << CS10);
  TCCR1B |= (1 << CS11);   // prescaler of 64
  OCR1A = 46874;           // 3/4 Hz /2 (using with software prescaler)
  TIMSK1 = (1 << OCIE1A);  // enable compare match A interrupt

  // int0 trigger on logic change
  EICRA |= (1 << ISC01);  // falling-edge of int0
  EIMSK |= (1 << INT0);   // enable interrupt

  sei();

  while (1) {
  }
}

ISR(INT0_vect) {
  if (pressed == 0) {
    pressed = 1;
  } else {
    pressed = 0;
  }
}

ISR(TIMER0_COMPA_vect) {
  if (pressed == 0) {
    PORTB ^= (1 << PORTB5);
  }
}

ISR(TIMER1_COMPA_vect) {
  if (pressed == 1 && count == 1) {
    PORTB ^= (1 << PORTB5);
    count = 0;
  } else if (count == 0) {
    count++;
  }
}