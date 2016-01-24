#include <Arduino.h>

#include <LiquidCrystal.h>
#include "Pedalboard.h"
#include "DigitDisplay.h"

DigitDisplay display(5, 6, 7, 8, 9, 10, 11, 12); /**< The digit display */

LiquidCrystal s1 = LiquidCrystal(36, 37, 38, 26, 27, 28, 29); /**< Screen 1*/
LiquidCrystal s2 = LiquidCrystal(40, 41, 42, 30, 31, 32, 33); /**< Screen 2 */

LiquidCrystal* screens[BANK_COUNT] = { &s1, &s2 }; /**< Array of Screens */

int preset_inputs[PRESET_COUNT] = { 46, 47 }; /**< Array of digital inputs for
    preset buttons */
int bank_inputs[2] = { 48, 49 }; /**< Array of digital inputs for banks */

int bank = 1; /**< Current bank. Starts at 1 and is in [1; BANK_COUNT[
    interval */
int current_preset = 0; /**< Current preset on bank. Starts at 0 and is in
    [0; PRESET_COUNT[ interval*/
bool init_done = false; /**< Boolean value to wait for the init(first preset
    loading) */
bool screen_update = false; /**< Boolean value to know when to update LCD
    screens */
char presets[BANK_COUNT * PRESET_COUNT * PRESET_NAME_LEN]; /**< Preset names
    array. Gets initialized after after first preset name loading. */

/**
* \brief Reads the preset names datas on serial and updates presets.
* \author PinkPR
* \warning If the connection gets lost while reading, the pedalboard gets
*   locked in an infinite loop and must be restarted.
*/
void get_data()
{
    delay(100);
    Serial.read();
    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);

    for (int i = 0; i < (BANK_COUNT * PRESET_COUNT * PRESET_NAME_LEN); i++)
    {
        ((char*) presets)[i] = Serial.read();

        if (presets[i] == -1)
            i--;
    }
    digitalWrite(2, LOW);

    bank = 1;
    display.print(bank);

    for (int j = 0; j < PRESET_COUNT; j++)
    {
        screens[j]->clear();
        screens[j]->setCursor(0, 0);
        screens[j]->print(((char*) presets)[PRESET_NAME_LEN * j]);
    }

    screen_update = true;
}

/**
* \brief Poll for a bank update. Read on digital bank inputs and update if
*   needed.
* \author PinkPR
*/
void bank_inputs_loop()
{
    if (digitalRead(bank_inputs[0]))
    {
        screen_update = true;
        bank++;
        if (bank > BANK_COUNT)
            bank = 1;
        delay(100);
    }
    else if (digitalRead(bank_inputs[1]))
    {
        screen_update = true;
        bank--;
        if (bank < 1)
            bank = BANK_COUNT;
        delay(100);
    }

    display.print(bank);
}

/**
* \brief Poll for a preset update. Read on digital preset inputs and update if
*   needed and sends update to master.
* \author PinkPR
*/
void preset_inputs_loop()
{
    for (int i = 0; i < PRESET_COUNT; i++)
    {
        if (digitalRead(preset_inputs[i]))
        {
            current_preset = i;
            Serial.print("S\n");
            Serial.print((char) ((bank - 1) * PRESET_COUNT + current_preset));
            Serial.print('\n');
            delay(500);
        }
    }
}

/**
* \brief Poll for a screen update. If needed, display new preset names.
* \author PinkPR
*/
void screens_update_loop()
{
    if (!screen_update)
        return;

    screen_update = false;

    for (int j = 0; j < PRESET_COUNT; j++)
    {
        screens[j]->clear();
        screens[j]->setCursor(0, 0);
        screens[j]->print((char*) presets + (((bank - 1) * PRESET_COUNT + j) * PRESET_NAME_LEN));
    }
}

/**
* \brief Arduino main init loop.
* \author PinkPR
*
* Init serial, LCD screens, digit display, and pin in/out modes.
*/
void setup()
{
    Serial.begin(115200);

    for (int i = 0; i < 2; i++)
    {
        screens[i]->begin(16, 2);
        screens[i]->clear();
        screens[i]->print("No Data");
    }

    display.print(PRINT_POINT);

    for (int i = 0; i < PRESET_COUNT; i++)
        pinMode(preset_inputs[i], INPUT);

    pinMode(bank_inputs[0], INPUT);
    pinMode(bank_inputs[1], INPUT);
}

/**
* \brief Arduino main loop.
* \author PinkPR
*
* Poll for master requests.
*/
void loop()
{
    if (!init_done)
    {
        if (Serial.read() == 'S')
        {
            get_data();
            init_done = true;
        }
        else
        {
            return;
        }
    }

    if (Serial.read() == 'S')
    {
        get_data();
        init_done = true;
    }

    preset_inputs_loop();
    bank_inputs_loop();
    screens_update_loop();
}
