#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Motor.h"
#include "Temp.h"
#include "Pass.h"
#include "Decryption.h"

int main(void)
{
  usart_init();
  motors_init();

  DDRA = (1 << DDA6) | (1 << DDA7);

  DDRC = 0xFF;

  sei(); // set I in SReg

  while (1)
  {
    int temperature = read_temperature();

    if (logged_in)
    {
      motors_control(temperature);
      usart_send(temperature);
      _delay_ms(2);
      int tmp1 = (motor1 * 64) + (motor2 * 32) + (motor3 * 16) + 128 + dutyCycle / 10;

      usart_send(tmp1);

      check_state2();
      check_state3();
      check_state1();

      PORTC = (motor3) | (motor2 * 2) | (motor1 * 4);
    }
    else
    {
      uint8_t temp = usart_receive();
      encrypted[counter++] = temp;
      if (counter == 16)
      {
        counter = 0;
        // decrypt
        decrypt_password(encrypted, key, &password);
        // int to received data
        for (int i = 3; i >= 0; i--)
        {
          received_data[i] = (password % 10) + '0';
          password /= 10;
        }
        check_pass();
      }
    }
  }
}