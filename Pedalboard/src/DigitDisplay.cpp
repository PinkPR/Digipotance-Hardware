#include "DigitDisplay.h"

int digits[][8] = { ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, POINT };

DigitDisplay::DigitDisplay(int p1,
                           int p2,
                           int p4,
                           int p5,
                           int p6,
                           int p7,
                           int p9,
                           int p10)
{
    _pins[0] = p1;
    _pins[1] = p2;
    _pins[2] = p4;
    _pins[3] = p5;
    _pins[4] = p6;
    _pins[5] = p7;
    _pins[6] = p9;
    _pins[7] = p10;

    for (int i = 0; i < 8; i++)
    {
        pinMode(_pins[i], OUTPUT);
        digitalWrite(_pins[i], HIGH);
    }
}

void
DigitDisplay::print(int n)
{
    if (n > 10)
        return;

    this->clear();

    for (int i = 0; i < 8; i++)
        digitalWrite(_pins[i], digits[n - 1][i]);
}

void
DigitDisplay::clear()
{
    for (int i = 0; i < 8; i++)
        digitalWrite(_pins[i], HIGH);
}
