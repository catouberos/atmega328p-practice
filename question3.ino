#include <avr/io.h>

int gpio_1_enabled = 0;
int count = 0;
int pressed = 0;

int main(void) {
  DDRD &= ~(1 << DDD2);  // PORTD2 as input

  DDRB |= (1 << DDB3) | (1 << DDB4);  // PORTB3-4 as output

  // clock 1
  TCCR1B |= (1 << WGM12);  // CTC mode
  TCCR1B |= (1 << CS12);   // prescaler of 256
  OCR1A = 1249;            // 25Hz
  TIMSK1 = (1 << OCIE1A);  // enable compare match A interrupt

  // int0 trigger on logic change
  EICRA |= (1 << ISC01);  // falling-edge
  EIMSK |= (1 << INT0);   // enable interrrupt

  sei();

  while (1) {
  }
}

ISR(INT0_vect) {
  pressed = 1;
}

ISR(TIMER1_COMPA_vect) {
  if (pressed == 1) {
    if (gpio_1_enabled == 1) {
      if (count == 6) {
        PORTB &= ~(1 << PORTB3);
        PORTB &= ~(1 << PORTB4);

        count = 0;
        gpio_1_enabled = 0;
        pressed = 0;
      } else {
        count++;
        PORTB ^= (1 << PORTB4);
      }
    } else {
      count = 0;
      PORTB |= (1 << PORTB3);
      gpio_1_enabled = 1;
    }
  }
}