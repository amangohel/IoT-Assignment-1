#include "mbed.h"
#include "C12832.h"

Ticker ticker;

PwmOut speaker(D6);

DigitalOut led1(LED1);

InterruptIn sw2(SW2);
InterruptIn sw3(SW3);

C12832 lcd(D11, D13, D12, D7, D10);

int i = 0;

float x = 2000.0;
float y = 9000.0;

volatile int a_flag = 0;
volatile int d_flag = 0;
volatile int increment_flag = 0;

//Method to set increment flag to 1
void increment_triggered(void) 
{
    increment_flag = 1;
}

//Method to set speaker flag to 1
void ascending_f(void)
{
    a_flag = 1;
}

//Method to set descending pitch flag to 1
void descending_f(void)
{
    d_flag = 1;
}

//Method for playing the sound for ascending pitch
//This function increments the x variable with each iteration
//of the while loop. This in combination with the speaker produces a
//pitch of ascending order until the limit of 8000 is reached, upon which
//the values of x, speaker and the a_flag are reset for the next call.
void play_a_sound(void)
{
        speaker.period(1.0/x);
        x += 100;
        speaker = 0.5;
        wait(0.02);
        
        if(x >= 8000.0)
        {
            x = 2000.0;
            speaker = 0.0;
            a_flag = 0;
        }
}

//Method for playing the sound for descending pitch
//This function decrements the y variable with each iteration
//of the while loop. This in combination with the speaker produces a
//pitch of descending order until the limit of 100 is reached, upon which
//the values of y, speaker and the d_flag are reset for the next call.
void play_d_sound(void) 
{  
        speaker.period(1.0/y);
        y -= 100;
        speaker = 0.5;
        wait(0.02);
        
        if(y <= 100.0)
        {
            y = 9000.0;
            speaker = 0.0;
            d_flag = 0;
        }
}

//Method to increment and print value of the counter variable
//This function will firstly check if the modulo of i is equal to 0, if true
//blink red, else close led1. Secondly, the current value of i is printed
//directly onto the screen of the Mbed.
void print_counter()
{
    if(i > 0 && i % 10 == 0)
    {
        led1 = 0;
    } else 
    {
        led1 = 1;
    }
    lcd.locate(0,3);
    lcd.printf("%d", i++);
}

//The main function contains a ticker which will periodically call
//the increment_triggered flag method to set its value to one, upon doing so,
//the print_counter method will be called within the while loop, 
//incrementing the value.
//
//Secondly, the sw2 and sw3 fall method calls set the relevant flags to play
//sounds of either ascending or descending pitch within the while loop. 
//Note: SW2 takes presedence over SW3 and does not interfere.
int main() 
{ 
    ticker.attach(increment_triggered, 1.0);
    sw2.fall(ascending_f);
    sw3.fall(descending_f);
    
    while(1)
    {
        if(increment_flag) 
        {
            increment_flag = 0;
            print_counter();
        }
        if(a_flag)
        {
            d_flag = 0;
            y = 9000;
            play_a_sound();
        }
        if(d_flag && !a_flag)
        {
            play_d_sound();
        }
    }
}

    


