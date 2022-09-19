#include "DIO_interface.h"
#include "DIO_private.h"
#include "TIMER1_interface.h"
#include "USART_interface.h"

#define F_CPU 8000000UL

int main()
{
    TIMER1_voidInit();
    DIO_voidSetPinDirection(PORTD, OC1A_PIN, OUTPUT);
    USART_voidInit();
    u8 duty = 0, frame[30], flag = 0, done = 0;
    u16 frequency = 0;
    while (1)
    {
        if (!done)
        {
            USART_voidReceiveString(frame, '.');
            u8 i = 10;
            while (frame[i] != '\0')
            {
                if (frame[i] == ',')
                {
                    i++;
                    flag++;
                    continue;
                }
                if (!flag)
                    frequency = frequency * 10 + frame[i] - '0';
                else
                    duty = duty * 10 + frame[i] - '0';
                i++;
            }
            done = 1;
        }
        else
            TIMER1_voidGeneratePWM(COMP_A, frequency, duty / 100.0f);
    }
}