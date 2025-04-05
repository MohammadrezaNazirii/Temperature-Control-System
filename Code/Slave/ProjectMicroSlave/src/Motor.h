#include <avr/io.h>

double dutyCycle = 0;

int motor1;
int motor2;
int motor3;
int error = 0;

void motors_init()
{
    // Set PD4 and PD5 pins as output
    DDRD |= (1 << DDD4) | (1 << DDD5);

    // Set Timer1 to Fast PWM mode, non-inverted output
    TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10);
    TCCR1B |= (1 << WGM12) | (1 << CS11);

    // Set PWM pins as non-inverted
    TCCR1A &= ~(1 << COM1A0);
    TCCR1A &= ~(1 << COM1B0);

    // Set OC2 pin as output
    DDRD |= (1 << DDD7);

    // Set Timer2 to Fast PWM mode, non-inverted output
    TCCR2 |= (1 << COM21) | (1 << WGM20) | (1 << WGM21);

    // Set PWM pin as non-inverted
    TCCR2 &= ~(1 << COM20);

    // Set prescaler to 8 for desired PWM frequency
    TCCR2 |= (1 << CS21);

    DDRB = (0 << PINB0) | (0 << PINB1) | (0 << PINB2);
}

void check_state1()
{
    motor1 = 1;

    if (!(PINB >> PINB0))
    {
        _delay_ms(2);
        if (!(PINB >> PINB0))
        {
            _delay_ms(2);
            if (!(PINB >> PINB0))
            {
                motor1 = 0;
            }
        }
    }
}
void check_state2()
{
    motor2 = 1;

    if (!(PINB >> PINB1))
    {
        _delay_ms(2.98);
        if (!(PINB >> PINB1))
        {
            _delay_ms(2);
            if (!(PINB >> PINB1))
            {
                motor2 = 0;
            }
        }
    }
}

void check_state3()
{
    motor3 = 1;

    if (!(PINB >> PINB2))
    {
        _delay_ms(2);
        if (!(PINB >> PINB2))
        {
            _delay_ms(2);
            if (!(PINB >> PINB2))
            {
                motor3 = 0;
            }
        }
    }
}

void motors_control(int temperature)
{ // calculate duty cycle

    check_state1();
    check_state2();
    check_state3();

    int coeff = 30;
    int motors = motor1 + motor2 + motor3;

    if (motors != 0)
    {
        coeff = 30 / motors;
    }

    int temp = temperature / 10;

    dutyCycle = coeff * temp;
    error = 0;
    if (dutyCycle > 100)
    {
        dutyCycle = 100;
        error = 1;
    }
    if (temperature >= 100)
    {
        dutyCycle = 100;
        if (motors < 3)
        {
            error = 1;
        }
    }

    OCR1A = (dutyCycle / 100) * 255;
    OCR1B = (dutyCycle / 100) * 255;
    OCR2 = (dutyCycle / 100) * 255;
}
