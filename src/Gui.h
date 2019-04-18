#ifndef GUI_H
#define GUI_H

#define MENU_HEIGHT 19
#define NUM_WINDOWS 4

/* forward declarations */
class Display;
class Gamepad;
struct XWRAP_CONFIG;
typedef union SDL_Event SDL_Event;
typedef int ImGuiWindowFlags;

class Gui {

    private:
        /* pointers to display, gamepad, and config */
        Display *display;
        Gamepad *gamepad;
        XWRAP_CONFIG *config;

        ImGuiWindowFlags controller_window_flags;
        ImGuiWindowFlags left_pane_flags;

        /* window toggles */
        bool show_left_pane;
        bool show_demo;
        //bool show_license;
        //bool show_about;

        /* flags for controller windows */
        bool show_controller_window[NUM_WINDOWS];

        void leftPane();
        void controllerWindows();

    public:

        Gui();
        ~Gui();

        void initialize(Display *display, 
                        Gamepad *gamepad, 
                        XWRAP_CONFIG *config);

        void processEvents(SDL_Event *event);
        void newFrame();
        void render();

        
};

#endif
