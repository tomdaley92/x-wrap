/* 

Xwrap - Xinput (and maybe direct-input later?) gamepad-to-keyboard mapping utility
Thomas Daley
6/27/2017

*/ 

#include "Gamepad.h"
#include "Display.h"
#include "config.h"
#include "registry.h"
#include <stdio.h>

int main(int argc, char**argv) {
    
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
    display.SetTrayIconBootFlag(config.boot_on_startup);
    config.boot_on_startup = registry_launch_on_startup(config.boot_on_startup);

    /* timing control variables */
    unsigned int t1;
    unsigned int t2;
    unsigned int elapsed;
    unsigned int remaining;
    unsigned int interval = 1000 / display.mode.refresh_rate;

    /* main program loop */
    bool done = 0;
    while (!done) {
        
        /* start timing in-between frames */
        t1 = SDL_GetTicks();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            display.ProcessEvents(&event);

            if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_MINIMIZED) 
                    display.Hide();
                if (event.window.event == SDL_WINDOWEVENT_CLOSE) 
                    display.Hide();
            }

            if (event.type == SDL_SYSWMEVENT) {
                if (event.syswm.msg->msg.win.msg == WM_USER + 1) {
                    /* left click tray icon -> show window */
                    if (LOWORD(event.syswm.msg->msg.win.lParam == WM_LBUTTONDOWN)) 
                        display.Show();
                    /* right click tray icon -> show menu */
                    if (LOWORD(event.syswm.msg->msg.win.lParam == WM_RBUTTONDOWN)) 
                        display.ShowTrayIconMenu();
                }

                if (event.syswm.msg->msg.win.msg == WM_COMMAND) {
                    /* close when the user right-clicks tray icon quit button */
                    if (LOWORD(event.syswm.msg->msg.win.wParam == TRAYICON_QUIT)) 
                        done = 1; 
                    /* toggle boot on startup (right-click tray icon menu) */
                    if (LOWORD(event.syswm.msg->msg.win.wParam == TRAYICON_BOOT_FLAG)) {
                        config.boot_on_startup = registry_launch_on_startup(!config.boot_on_startup);
                        display.SetTrayIconBootFlag(config.boot_on_startup);
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
