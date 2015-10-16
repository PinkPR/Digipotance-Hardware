#include <Arduino.h>
#include <SPI.h>

#include "EHX_The_Mole.h"

#define R1      150.0
#define VIN     5.0

#define CS      10

DEF_MAKE_S_NAME
DEF_MAKE_REQ_INFO_MSG(POT_CNT, BTN_CNT)
DEF_MAKE_GET_STATE_REQ(POT_CNT, BTN_CNT)

struct s_dpot
init_dpot(bool pot_type,
          long max_resistor,
          int range,
          int cnt)
{
    Serial.println("INIT_DPOT");
    Serial.println(max_resistor);
    Serial.println(range);

    struct s_dpot dpot;

    dpot.sdpot_cnt = (pot_type ? 0 : cnt);
    dpot.ddpot_cnt = (pot_type ? cnt : 0);

    // simple digipots init
    for (int i = 0; !pot_type && i < cnt; i++)
    {
        dpot.sdpots[i].range = range;
        dpot.sdpots[i].max_resistor = max_resistor;
    }

    // double digipots init
    for (int i = 0; pot_type && i < cnt; i++)
    {
        dpot.ddpots[i].range = range;
        dpot.ddpots[i].max_resistor = max_resistor;
    }

    return dpot;
}

struct s_pot
init_pot(int analog_pin,
         int cs_pin,
         struct s_dpot dpot,
         long ohmm_resistor,
         char* name)
{
    struct s_pot pot;

    pot.analog_pin = analog_pin;
    pot.cs_pin = cs_pin;
    pot.dpot = dpot;
    pot.ohmm_resistor = ohmm_resistor;
    pot.name = name;

    pinMode(pot.cs_pin, OUTPUT);
    digitalWrite(pot.cs_pin, HIGH);

    return pot;
}

struct s_set_state_msg
parse_set_state_msg()
{
    //while (Serial.available() <= 0);
    delay(100);
    Serial.read();

    struct s_set_state_msg msg;

    for (int i = 0; i < POT_CNT; i++)
    {
        //while (Serial.available() <= 0);
        msg.pot_values[i] = Serial.read();
    }

    //while (Serial.available() <= 0);
    msg.linefeed1 = Serial.read();

    for (int i = 0; i < BTN_CNT; i++)
    {
        //while (Serial.available() <= 0);
        msg.btn_values[i] = Serial.read();
    }

    //while (Serial.available() <= 0);
    msg.linefeed2 = Serial.read();

    return msg;
}

long readPot(int analogPin, float r)
{
    float p = float(analogRead(analogPin));

    return long(((1024.0 - p) / 1024.0) * r);
}

/*void writeDigiPot(int value, int pin)
{
    if (value > 128)
        value = 128;

    digitalWrite(pin, LOW);
    SPI.transfer(0);
    SPI.transfer(value);
    digitalWrite(pin, HIGH);
}*/

void writeDigiPot(struct s_pot pot, long value)
{
    // false --> simple digipot
    // true  --> double digipot
    bool dpot_type = (pot.dpot.sdpot_cnt ? false : true);
    float pot_cnt = (dpot_type ? pot.dpot.ddpot_cnt * 2
                               : pot.dpot.sdpot_cnt);
    float max_resistor =
        float((dpot_type ? pot.dpot.ddpots[0].max_resistor
                         : pot.dpot.sdpots[0].max_resistor));

    float range = (dpot_type ? pot.dpot.ddpots[0].range
                             : pot.dpot.sdpots[0].range);

    int val =
        int(range * (float(value) / max_resistor) / float(pot_cnt));

    //DEBUG
    /*Serial.println("WRITE DIGIPOT");
    Serial.println(float(value));
    Serial.println(pot.dpot.ddpots[0].max_resistor);
    Serial.println(pot.dpot.sdpots[0].max_resistor);
    Serial.println(pot.name);
    Serial.println(dpot_type);
    Serial.println(pot_cnt);
    Serial.println(max_resistor);
    Serial.println(range);
    Serial.println(val);*/

    val = int(range) - val;

    if (val > int(range))
        return;

    digitalWrite(pot.cs_pin, LOW);
    Serial.println(val);

    // simple digipot
    if (!dpot_type)
    {
        SPI.transfer(0);
        SPI.transfer(val);
    }
    else // double digipot
    {
        SPI.transfer(0x13);
        SPI.transfer(val);
    }

    digitalWrite(pot.cs_pin, HIGH);
}

/*void apply_preset(struct s_set_state_msg msg)
{
    int value = 0;

    for (int i = 0; i < POT_CNT; i++)
    {
        value =
            int(128.0f * float(msg.pot_values[i]) / 100.0f);
        writeDigiPot(value, pots[i].cs_pin);
    }
}*/

long pot = 0;

void setup()
{
    Serial.begin(115200);
    INIT_POTS()
    SPI.begin();
    delay(300);
}

long vals[] = {0, 0, 0};

void loop()
{
    long new_vals[3];

    for (int i = 0; i < POT_CNT; i++)
        new_vals[i] = readPot(pots[i].analog_pin, float(pots[i].ohmm_resistor));

    Serial.print(new_vals[0]);
    Serial.print("\t");
    Serial.print(new_vals[1]);
    Serial.print("\t");
    Serial.println(new_vals[2]);

    for (int i = 0; i < POT_CNT; i++)
    {
        if (new_vals[i] != vals[i])
        {
            vals[i] = new_vals[i];
            writeDigiPot(pots[i], new_vals[i]);
        }
    }

    delay(250);

    /*if (pot != new_pot)
    {
        pot = new_pot;
        writeDigiPot(pots[0], pot);
    }*/
    //Serial.println(readPot(1, VIN, 150.0f));

    /*if (Serial.read() == 'S')
    {
        apply_preset(parse_set_state_msg());
        Serial.print('K');
    }*/
}
