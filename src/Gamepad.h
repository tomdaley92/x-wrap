#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <windows.h>
//#include <xinput.h> /* Lets try to implement our own, XInputGetKeyStroke() */

/* [0.0, 1.0] */
#define TRIGGER_TO_FLOAT(x) (x / (float) 0xFF) 
#define JOYSTICK_TO_FLOAT(x) (((x / (float) 0x7FFF) / (float)2.0) + (float)0.5) 

/* [0.0, 100.0] */
#define TRIGGER_PERCENT(x) (TRIGGER_TO_FLOAT(x) * 100.0)
#define JOYSTICK_PERCENT(x) (JOYSTICK_TO_FLOAT(x) * 100.0)

/* the (default) amount the joysticks are required to exceed 
   from the center in one direction in order to register */
#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE 7849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689

/* triggers must be greater than this value to 
   register as a press */
#define XINPUT_GAMEPAD_TRIGGER_THRESHOLD 30 

/* button bit-masks */
#define XINPUT_GAMEPAD_DPAD_UP 0x0001
#define XINPUT_GAMEPAD_DPAD_DOWN 0x0002
#define XINPUT_GAMEPAD_DPAD_LEFT 0x0004
#define XINPUT_GAMEPAD_DPAD_RIGHT 0x0008
#define XINPUT_GAMEPAD_START 0x0010
#define XINPUT_GAMEPAD_BACK 0x0020
#define XINPUT_GAMEPAD_LEFT_THUMB 0x0040
#define XINPUT_GAMEPAD_RIGHT_THUMB 0x0080
#define XINPUT_GAMEPAD_LEFT_SHOULDER 0x0100
#define XINPUT_GAMEPAD_RIGHT_SHOULDER 0x0200
#define XINPUT_GAMEPAD_GUIDE 0x0400
#define XINPUT_GAMEPAD_A 0x1000
#define XINPUT_GAMEPAD_B 0x2000
#define XINPUT_GAMEPAD_X 0x4000
#define XINPUT_GAMEPAD_Y 0x8000

#define MAX_PLAYERS 4
#define VALID_PLAYER(x) (x >= 0 && x < MAX_PLAYERS)

class Gamepad {
    
    private:

        typedef struct {
            unsigned long   eventCount;
            unsigned short  buttons;
            unsigned char   leftTrigger;
            unsigned char   rightTrigger;  
            short int       thumbLX;
            short int       thumbLY;
            short int       thumbRX;
            short int       thumbRY;
        } XINPUT_GAMEPAD;
        
        /* function pointer to (guide compatible) getstate
           returns 0 on success, 1167 on not connected */
        typedef int(__stdcall *XINPUT_GET_STATE) (int, XINPUT_GAMEPAD*);

        XINPUT_GET_STATE XInputGetState;
        HINSTANCE xinput_dll;

        /* create xinput gamepad instance */
        XINPUT_GAMEPAD state[MAX_PLAYERS];
        bool connected_list[MAX_PLAYERS];

    public:

        Gamepad();
        ~Gamepad();

        /* poll controllers and update state */
        void Update();

        bool Connected(int player);
        unsigned long EventCount(int player);
        unsigned short Buttons(int player);

        /* Analog trigger values are from 0-255 
           (on a fully analog supported controllers) 
           e.g. 8bitdo controllers are only on or off (0 or 255) */
        unsigned char LeftTrigger(int player);
        unsigned char RightTrigger(int player);

        /* joystick values range from -32768 to 32767 
           with 0 being centered */
        short int ThumbLX(int player);
        short int ThumbLY(int player);
        short int ThumbRX(int player);
        short int ThumbRY(int player);  
        
};

#endif
