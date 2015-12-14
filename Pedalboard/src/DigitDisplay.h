#ifndef DIGIT_DISPLAY_H
# define DIGIT_DISPLAY_H

# include <Arduino.h>

# define PRINT_POINT  10/**<Magic number defining print point instruction */

/** @defgroup display_en_segments Segments Enabled for Displaying Digits
    Defines segments to be enabled on display to represent digits
    @{
*/
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
/**
    @}
*/

/**
* \class DigitDisplay DigitDisplay.h
* \author PinkPR
*/
class DigitDisplay
{
    public:
        /**
        * \brief Constructor for class \b DigitDisplay
        * \author PinkPR
        * \param pX GPIO pin wired to the digit display pins. p3 and p8 are
        * willingly omitted because they are cathods of the display.
        */
        DigitDisplay(int p1,
                     int p2,
                     int p4,
                     int p5,
                     int p6,
                     int p7,
                     int p9,
                     int p10);

    public:
        /**
        * \brief Print the given digit on the display.
        * \author PinkPR
        * \param n The digit to be printed. \a n must be in [0; 10] interval.
        * If the \n given is 10, then a point will be printed.
        */
        void print(int n);

    private:
        /**
        * \brief Clear the display.
        * \author PinkPR
        */
        void clear();

    private:
        /**
        * Private attribute representing the GPIO pins wired to the display pins.
        */
        int _pins[8];
};

#endif /* DIGIT_DISPLAY_H */
