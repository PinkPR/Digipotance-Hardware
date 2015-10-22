#include <Arduino.h>

#include <LiquidCrystal.h>
#include "Pedalboard.h"
#include "DigitDisplay.h"

#define P1  8
#define P2  9

DigitDisplay display(5, 6, 7, 8, 9, 10, 11, 12);

LiquidCrystal s1 = LiquidCrystal(36, 37, 38, 26, 27, 28, 29);
LiquidCrystal s2 = LiquidCrystal(40, 41, 42, 30, 31, 32, 33);

LiquidCrystal* screens[BANK_COUNT] = { &s1, &s2 };

int preset_inputs[PRESET_COUNT] = { 46, 47 };
int bank_inputs[2] = { 48, 49 };

int bank = 1;
int current_preset = 0;
bool init_done = false;
bool screen_update = false;
char presets[BANK_COUNT * PRESET_COUNT * PRESET_NAME_LEN];

void get_data()
{
    delay(100);
    Serial.read();

    for (int i = 0; i < (BANK_COUNT * PRESET_COUNT * PRESET_NAME_LEN); i++)
    {
        ((char*) presets)[i] = Serial.read();

        if (presets[i] == -1)
            i--;
    }

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

void preset_inputs_loop()
{
    for (int i = 0; i < PRESET_COUNT; i++)
    {
        if (digitalRead(preset_inputs[i]))
        {
            current_preset = i;
            Serial.print("S\n");
            Serial.print((char) (bank - 1) * PRESET_COUNT + current_preset);
            Serial.print('\n');
            delay(500);
        }
    }
}

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
