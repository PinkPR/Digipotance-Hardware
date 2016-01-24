#ifndef PEDAL_H
# define PEDAL_H

/*  PC ---> PEDAL
 *  ============
 *
 *  Pedal Info
 *      I\n
 *
 *  Pots/buttons state
 *      Get:
 *          G\n
 *
 *      Set:
 *          S\n
 *          <XX>+   (N times a value in range [0; 99] with N = pots count)
 *          <Y>+    (N times a value in range [0; 1] with N = btns count)
 *
 *
 *  PEDAL ---> PC
 *  =============
 *
 *  Pedal Info
 *      I\n
 *      N\n         (pot cnt)
 *      <Name\n>+   (N times pot name)
 *      M\n         (btn cnt)
 *      <Name\n>+   (M times btn name)
 *
 *  Pots/buttons state
 *
 *      P\n
 *      <XX>+\n     (N times a value in range [0; 99] with N = pots count)
 *      <Y>+\n      (N times a value in range [0; 1] with N = btns count)
 *
 *
 *
 *
 */

# define NAME_MAX_SIZE      16 /**< Max size of pedal name */
# define LF                 '\n' /**< shortcut for printing linefeed character */

/**
*   Defines struct for a name
*/
# define DEF_STRUCT_NAME_ARRAY(n)   \
    struct s_name                   \
    {                               \
        char name[n];               \
    } __attribute__((__packed__));

DEF_STRUCT_NAME_ARRAY(NAME_MAX_SIZE)

/**
*   Defines struct for set_state message
*/
# define DEF_STRUCT_SET_STATE_MSG(p_cnt, b_cnt) \
    struct s_set_state_msg                      \
    {                                           \
        unsigned char pot_values[p_cnt];        \
        char linefeed1;                         \
        unsigned char btn_values[b_cnt];        \
        char linefeed2;                         \
    } __attribute__((__packed__));

/**
*   Defines struct for set_info request
*/
# define DEF_STRUCT_SET_INFO_REQ(p_cnt, b_cnt)  \
    struct s_req_info_msg                       \
    {                                           \
        char header;                            \
        char linefeed1;                         \
        char pot_cnt;                           \
        char linefeed2;                         \
        struct s_name pot_names[p_cnt];         \
        char linefeed3;                         \
        char btn_cnt;                           \
        char linefeed4;                         \
        struct s_name btn_names[b_cnt];         \
        char linefeed5;                         \
        char pedal_brand[NAME_MAX_SIZE];        \
        char linefeed6;                         \
        char pedal_model[NAME_MAX_SIZE];        \
        char linefeed7;                         \
    } __attribute__((__packed__));

/**
*   Defines struct for get_state request
*/
# define DEF_STRUCT_GET_STATE_REQ(p_cnt, b_cnt) \
    struct s_req_state_msg                      \
    {                                           \
        char header;                            \
        char linefeed1;                         \
        char pot_values[p_cnt];                 \
        char linefeed2;                         \
        char btn_values[b_cnt];                 \
        char linefeed3;                         \
    } __attribute__((__packed__));

/**
*   struct defining a simple digipot
*/
struct s_simple_dpot
{
    int range;
    long max_resistor;
};

/**
*   struct defining a double digipot
*/
struct s_double_dpot
{
    int range;
    long max_resistor;
};

/**
*   struct defining a digipot
*/
struct s_dpot
{
    int sdpot_cnt;
    int ddpot_cnt;
    long current_value;
    struct s_simple_dpot sdpots[5];
    struct s_double_dpot ddpots[5];
};

/**
*   struct defining a digipot associated to an analog pot
*/
struct s_pot
{
    struct s_dpot dpot;
    int analog_pin;
    int cs_pin;
    long ohmm_resistor;
    long current_value;
    char* name;
};

// type :   true    -> normal switch
//          false   -> behringer switch
/**
*   struct defining an emulated button associated to an analog button
*/
struct s_btn
{
    int digital_pin;
    int digital_input;
    int analog_pin;
    bool type;
    bool state;
    char* name;
};

# include "Pedal2.h"

#endif /* PEDAL_H */
