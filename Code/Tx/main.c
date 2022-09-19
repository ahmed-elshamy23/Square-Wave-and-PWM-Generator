#include "DIO_interface.h"
#include "DIO_private.h"
#include "KPD_interface.h"
#include "LCD_interface.h"
#include "LCD_private.h"
#include "USART_interface.h"

#define F_CPU 8000000UL

int main()
{
    USART_voidInit();
    LCD_voidInit();
    LCD_voidSendCommand(cursorOff);
    KPD_voidInit(PORTA);
    u8 flag = 0, duty = 0;
    u16 frequency = 0;
    LCD_voidSendString((u8 *)("Frequency: "));
    LCD_voidSetCursorPosition(1, 0);
    USART_voidSendString("Frequency,");
    while (1)
    {
        while (KPD_u8GetPressedKey(PORTA) != NOT_PRESSED)
        {
            u8 key = KPD_u8GetPressedKey(PORTA);
            if (key == '=')
            {
                if (flag == 2)
                    USART_voidSendData('.');
                else
                    USART_voidSendData(',');
                flag++;
                break;
            }
            LCD_voidSendChar(key);
            USART_voidSendData(key);
            if (flag == 0)
                frequency = frequency * 10 + key - '0';
            else
                duty = duty * 10 + key - '0';
        }
        if (flag == 1)
        {
            LCD_voidSendCommand(clearDisplay);
            LCD_voidSendString((u8 *)("Duty Cycle: "));
            LCD_voidSetCursorPosition(1, 0);
            flag++;
        }
        else if (flag == 3)
        {
            LCD_voidSendCommand(clearDisplay);
            LCD_voidSendString((u8 *)("Frequency: "));
            LCD_voidSendNumber(frequency);
            LCD_voidSetCursorPosition(1, 0);
            LCD_voidSendString((u8 *)("Duty: "));
            LCD_voidSendNumber(duty);
            LCD_voidSendChar('%');
            break;
        }
    }
}