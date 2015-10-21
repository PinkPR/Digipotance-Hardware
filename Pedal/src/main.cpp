#include <Arduino.h>
#include <SPI.h>

#include "EHX_The_Mole.h"

#define LED_MANUAL  8

bool manual = true;
long pot_vals[POT_CNT];
bool btn_vals[BTN_CNT];
long pot_preset_vals[POT_CNT];
long btn_preset_vals[BTN_CNT];

/*DEF_MAKE_S_NAME
DEF_MAKE_REQ_INFO_MSG(POT_CNT, BTN_CNT)
DEF_MAKE_GET_STATE_REQ(POT_CNT, BTN_CNT)*/

long readPot(int analogPin, float r)
{
    float p = float(analogRead(analogPin));

    return long(((1024.0 - p) / 1024.0) * r);
}

bool readBtn(int analogPin)
{
    return analogRead(analogPin) > 10;
}

struct s_dpot
init_dpot(bool pot_type,
          long max_resistor,
          int range,
          int cnt)
{
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
    pot.current_value = 0;
    pot.name = name;

    pinMode(pot.cs_pin, OUTPUT);
    digitalWrite(pot.cs_pin, HIGH);

    return pot;
}

struct s_btn
init_btn(int digital_pin,
         int digital_input,
         int analog_pin,
         bool type,
         char* name)
{
    struct s_btn btn;

    btn.digital_pin = digital_pin;
    btn.digital_input = digital_input;
    btn.analog_pin = analog_pin;
    btn.type = type;
    btn.state = false;
    btn.name = name;

    pinMode(digital_pin, OUTPUT);
    pinMode(digital_input, INPUT);

    return btn;
}

struct s_set_state_msg
parse_set_state_msg()
{
    //while (Serial.available() <= 0);
    delay(100);

    struct s_set_state_msg msg;
    memset(&msg, 0, sizeof (struct s_set_state_msg));

    for (int i = 0; i < POT_CNT; i++)
    {
        msg.pot_values[i] = Serial.read();
    }

    msg.linefeed1 = Serial.read();

    for (int i = 0; i < BTN_CNT; i++)
    {
        msg.btn_values[i] = Serial.read();
    }

    msg.linefeed2 = Serial.read();

    return msg;
}

struct s_req_info_msg
make_req_info_msg(void)
{
    struct s_req_info_msg msg;
    memset(&msg, 0, sizeof (struct s_req_info_msg));

    msg.header = 'I';
    msg.linefeed1 = LF;
    msg.pot_cnt = POT_CNT;
    msg.linefeed2 = LF;
    msg.linefeed4 = LF;
    msg.linefeed5 = LF;

    for (int i = 0; i < POT_CNT; i++)
        strcpy((char*) &(msg.pot_names[i]), pots[i].name);

    msg.btn_cnt = BTN_CNT;
    msg.linefeed3 = LF;

    for (int i = 0; i < BTN_CNT; i++)
        strcpy((char*) &(msg.btn_names[i]), btns[i].name);

    return msg;
}

struct s_req_state_msg
make_req_state_msg(void)
{
    struct s_req_state_msg msg;
    memset(&msg, 0, sizeof (struct s_req_state_msg));
    float val = 0.0;

    msg.header = 'P';
    msg.linefeed1 = LF;

    for (int i = 0; i < POT_CNT; i++)
    {
        val = readPot(pots[i].analog_pin, float(pots[i].ohmm_resistor));
        val = 100.0 * float(val) / float(pots[i].ohmm_resistor);
        msg.pot_values[i] = long(val);
    }

    msg.linefeed2 = LF;

    for (int i = 0; i < BTN_CNT; i++)
        msg.btn_values[i] = btns[i].state;

    msg.linefeed3 = LF;

    return msg;
}

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

    val = int(range) - val;

    if (val > int(range))
        return;

    digitalWrite(pot.cs_pin, LOW);

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

void writeDigiPotPercent(struct s_pot pot, unsigned int val)
{
    if (val > 100)
        return;

    float resistor = (pot.dpot.sdpot_cnt ?
                        float(pot.dpot.sdpots[0].max_resistor)
                      : float(pot.dpot.sdpots[0].max_resistor));
    writeDigiPot(pot, resistor * float(val) / 100.0);
}

void writeBtn(struct s_btn btn, bool on)
{
    // behringer type
    if (!btn.type)
    {
        if (on != btn.state)
        {
            digitalWrite(btn.digital_pin, LOW);
            digitalWrite(btn.digital_pin, HIGH);
            delay(100);
        }
    }

    // TODO : normal switch
}

long get_percent(struct s_pot pot, long r)
{
    float val = float(r);
    val = 100.0 * val / float(pot.ohmm_resistor);
    return long(val);
}

void apply_preset(struct s_set_state_msg msg)
{
    int value = 0;

    for (int i = 0; i < POT_CNT; i++)
    {
        writeDigiPotPercent(pots[i], msg.pot_values[i]);
        pot_preset_vals[i] = msg.pot_values[i];
    }

    for (int i = 0; i < BTN_CNT; i++)
    {
        writeBtn(btns[i], msg.btn_values[i]);
        btns[i].state = msg.btn_values[i];
        btn_preset_vals[i] = msg.btn_values[i];
    }
}

void setup()
{
    Serial.begin(115200);
    SPI.begin();
    INIT_POTS()
    INIT_BTNS()
    pinMode(LED_MANUAL, OUTPUT);

    //Init pot values
    for (int i = 0; i < POT_CNT; i++)
        pots[i].current_value =
            readPot(pots[i].analog_pin, float(pots[i].ohmm_resistor));

    //Init btn value
    //for (int i = 0; i < BTN_CNT; i++)
        //btns[i].state = readBtn(btns[i].analog_pin);

    delay(300);
    pinMode(7, OUTPUT);
}

void manual_loop()
{
    float ratio = 0.0;
    long percent = 0;

    for (int i = 0; i < POT_CNT; i++)
    {
        pot_vals[i] = readPot(pots[i].analog_pin, float(pots[i].ohmm_resistor));
        percent = get_percent(pots[i], pot_vals[i]);

        if (!manual && ((percent - pot_preset_vals[i]) < 5 || (percent - pot_preset_vals[i]) > 5))
            break;

        manual = true;
        pots[i].current_value = pot_vals[i];
        writeDigiPot(pots[i], pots[i].ohmm_resistor - pot_vals[i]);
    }

    for (int i = 0; i < BTN_CNT; i++)
    {
        if (digitalRead(btns[i].digital_input))
        {
            writeBtn(btns[i], !btns[i].state);
            btns[i].state = !btns[i].state;
            manual = true;
        }

    }
}

void loop()
{
    digitalWrite(LED_MANUAL, manual);

    char req = Serial.read();
    char* data = 0;

    if (req == 'S')
    {
        manual = false;
        delay(100);
        Serial.read();
        apply_preset(parse_set_state_msg());
    }
    else if (req == 'I')
    {
        Serial.read();
        struct s_req_info_msg info = make_req_info_msg();
        data = (char*) &info;

        for (int i = 0; i < (sizeof (struct s_req_info_msg)); i++)
            Serial.print((char) data[i]);
    }
    else if (req == 'G')
    {
        Serial.read();
        struct s_req_state_msg state = make_req_state_msg();
        data = (char*) &state;

        for (int i = 0; i < (sizeof (struct s_req_state_msg)); i++)
            Serial.print((char) data[i]);
    }

    manual_loop();
}
