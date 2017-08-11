#include "Display.h"
#include "Gui.h"
#include "Gamepad.h"
#include "config.h"
#include <stdio.h> /* fprintf */

Display::Display(){
    gui = Gui();

    WINDOW_WIDTH = WIDTH;
    WINDOW_HEIGHT = HEIGHT;

    /* set rendering colors */
    background_color[0] = (float) DEFAULT_BACKGROUND_R / (float) 0xFF;
    background_color[1] = (float) DEFAULT_BACKGROUND_G / (float) 0xFF;
    background_color[2] = (float) DEFAULT_BACKGROUND_B / (float) 0xFF;
}

Display::~Display(){
    /* remove the tray icon before destroying the window handle! 
       Otherwise there will be a race condition and this may fail */
    remove_tray_icon(hwnd);

    /* clean-up, shutdown SDL */
    SDL_GL_DeleteContext(glcontext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    CloseHandle(hwnd);
    CloseHandle(hinst);
    CloseHandle(hicon);
    DestroyMenu(hmenu);
}
    

int Display::Initialize(Gamepad *gamepad, XWRAP_CONFIG *config) {

    /* set up SDL - we don't need audio */
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_EVENTS) != 0) {
        fprintf(stderr, "Error: %s\n", SDL_GetError());
        return 1;
    }

    /* set up window */
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    
    unsigned int window_flags = SDL_WINDOW_OPENGL;
    if (config->start_in_tray) window_flags |= SDL_WINDOW_HIDDEN; 
        
    /* create window */    
    window = SDL_CreateWindow(WINDOW_TITLE, 
                              SDL_WINDOWPOS_CENTERED, 
                              SDL_WINDOWPOS_CENTERED, 
                              WINDOW_WIDTH, 
                              WINDOW_HEIGHT, 
                              window_flags);
    if (!window) {
        fprintf(stderr, "Error: %s\n", SDL_GetError());
        return 1;
    }

    /* Get the current display mode info */
    int should_be_zero = SDL_GetCurrentDisplayMode(0, &mode);
    if(should_be_zero != 0) {
        fprintf(stderr, "Could not get display mode for video display: %s\n", 
            SDL_GetError());
        return 1;
    } else {
        fprintf(stderr, "Current display mode is %dx%dpx @ %dhz.\n", 
            mode.w, mode.h, mode.refresh_rate);
    }

    /* init info structure with SDL version info */
    SDL_VERSION(&info.version);

    /* enable system-specific events (used for system tray icon) */
    SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);

    /* get window info */
    if (!SDL_GetWindowWMInfo(window, &info)) {
        fprintf(stderr, "Error: %s\n", SDL_GetError());
        return 1;
    }

    /* set windows specific handles */
    hwnd = info.info.win.window;
    hinst = GetModuleHandle(NULL);

    /* load the menu resource */
    if (!(hmenu = LoadMenu(hinst, "SYSTEMTRAY_MENU"))) {
        fprintf(stderr, "Error: LoadMenu() failed\n");
        return 1;
    }

    /* handle to the first shortcut menu */
    if (!(hmenuTrackPopup = GetSubMenu(hmenu, 0))) {
        fprintf(stderr, "Error: GetSubMenu() failed\n");
        return 1;
    }

    /* load the tray icon */
    hicon = LoadIcon(hinst, MAKEINTRESOURCE(XWRAP_ICON));
    add_tray_icon(info.info.win.window, hicon, "Xwrap");

    /* create OpenGL context */
    SDL_GLContext glcontext = SDL_GL_CreateContext(window);

    /* disable vsync */
    SDL_GL_SetSwapInterval(0);

    /* setup ImGui binding */
    gui.Initialize(this, gamepad, config);

    return 0;
}

void Display::Resize(int x, int y) {
    /* get the current window size */
    WINDOW_WIDTH = x;
    WINDOW_HEIGHT = y;
}

void Display::Hide() {
    /* this also "backgrounds" the process */
    SDL_MinimizeWindow(window);
    SDL_HideWindow(window);
}

void Display::Show() {
    SDL_ShowWindow(window);
    SDL_RestoreWindow(window);
    SDL_RaiseWindow(window);
}

void Display::ShowTrayMenu() {
    show_tray_menu(hwnd, hmenuTrackPopup);
}

void Display::SetTrayMenuBootFlag(bool enable) {
    set_tray_menu_flag(hmenu, enable, TRAYICON_BOOT_FLAG);
}

void Display::ProcessEvents(SDL_Event *event) {
    gui.ProcessEvents(event);
}

void Display::RenderFrame(){
    gui.NewFrame();

    /* set Viewport & Clear the screen (sets the background color) */
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glClearColor(background_color[0], background_color[1], background_color[2], 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    gui.Render();
    SDL_GL_SwapWindow(window);
}