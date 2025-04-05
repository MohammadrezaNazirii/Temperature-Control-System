#include "USART.h"

unsigned char received_data[4];
int counter = 0;
unsigned char pass[4] = {'1', '2', '3', '4'};
int flag;
int logged_in = 0;

void check_pass()
{
    flag = 1;
    for (int i = 0; i < 4; i++)
    {
        if (pass[i] != received_data[i])
        {
            PORTA ^= (1 << PORTA6);
            usart_send(0x00); // error: invalid pass
            flag = 0;
            logged_in = 0;
            break;
        }
    }
    if (flag)
    {
        PORTA ^= (1 << PORTA7);
        usart_send(0x01); // ack for valid pass
        logged_in = 1;
    }
}
