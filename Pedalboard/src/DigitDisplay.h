#ifndef DIGIT_DISPLAY_H
# define DIGIT_DISPLAY_H

# include <Arduino.h>

# define PRINT_POINT  10

# define ONE    { 1, 1, 0, 1, 0, 1, 1, 1 }
# define TWO    { 0, 0, 1, 1, 0, 0, 1, 0 }
# define THREE  { 1, 0, 0, 1, 0, 0, 1, 0 }
# define FOUR   { 1, 1, 0, 1, 0, 1, 0, 0 }
# define FIVE   { 1, 0, 0, 1, 1, 0, 0, 0 }
# define SIX    { 0, 0, 0, 1, 1, 0, 0, 0 }
# define SEVEN  { 1, 1, 0, 1, 0, 0, 1, 1 }
# define EIGHT  { 0, 0, 0, 1, 0, 0, 0, 0 }
# define NINE   { 1, 0, 0, 1, 0, 0, 0, 0 }
# define POINT  { 1, 1, 1, 0, 1, 1, 1, 1 }

class DigitDisplay
{
    public:
        DigitDisplay(int p1,
                     int p2,
                     int p4,
                     int p5,
                     int p6,
                     int p7,
                     int p9,
                     int p10);

    public:
        void print(int n);

    private:
        void clear();

    private:
        int _pins[8];
};

#endif /* DIGIT_DISPLAY_H */
