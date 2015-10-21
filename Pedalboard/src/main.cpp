#include <Arduino.h>
#include <LiquidCrystal.h>

#include "DigitDisplay.h"

#define P1  8
#define P2  9

int state = 0;
int n = 0;
int inputs[] = { P1, P2 };
DigitDisplay display(5, 6, 7, 8, 9, 10, 11, 12);
LiquidCrystal lcd1(40, 41, 42, 30, 31, 32, 33);

void updateState(int *inputs, int input_cnt)
{
    for (int i = 0; i < input_cnt; i++)
    {
        if (digitalRead(inputs[i]))
            state = inputs[i];
    }
}

void setup()
{
    state = inputs[0];
    //pinMode(P1, INPUT);
    //pinMode(P2, INPUT);
    pinMode(22, INPUT);
    pinMode(23, INPUT);

    lcd1.begin(16, 2);
    lcd1.display();

    Serial.begin(115200);
}

void loop()
{
    display.print(n + 1);
    lcd1.print(n + 1);

    if (digitalRead(22))
    {
        n++;
        n %= 10;
        delay(500);
    }
    else if (digitalRead(23))
    {
        if (n > 0)
            n--;
        else
            n = 9;
        delay(500);
    }

    /*updateState(inputs, 2);

    if (Serial.available() > 0)
    {
        if (Serial.read() == 'S')
            Serial.print(state);
    }*/
}
