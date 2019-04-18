#ifndef GAMEPAD_H
#define GAMEPAD_H

//#include <xinput.h> /* Lets try to implement our own, XInputGetKeyStroke() */

/* forward declaration */
typedef struct HINSTANCE__* HINSTANCE;


/* [0.0, 1.0] */
#define TRIGGER_SIGMOID(x) (x / (float) 0xFF) 
#define JOYSTICK_SIGMOID(x) (((x / (float) 0x7FFF) / (float)2.0) + (float)0.5)

/* [0.0, 100.0] */
#define TRIGGER_PERCENT(x) (TRIGGER_SIGMOID(x) * 100.0)
#define JOYSTICK_PERCENT(x) (JOYSTICK_SIGMOID(x) * 100.0)

/* the (default) amount the joysticks are required to exceed 
   from the center in one direction in order to register */
#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE 7849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689

/* triggers must be greater than this value to 
   register as a press */
#define XINPUT_GAMEPAD_TRIGGER_THRESHOLD 30 

/* button bit-masks */
#define XINPUT_GAMEPAD_UNDEFINED 0x0000
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

#define XINPUT_BUTTON_TO_STRING(X)( \
    X == XINPUT_GAMEPAD_DPAD_UP? "DPAD_UP" : \
    X == XINPUT_GAMEPAD_DPAD_DOWN? "DPAD_DOWN" : \
    X == XINPUT_GAMEPAD_DPAD_LEFT? "DPAD_LEFT" : \
    X == XINPUT_GAMEPAD_DPAD_RIGHT? "DPAD_RIGHT" : \
    X == XINPUT_GAMEPAD_START? "START" : \
    X == XINPUT_GAMEPAD_BACK? "BACK" : \
    X == XINPUT_GAMEPAD_LEFT_THUMB? "LEFT_THUMB" : \
    X == XINPUT_GAMEPAD_RIGHT_THUMB? "RIGHT_THUMB" : \
    X == XINPUT_GAMEPAD_LEFT_SHOULDER? "LEFT_SHOULDER" : \
    X == XINPUT_GAMEPAD_RIGHT_SHOULDER? "RIGHT_SHOULDER" : \
    X == XINPUT_GAMEPAD_GUIDE? "GUIDE" : \
    X == XINPUT_GAMEPAD_A? "A" : \
    X == XINPUT_GAMEPAD_B? "B" : \
    X == XINPUT_GAMEPAD_X? "X" : \
    X == XINPUT_GAMEPAD_Y? "Y" : "UNDEFINED" )

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
        void update();

        bool connected(int player);
        unsigned long eventCount(int player);
        unsigned short buttons(int player);

        /* Analog trigger values are from 0-255 
           (on a fully analog supported controllers) 
           e.g. 8bitdo controllers are only on or off (0 or 255) */
        unsigned char leftTrigger(int player);
        unsigned char rightTrigger(int player);

        /* joystick values range from -32768 to 32767 
           with 0 being centered */
        short int thumbLX(int player);
        short int thumbLY(int player);
        short int thumbRX(int player);
        short int thumbRY(int player);  
        
};

#endif
