#include <avr/io.h>
#include <util/delay.h>

void usart_init()
{
    UBRRH = 0x00;
    UBRRL = 0x33; // (8mhZ / 16 (9600)) - 1 = 51 = 0X33
    UCSRB = (1 << RXEN) | (1 << TXEN);
    UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
}

// USART Send a single character
void usart_send(unsigned char data)
{
    _delay_ms(20);
    while (!(UCSRA & (1 << UDRE)))
        ;
    UDR = data;
}

// USART Send a string
void usart_send_string(const char *str)
{
    while (*str)
    {
        usart_send(*str++);
    }
}

// USART Receive a single character
unsigned char usart_receive()
{
    while (!(UCSRA & (1 << RXC)))
        ;
    return UDR;
}
