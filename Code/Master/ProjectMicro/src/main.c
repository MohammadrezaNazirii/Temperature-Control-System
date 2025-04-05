#include <stdio.h>
#include <math.h>
#include "LCD.h"
#include "Keypad.h"
#include "USART.h"
#include "Encryption.h"
#include "Speaker.h"

int logged_in = 0;
char temp_str[5];
int motor1;
int motor2;
int motor3;
int tempe;
int duty;
int error;
int pass_try = 0;

unsigned char input_data[4];
int counter = 0;
uint8_t encrypted[16];
int motors;

uint8_t tmp;

void manage_received_data(uint8_t tmp)
{
    if (tmp >= 128)
    {
        motor1 = (tmp & 0b01000000) / 64;
        motor2 = (tmp & 0b00100000) / 32;
        motor3 = (tmp & 0b00010000) / 16;
    }
    else
    {
        motors = motor1 + motor2 + motor3;
        PORTB &= 0b11111111;
        if (motors == 0)
        {
            PORTB = (1 << PORTB6);
            OCR2 = 150;
        }
        else if (motors == 3)
        {
            PORTB = (1 << PORTB7);
            OCR2 = 0;
        }
        else
        {
            PORTB = (1 << PORTB5);
            OCR2 = 0;
        }
        tempe = tmp;
        int coeff = 0;
        if (motors != 0)
        {
            coeff = 30 / motors;
        }

        int temp = tempe / 10;

        duty = coeff * temp;
        error = 0;
        if (duty > 100)
        {
            duty = 100;
            error = 1;
        }
        if (tempe >= 100)
        {
            duty = 100;
            if (motors < 3)
            {
                error = 1;
            }
        }

        if (error)
        {
            PORTB |= (1 << PORTB6);
        }
    }
}

int main()
{
    // Initializations
    LCD_init();
    keypad_init();
    usart_init();
    speaker_init();

    DDRA = 0xFF;
    DDRB |= (1 << DDB5) | (1 << DDB6) | (1 << DDB7);

    print_login();

    while (1)
    {
        if (logged_in == 0)
        {
            char key = keypad_read();
            if (key != '\0')
            {
                input_data[counter++] = key; // pass
                if (counter == 4)
                {
                    counter = 0;
                    // input data to int 4 digit
                    uint16_t pass = 0;
                    for (int i = 0; i < 4; i++)
                    {
                        pass *= 10;
                        pass += input_data[i] - '0';
                    }
                    // ecrypt;
                    encrypt_password(pass, keyy, encrypted);
                    for (int i = 0; i < 16; i++)
                    {
                        // _delay_ms(3);
                        usart_send(encrypted[i]);
                    }
                }
                LCD_write('X');
                _delay_ms(1);
            }

            if ((UCSRA & (1 << RXC)))
            {
                unsigned char received_data = usart_receive();
                if (received_data == 0x00)
                { // invalid pass
                    pass_try++;
                    LCD_write_string("invalid Password!");
                    _delay_ms(50);
                    if (pass_try == 3) // after 3 tries
                    {
                        pass_try = 0;
                        LCD_cmd(0x01);

                        for (size_t i = 0; i <= 30; i++)
                        {
                            int num = 30 - i;
                            char numStr[10]; // Buffer to hold the converted number

                            // Convert integer to string
                            sprintf(numStr, "%d", num);

                            // Concatenate the strings
                            LCD_cmd(0x01);
                            LCD_write_string(numStr);
                            LCD_write_string(" Sec");
                            _delay_ms(25);
                        }
                    }
                    print_login();
                }
                else if (received_data == 0x01)
                { // valid pass
                    PORTA = received_data;
                    LCD_write_string("Login Successful!");
                    _delay_ms(100);
                    logged_in = 1;
                }
            }
        }
        else if (logged_in)
        {
            print_menu();
            int back = 0;
            while (1)
            {
                tmp = usart_receive();
                manage_received_data(tmp);
                _delay_ms(2);
                tmp = usart_receive();
                manage_received_data(tmp);
                int last_tempe = 0;
                char key = keypad_read();
                if (key == '2')
                {
                    LCD_cmd(0x01);
                    LCD_write_string("Temprature:   ");
                    while (key != '*')
                    {
                        tmp = usart_receive();
                        manage_received_data(tmp);
                        last_tempe = tempe;
                        key = keypad_read();
                        if (key == '*')
                        {
                            back = 1;
                            break;
                        }
                        sprintf(temp_str, "%d", tempe);
                        LCD_write_string(temp_str);
                        _delay_ms(50);
                        for (int k = 0; k < (floor(log10(last_tempe)) + 1); k++)
                            LCD_delete_last_char();
                    }
                }
                else if (key == '1')
                {
                    LCD_cmd(0x01);

                    LCD_write_string("Motors:             ");
                    while (key != '*')
                    {
                        tmp = usart_receive();
                        manage_received_data(tmp);
                        key = keypad_read();
                        if (key == '*')
                        {
                            back = 1;
                            break;
                        }
                        LCD_cmd(0x01);
                        if (motor1)
                        {
                            LCD_write_string("1 ");
                        }
                        if (motor2)
                        {
                            LCD_write_string("2 ");
                        }
                        if (motor3)
                        {
                            LCD_write_string("3 ");
                        }
                        sprintf(temp_str, "%d", duty);
                        LCD_write_string(temp_str);
                        if (error)
                        {
                            LCD_write_string(" er.");
                        }
                        _delay_ms(50);
                    }
                }

                if (back)
                {
                    break;
                }
            }
        }
    }
}