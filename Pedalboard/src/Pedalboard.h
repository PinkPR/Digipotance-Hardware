#ifndef PEDALBOARD_H
# define PEDALBOARD_H

# include "DigitDisplay.h"

# define PRESET_NAME_LEN    17 /**< Max preset name length including null
    terminating character */
# define PRESET_COUNT       2 /**< Preset count per bank */
# define BANK_COUNT         9 /**< Bank count */

/*
struct preset_name
{
    char name[PRESET_NAME_LEN];
} __attribute__((__packed__));

struct set_bank_msg
{
    char bank_id;
    struct preset_name names[PRESET_COUNT];
} __attribute__((__packed__));

struct s_presets
{
    struct preset_name names[PRESET_COUNT * BANK_COUNT];
} __attribute__((__packed__));

struct req_preset_msg
{
    char preset_nb;
} __attribute__((__packed__));
*/

#endif /* PEDALBOARD_H */
