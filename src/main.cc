/* 

Xwrap - Xinput (and maybe direct-input later?) gamepad-to-keyboard mapping utility
Thomas Daley
6/27/2017

*/ 

#include "Gamepad.h"
#include "Display.h"
#include "config.h"
#include "registry.h"
#include "parser.h" /* for parser tests */
#include <stdio.h>

int main(int argc, char **argv) {
    
    /* we only want one instance running at any given time */
    HANDLE running;
    char *mutex_name = "Xwrap_singleton_instance_running";
    running = OpenMutex(SYNCHRONIZE, TRUE, mutex_name);
    if (running) {
        /* bring the window to the foreground. */
        HWND hwnd = FindWindow(NULL, WINDOW_TITLE);
        ShowWindow(hwnd, SW_RESTORE);
        SetForegroundWindow(hwnd);
        CloseHandle(hwnd);
        CloseHandle(running);
        return 0;
    }
    running = CreateMutex(NULL, TRUE, mutex_name);

    /* load config file from /User/AppData/Roaming/Xwrap/config.ini */
    XWRAP_CONFIG config;
    load_config(&config);

    /* init gamepad */
    Gamepad gamepad = Gamepad();
    
    /* init display */
    Display display = Display();
    if (display.Initialize(&gamepad, &config))
        return 1;

    /* set the boot flag / update registry value */
    display.SetTrayMenuBootFlag(config.boot_on_startup);
    config.boot_on_startup = registry_launch_on_startup(config.boot_on_startup);

    /* timing control variables */
    unsigned int t1;
    unsigned int t2;
    unsigned int elapsed;
    unsigned int remaining;
    unsigned int interval = 1000 / display.mode.refresh_rate;

    ///////////////////////////////////////////////////////
    /* parser tests */
    fprintf(stderr, "-----Parser tests-----\n");
    

    char test[MAX_PATH];

    /* simple real-world example */
    const char *test_1 = "(START & BACK) = ALT + F4";
    memcpy(test, test_1, strlen(test_1) + 1);
    parse(test);

    /* the empty string */
    const char *test_2 = "";
    memcpy(test, test_2, strlen(test_2) + 1);
    parse(test);

    /* stack integrity test */
    const char *test_3 = "(((X | Y))) = MOUSE_LEFT";
    memcpy(test, test_3, strlen(test_3) + 1);
    parse(test);

    /* syntax error test 'C' doesn't exist */
    const char *test_4 = "(A | C) & B = ESCAPE";
    memcpy(test, test_4, strlen(test_4) + 1);
    parse(test);

    /* no right-hand side test */

    /* ??? test */

    fprintf(stderr, "------End tests-------\n");
    /* end parser tests */
    ///////////////////////////////////////////////////////

    /* main program loop */
    bool done = 0;
    while (!done) {
        
        /* start timing in-between frames */
        t1 = SDL_GetTicks();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            display.ProcessEvents(&event);

            if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_MINIMIZED || 
                    event.window.event == SDL_WINDOWEVENT_CLOSE) 
                    /* background the process */
                    display.Hide();
            }

            if (event.type == SDL_SYSWMEVENT) {
                if (event.syswm.msg->msg.win.msg == WM_USER + 1) {
                    /* left click tray icon -> show window */
                    if (LOWORD(event.syswm.msg->msg.win.lParam == WM_LBUTTONDOWN)) 
                        display.Show();
                    /* right click tray icon -> show menu */
                    if (LOWORD(event.syswm.msg->msg.win.lParam == WM_RBUTTONDOWN)) 
                        display.ShowTrayMenu();
                }

                if (event.syswm.msg->msg.win.msg == WM_COMMAND) {
                    /* close when the user clicks the tray menu quit button */
                    if (LOWORD(event.syswm.msg->msg.win.wParam == TRAYICON_QUIT)) 
                        done = 1; 
                    /* toggle boot on startup (right-click tray icon menu) */
                    if (LOWORD(event.syswm.msg->msg.win.wParam == TRAYICON_BOOT_FLAG)) {
                        config.boot_on_startup = registry_launch_on_startup(!config.boot_on_startup);
                        display.SetTrayMenuBootFlag(config.boot_on_startup);
                        save_config(&config);
                    }
                }
            }

            /* quit if user click on the 'X', 
               unless minimize_on_close is enabled */
            if (event.type == SDL_QUIT && !config.minimize_on_exit) 
                done = 1;
        }

        /* poll the controllers */
        gamepad.Update();
   
        /* rendering */
        display.RenderFrame();

        /* calculate how long to sleep thread 
           based on remaining frame time */
        t2 = SDL_GetTicks();

        elapsed = t2 - t1;
        remaining = interval - elapsed;
        if (elapsed < interval) {
            SDL_Delay(remaining);
            elapsed = interval;
        }
    }

    /* the mutex object is automatically destroyed 
       when the last handle is closed */
    CloseHandle(running);

    fprintf(stderr, "Xwrap terminated successfully.\n");
    return 0;
}
