#ifndef CONFIG_H
#define CONFIG_H

#define CONFIG_NUM_REQUIRED_FIELDS 3

typedef struct XWRAP_CONFIG {
    bool start_on_login;
    bool start_in_tray;
    bool minimize_on_exit;
} XWRAP_CONFIG;

int load_config(XWRAP_CONFIG *config);
int save_config(XWRAP_CONFIG *config);

#endif
