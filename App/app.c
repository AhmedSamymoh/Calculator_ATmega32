/*
 * app.c
 *
 *  Created on: Feb 6, 2024
 *      Author: AhmedSamy
 */
#include "app.h"




uint8 Buzzer = PIN_0;
uint8 key =	0xff;
char total[16] = {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'};
uint8 totalCounter = 0;
uint8 num1Arr[7] = {0};
uint8 num2Arr[7] = {0};
sint32 num1 = 0;
sint32 num2 = 0;
sint8 counter1 = 0;
sint8 counter2 = 0;
uint32 multiplier = 1;
uint32 multiplier1 = 1;
uint32 multiplier2 = 1;
sint32 result = 66;
uint16 floating = 0;
uint8 flag1 = 0;
uint8 flag2 = 0;
uint8 flag3 = 0;
char op;
uint8 opPosition;



/**
 * @brief Manages the calculator application's functionality and user interaction.
 * - Initializes the calculator, displaying the title on the LCD screen.
 * - Listens for user input from a keypad to perform arithmetic operations.
 * - Handles division by zero and floating-point division scenarios.
 * - Allows the user to clear the calculator upon request.
 */
void Calculator(){
	LCD_Init();
	KEYPAD_init();

	DIO_SetPinDirection(PORT_B , Buzzer , PIN_OUTPUT);

	LCD_Set_Cursor(2u,4u);
	LCD_Write_String_Position("Calculator" , 2 , 4);
	_delay_ms(1000);
	LCD_Send_Command(1);

	while(1)
	{
		LCD_Send_Command(1);
		key = 0xff;
		for(uint8 i = 0; i < 16; i++)
		{
			total[i] = '#';
		}
		totalCounter = 0;
		for(uint8 i = 0; i < 7; i++)
		{
			num1Arr[i] = 0;
		}
		for(uint8 i = 0; i < 7; i++)
		{
			num2Arr[i] = 0;
		}
		num1 = 0;
		num2 = 0;
		counter1 = 0;
		counter2 = 0;
		multiplier = 1;
		multiplier1 = 1;
		multiplier2 = 1;
		result = 0;
		floating = 0;
		flag1 = 0;
		flag2 = 0;
		flag3 = 0;
		_delay_ms(125);

		while(key != 'c')
		{
			while(key != '=')
			{
				do
				{
					key = KEYPAD_GetPressedKey();
				}
				while(key == 0xff);
				Sound();

				if(key == 'c')
				{
					break;
				}

				total[totalCounter] = key;
				totalCounter++;

				if((key >= 0) && (key <= 9))
				{
					LCD_Send_Number(key);
					_delay_ms(50);
				}
				else
				{

					LCD_Send_Data(key);
					_delay_ms(50);
				}
			}

			if(key == 'c')
			{
				break;
			}

			totalCounter = 0;

			LCD_Set_Cursor(2u,1u);

			if(total[totalCounter] == '-')
			{
				flag1 = 1;
				totalCounter++;
			}

			for(; (total[totalCounter] >= 0) && (total[totalCounter] <= 9); totalCounter++)
			{
				if(counter1 < 7)
				{
					num1Arr[(uint8)counter1] = total[totalCounter];
					counter1++;
				}
			}
			counter1--;

			op = total[totalCounter];
			opPosition = totalCounter;

			multiplier = 1;
			for(; counter1 >= 0 ; counter1--)
			{
				num1 = num1 + (num1Arr[(uint8)counter1] * multiplier);
				multiplier = multiplier * 10;
			}

			if(flag1 == 1)
			{
				num1 = -1 * num1;
			}

			for(; !((total[totalCounter] >= 0) && (total[totalCounter] <= 9)); totalCounter++);

			if((total[totalCounter-1] == '-') && ((totalCounter-1) != opPosition ))
			{
				flag2 = 1;
			}

			for(; (total[totalCounter] >= 0) && (total[totalCounter] <= 9); totalCounter++)
			{
				if(counter2 < 7)
				{
					num2Arr[(uint8)counter2] = total[totalCounter];
					counter2++;
				}
			}
			counter2--;

			multiplier = 1;
			for(; counter2 >= 0 ; counter2--)
			{
				num2 = num2 + (num2Arr[(uint8)counter2] * multiplier);
				multiplier = multiplier * 10;
			}

			if(flag2 == 1)
			{
				num2 = -1 * num2;
			}

			switch(op)
			{
			case '+': result = num1 + num2; break;
			case '-': result = num1 - num2; break;
			case '*': result = num1 * num2; break;
			case '/': flag3 = 1; break;
			default:  result = num1 + num2; break;
			}



			if((flag3 == 1) && (num2 == 0))
			{
				LCD_Set_Cursor(4u,1u);
				LCD_Write_String("Math Error");
			}
			else if(flag3 == 1)
			{
				result = num1 / num2;
				if(result == 0)
				{
					if(((num1<0) && (num2>0)) || ((num1>0) && (num2<0)))
					{
						LCD_Send_Data('-');
					}
				}

				LCD_Send_Number(result);
				if((op == '/') && (num1 % num2 != 0))
				{
					if(num1 < 0)
					{
						num1 = -1 * num1;
					}
					if(num2 < 0)
					{
						num2 = -1 * num2;
					}
					LCD_Send_Data('.');
					multiplier1 = 10;
					multiplier2 = 10000;
					while(((((num1 % num2) * multiplier1) / num2) == 0) && (multiplier1<10000))
					{
						LCD_Send_Number(0);
						multiplier1 = multiplier1 * 10;
					}
					floating = ((num1 % num2) * multiplier2) / num2;
					LCD_Send_Number(floating);
				}
			}
			else
			{
				LCD_Send_Number(result);
			}

			key = 0xff;
			do
			{
				key = KEYPAD_GetPressedKey();
			}while(key != 'c');
		}
	}

}


void Sound(){
	DIO_SetPinValue(PORT_B , Buzzer , PIN_HIGH);
	_delay_ms(200);
	DIO_SetPinValue(PORT_B , Buzzer , PIN_LOW);
	_delay_ms(200);
}
