#include "parser.h"
#include <stdio.h>
#include <string.h>

//#define STACK_DEPTH 16

char *identify(char *pos, int *button) {

    /* identify the pos string and return xinput bit-mask */
    if      (strncmp(pos, "DPAD_UP", 7) == 0)          { pos+=7; *button = XINPUT_GAMEPAD_DPAD_UP; }
    else if (strncmp(pos, "DPAD_DOWN", 9) == 0)        { pos+=9; *button = XINPUT_GAMEPAD_DPAD_DOWN; }
    else if (strncmp(pos, "DPAD_LEFT", 9) == 0)        { pos+=9; *button = XINPUT_GAMEPAD_DPAD_LEFT; }
    else if (strncmp(pos, "DPAD_RIGHT", 10) == 0)      { pos+=10; *button = XINPUT_GAMEPAD_DPAD_RIGHT; }
    else if (strncmp(pos, "START", 5) == 0)            { pos+=5; *button = XINPUT_GAMEPAD_START; }
    else if (strncmp(pos, "BACK", 4) == 0)             { pos+=4; *button = XINPUT_GAMEPAD_BACK; }
    else if (strncmp(pos, "LEFT_THUMB", 10) == 0)      { pos+=10; *button = XINPUT_GAMEPAD_LEFT_THUMB; }
    else if (strncmp(pos, "RIGHT_THUMB", 11) == 0)     { pos+=11; *button = XINPUT_GAMEPAD_RIGHT_THUMB; }
    else if (strncmp(pos, "LEFT_SHOULDER", 13) == 0)   { pos+=13; *button = XINPUT_GAMEPAD_LEFT_SHOULDER; }
    else if (strncmp(pos, "RIGHT_SHOULDER", 14) == 0)  { pos+=14; *button = XINPUT_GAMEPAD_RIGHT_SHOULDER; }
    else if (strncmp(pos, "GUIDE", 5) == 0)            { pos+=5; *button = XINPUT_GAMEPAD_GUIDE; }
    else if (strncmp(pos, "A", 1) == 0)                { pos+=1; *button = XINPUT_GAMEPAD_A; }
    else if (strncmp(pos, "B", 1) == 0)                { pos+=1; *button = XINPUT_GAMEPAD_B; }
    else if (strncmp(pos, "X", 1) == 0)                { pos+=1; *button = XINPUT_GAMEPAD_X; }
    else if (strncmp(pos, "Y", 1) == 0)                { pos+=1; *button = XINPUT_GAMEPAD_Y; }
    else *button = XINPUT_GAMEPAD_UNDEFINED;

    /* else return 0 */
    return pos;
}

int left(char *line) {
    fprintf(stderr, "input: %s\n", line);

    /* (start & back) | A => buttons ^ (START | BACK) || buttons & A */
    int sp = 0;
    //char stack[STACK_DEPTH];
    char *pos = line;

    while (*pos) {

        int button;
        if (*pos == '(') { sp++; pos++; }
        else if (*pos == ')') { sp--; pos++; }
        else if (*pos == ' ') pos++;
        else if (*pos == '|') pos++;
        else if (*pos == '&') pos++;
        else if (*pos == '=') return 0;
        else {
            pos = identify(pos, &button);

            fprintf(stderr, "button: 0x%04X\n", button);
            fprintf(stderr, "button: %s\n", XINPUT_BUTTON_TO_STRING(button));
            if (!button) {
                fprintf(stderr, "Syntax Error:\"%s\"\n", pos);
                return 1;
            }
            
        }

    }
    return 0;
}

int parse(char *line) {
    left(line);
    return 0;
}


