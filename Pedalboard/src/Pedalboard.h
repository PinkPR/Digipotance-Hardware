#ifndef PEDALBOARD_H
# define PEDALBOARD_H

# define PRESET_NAME_LEN    17
# define PRESET_COUNT       4

struct preset_name
{
    char name[PRESET_NAME_LEN];
} __attribute__((__packed__));

struct set_bank_msg
{
    char bank_id;
    struct preset_name[PRESET_COUNT];
} __attribute__((__packed__));

struct req_preset_msg
{
    char req_id;
    char linefeed;
    char preset_nb;
} __attribute__((__packed__));

#endif /* PEDALBOARD_H */
