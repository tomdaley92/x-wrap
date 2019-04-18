#ifndef MACRO_H
#define MACRO_H

/*

condition with possible nested conditions

- write a class that can parse a mapping-defition string into an easy to test condition.
- ideally a 1 liner

& operator -> button_butmask & buttons == button_butmask
| operator -> button_butmask & buttons
! operator -> button_butmask & buttons != button_butmask

*/

/* forward declarations */
typedef struct CONDITION CONDITION;

typedef struct OPERAND {
    unsigned short button_mask;
    CONDITION *condition; // pointer to another condition (if null, then the button is valid)
} OPERAND;

typedef struct CONDITION {
    char op; // operator is a c++ keyword unfortunatley
    OPERAND operand;
} CONDITION;

class Macro { // TODO
    
    private:
        
    public:

        Macro();
        ~Macro();
};

#endif
