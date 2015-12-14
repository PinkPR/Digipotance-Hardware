/**
*   Defines the s_name builder function
*/
#define DEF_MAKE_S_NAME     \
    struct s_name               \
    get_s_name(char* str)       \
    {                           \
        struct s_name name;     \
        memset(&name, 0, sizeof (struct s_name));   \
                                                            \
        for (int i = 0; str[i] && i < NAME_MAX_SIZE; i++)   \
            name.name[i] = str[i];                          \
                                                            \
        return name;            \
    }

/**
*   Defines the s_req_info_msg builder function
*/
#define DEF_MAKE_REQ_INFO_MSG(p_cnt, b_cnt)   \
    struct s_req_info_msg                       \
    make_req_info_msg(struct s_pot *pots,       \
                      struct s_btn *btns)       \
    {                                           \
        struct s_req_info_msg msg;              \
        memset(&msg, 0, sizeof (struct s_req_info_msg));    \
                                                \
        msg.header = 'I';                       \
        msg.linefeed1 = LF;                     \
        msg.linefeed2 = LF;                     \
        msg.linefeed3 = LF;                     \
                                                \
        msg.pot_cnt = p_cnt;                    \
                                                \
        for (int i = 0; i < p_cnt; i++)         \
            msg.pot_names[i] =                  \
                get_s_name(pots[i].name);       \
                                                \
        msg.btn_cnt = b_cnt;                    \
                                                \
        for (int i = 0; i < b_cnt; i++)         \
            msg.btn_names[i] =                  \
                get_s_name(btns[i].name);       \
                                                \
        return msg;                             \
    }

/**
*   Defines the s_req_state_msg builder function
*/
#define DEF_MAKE_GET_STATE_REQ(p_cnt, b_cnt)  \
    struct s_req_state_msg                      \
    make_req_state_msg(char *pot_values,        \
                       char *btn_values)        \
    {                                           \
        struct s_req_state_msg msg;             \
        memset(&msg, 0, sizeof (struct s_req_state_msg));   \
                                                \
        msg.header = 'P';                       \
        msg.linefeed1 = LF;                     \
        msg.linefeed2 = LF;                     \
        msg.linefeed3 = LF;                     \
                                                \
        for (int i = 0; i < p_cnt; i++)         \
            msg.pot_values[i] = pot_values[i];  \
                                                \
        for (int i = 0; i < b_cnt; i++)         \
            msg.btn_values[i] = btn_values[i];  \
                                                \
        return msg;                             \
    }
