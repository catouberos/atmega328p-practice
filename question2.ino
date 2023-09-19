#include <avr/io.h>

int scaler = 1;
int counter = 1;
int direction = 1;

int main(void) {
  DDRD &= ~(1 << DDD2);  // PORTD2 as input
  DDRD &= ~(1 << DDD3);  // PORTD3 as input

  DDRB |= (1 << DDB0);  // PORTB0-2 as output
  DDRB |= (1 << DDB1);  // PORTB0-2 as output
  DDRB |= (1 << DDB2);  // PORTB0-2 as output

  // clock 1
  TCCR1B |= (1 << WGM12);  // CTC mode
  TCCR1B |= (1 << CS10);
  TCCR1B |= (1 << CS11);   // prescaler of 64
  OCR1A = 31249;           // 5Hz = 0.125s/interval (default)
  TIMSK1 = (1 << OCIE1A);  // enable compare match A interrupt

  // int0 trigger on logic change
  EICRA |= (1 << ISC11);  // falling-edge
  EIMSK |= (1 << INT0);   // enable interrrupt

  // int0 trigger on logic change
  EICRA |= (1 << ISC01);  // falling-edge
  EIMSK |= (1 << INT1);   // enable interrrupt

  sei();

  while (1) {
  }
}

ISR(INT0_vect) {
  if (scaler < 3) {
    scaler++;
  } else {
    scaler = 1;
  }
}

ISR(INT1_vect) {
  if (direction == 1) {
    direction = 0;
    PORTB = 1;
  } else {
    direction = 1;
    PORTB = 1;
  }
}

ISR(TIMER1_COMPA_vect) {
  if (counter >= scaler) {
    if (direction == 1) {
      // if PORTB0-PORTB2 is 100
      if (!(PORTB & ~(1 << DDB2))) {
        PORTB = 1;
      } else {
        PORTB = PORTB << 1;
      }
    } else {
      // if PORTB0-PORTB2 is 001
      if (PORTB) { // !(PORTB & ~(1 << DDB0))
        PORTB = PORTB >> 1;
      } else {
        PORTB = 100; // somehow there's an invisible 1100 here
      }
    }

    counter = 1;
  } else {
    counter++;
  }
}