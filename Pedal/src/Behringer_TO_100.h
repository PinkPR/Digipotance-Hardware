#ifndef BEHRINGER_TO_100
# define BEHRINGER_TO_100

# include "Pedal.h"

# define PEDAL_BRAND    "Behringer"
# define PEDAL_MODEL    "TO 100"
# define POT_CNT        3 /**<Number of digipots on the pedal */
# define BTN_CNT        1 /**<Number of emulated buttons on the pedal */

/**
*   Init pots array
*/
# define INIT_POTS()        \
    pots[0] = init_pot(A0,  \
                       4,   \
                       init_dpot(true, 100000L, 255, 3),    \
                       500000L,     \
                       "Drive");    \
                            \
    pots[1] = init_pot(A1,  \
                       5,   \
                       init_dpot(false, 10000L, 127, 2),     \
                       20000L,  \
                       "Tone");     \
    pots[2] = init_pot(A2,  \
                       3,   \
                       init_dpot(true, 100000L, 255, 1),   \
                       100000L,     \
                       "Volume");

/**
*   Init buttons array
*/
# define INIT_BTNS()        \
    btns[0] = init_btn(6, 9, A3, false, "Power");

struct s_pot pots[POT_CNT]; /**<pots array */
struct s_btn btns[BTN_CNT]; /**<buttons array */

DEF_STRUCT_SET_STATE_MSG(POT_CNT, BTN_CNT)
DEF_STRUCT_SET_INFO_REQ(POT_CNT, BTN_CNT)
DEF_STRUCT_GET_STATE_REQ(POT_CNT, BTN_CNT)

#endif /* BEHRINGER_TO_100 */
