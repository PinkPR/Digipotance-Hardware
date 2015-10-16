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

# define NAME_MAX_SIZE      16
# define LF                 '\n'

# define DEF_STRUCT_NAME_ARRAY(n)   \
    struct s_name                   \
    {                               \
        char name[n];               \
    } __attribute__((__packed__));

DEF_STRUCT_NAME_ARRAY(NAME_MAX_SIZE)

# define DEF_STRUCT_SET_STATE_MSG(p_cnt, b_cnt) \
    struct s_set_state_msg                      \
    {                                           \
        unsigned char pot_values[p_cnt];        \
        char linefeed1;                         \
        unsigned char btn_values[b_cnt];        \
        char linefeed2;                         \
    } __attribute__((__packed__));

# define DEF_STRUCT_SET_INFO_REQ(p_cnt, b_cnt)  \
    struct s_req_info_msg                       \
    {                                           \
        char header;                            \
        char linefeed1;                         \
        char pot_cnt;                           \
        char linefeed2;                         \
        struct s_name pot_names[p_cnt];         \
        char btn_cnt;                           \
        char linefeed3;                         \
        struct s_name btn_names[b_cnt];         \
    } __attribute__((__packed__));

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

struct s_simple_dpot
{
    int range;
    long max_resistor;
};

struct s_double_dpot
{
    int range;
    long max_resistor;
};

struct s_dpot
{
    int sdpot_cnt;
    int ddpot_cnt;
    struct s_simple_dpot sdpots[5];
    struct s_double_dpot ddpots[5];
};

struct s_pot
{
    struct s_dpot dpot;
    int analog_pin;
    int cs_pin;
    long ohmm_resistor;
    char* name;
};

struct s_btn
{
    int digital_pin;
    char* name;
};

# include "Pedal2.h"

#endif /* PEDAL_H */
