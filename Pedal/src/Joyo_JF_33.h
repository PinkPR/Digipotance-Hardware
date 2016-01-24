#ifndef JOYO_JF_33
# define JOYO_JF_33

# include "Pedal.h"

# define PEDAL_BRAND    "Joyo" /**<Pedal brand */
# define PEDAL_MODEL    "JF 33"    /**<Pedal model */
# define POT_CNT        3 /**<Number of digipots on the pedal */
# define BTN_CNT        1 /**<Number of emulated buttons on the pedal */

/**
*   Init pots array
*/
# define INIT_POTS()        \
    pots[0] = init_pot(A0,  \
                       3,   \
                       init_dpot(true, 50000L, 255, 1),    \
                       50000L,     \
                       "Time");    \
                            \
    pots[1] = init_pot(A1,  \
                       4,   \
                       init_dpot(true, 50000L, 255, 1),     \
                       50000L,  \
                       "Repeat");     \
    pots[2] = init_pot(A2,  \
                       5,   \
                       init_dpot(true, 50000L, 255, 1),   \
                       50000L,     \
                       "Mix");

/**
*   Init buttons array
*/
# define INIT_BTNS()        \
    btns[0] = init_btn(6, 9, A3, true, "Power");

struct s_pot pots[POT_CNT]; /**<pots array */
struct s_btn btns[BTN_CNT]; /**<buttons array */

DEF_STRUCT_SET_STATE_MSG(POT_CNT, BTN_CNT)
DEF_STRUCT_SET_INFO_REQ(POT_CNT, BTN_CNT)
DEF_STRUCT_GET_STATE_REQ(POT_CNT, BTN_CNT)

#endif /* JOYO_JF_33 */
