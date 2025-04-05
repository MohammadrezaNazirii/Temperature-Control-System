#include <string.h>

#include <avr/io.h>
#include <util/delay.h>

#define LCD_DATA PORTA
#define ctrl PORTD

void LCD_cmd(unsigned char cmd)
{
    LCD_DATA = cmd;
    ctrl = 0x40; // Register Select = 0, Read/Write = 0, Enable = 1
    _delay_ms(1);
    ctrl = 0x00; // Enable = 0
    _delay_ms(1);
    return;
}

void init_LCD(void)
{
    LCD_cmd(0x38); // 8-bit mode
    _delay_ms(1);
    LCD_cmd(0x01); // clear the screen
    _delay_ms(1);
    LCD_cmd(0x0E); // turn on the cursor
    // LCD_cmd(0x0f);         // make cursor blinking
    _delay_ms(1);
    LCD_cmd(0x80); // move cursor to the first place of the first row
    _delay_ms(1);
    return;
}

void LCD_write(unsigned char data)
{
    LCD_DATA = data;
    ctrl = 0x50; // Register Select = 1, Read/Write = 0, Enable = 1
    _delay_ms(1);
    ctrl = 0x10; // Enable = 0
    _delay_ms(2);
    return;
}

void LCD_write_string(char *string)
{
    // LCD_cmd(0x01); // Clear the screen
    for (int character_index = 0; character_index < strlen(string); character_index++)
        LCD_write(string[character_index]);
}

void LCD_init()
{
    DDRA |= (1 << DDA7) | (1 << DDA6) | (1 << DDA5) | (1 << DDA4) | (1 << DDA3) | (1 << DDA2) | (1 << DDA1) | (1 << DDA0);
    DDRD |= (1 << DDD6) | (1 << DDD5) | (1 << DDD4);
    init_LCD();
}

void LCD_delete_last_char()
{
    LCD_cmd(0x10);  // Move the cursor to the left
    LCD_write(' '); // Write a space
    LCD_cmd(0x10);  // Move the cursor to the left
}

void print_login()
{
    LCD_cmd(0x01); // Clear the screen
    LCD_write_string("Enter Password: ");
}

void print_menu()
{
    LCD_cmd(0x01); // Clear the screen
    LCD_write_string("1. Motors Status    ");
    LCD_write_string("2. Temperature");
}