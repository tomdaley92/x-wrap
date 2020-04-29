#ifndef DISPLAY_H
#define DISPLAY_H

#include "Gui.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include "resource.h" /* window icon, trayicon menu */
#include "tray.h" /* TRAYICON_ID, and tray functions */

#define WINDOW_TITLE "Xwrap (BETA)"

#define WIDTH 800
#define HEIGHT 800

/* default render colors */
#define DEFAULT_BACKGROUND_R 114
#define DEFAULT_BACKGROUND_G 144
#define DEFAULT_BACKGROUND_B 154

/* forward declaration */
class Gamepad;
struct XWRAP_CONFIG;

class Display {

    private:
        SDL_GLContext glcontext;
        
        int WINDOW_WIDTH;
        int WINDOW_HEIGHT;

        Gui gui;

        /* window/traymenu/icon handles */
        HWND hwnd;
        HMENU hmenu;
        HMENU hmenuTrackPopup;
        HINSTANCE hinst;
        HICON hicon;

    public:

        /* refresh rate in Hz */
        SDL_DisplayMode mode;

        /* RGB colors for foreground/background */
        float background_color[3];

        SDL_Window *window;
        SDL_SysWMinfo info;

        Display();
        ~Display();

        int initialize(Gamepad *gamepad, XWRAP_CONFIG *config);

        void resize(int x, int y);
        void hide();
        void show();
        void showTrayMenu();
        void setTrayMenuBootFlag(bool enable);
        void processEvents(SDL_Event *event);
        void renderFrame();
};

#endif
