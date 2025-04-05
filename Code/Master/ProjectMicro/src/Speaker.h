#include <avr/io.h>

void speaker_init()
{
    // Set OC2 pin as output
    DDRD |= (1 << DDD7);

    // Set Timer2 to Fast PWM mode, non-inverted output
    TCCR2 |= (1 << COM21) | (1 << WGM20) | (0 << WGM21);

    // Set PWM pin as non-inverted
    TCCR2 &= ~(1 << COM20);

    // Set prescaler to 8 for desired PWM frequency
    TCCR2 |= (1 << CS21);

    OCR2 = 0;
}