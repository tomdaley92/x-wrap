#include "registry.h"
#include <windows.h>
#include <stdio.h> /* fprintf */

bool registry_launch_on_startup(bool enable) {

    char progPath[MAX_PATH];
    unsigned long int disp;
    HKEY hkey;
    LONG createStatus; 
    LONG setStatus;
    
    /* open the registry key we want to edit */
    createStatus = RegCreateKeyEx(
        HKEY_CURRENT_USER, 
        "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 
        0, 
        NULL,
        REG_OPTION_NON_VOLATILE, 
        KEY_WRITE | KEY_WOW64_32KEY , 
        NULL, 
        &hkey, 
        &disp ); 

    if (createStatus) {
        fprintf(stderr, "Error: Failed to open Registry key, create status: %d\n", 
            createStatus);
        return 0;
    }

    if (enable) {

        if (disp == REG_OPENED_EXISTING_KEY || 
            disp == REG_CREATED_NEW_KEY )  {

            GetModuleFileName(NULL, progPath, MAX_PATH);
            
            /* update the registry value */
            setStatus = RegSetValueEx(
                hkey, 
                "Xwrap Gamepad Mapper", 
                0, 
                REG_SZ, 
                (BYTE *) progPath, 
                strlen(progPath)+1 );

            if (setStatus) {
                fprintf(stderr, "Error: Failed to update registry value.\n");
                RegCloseKey(hkey);
                return 0;
            }
        }

    } else {
        /* remove the value from the key */
        RegDeleteValue(hkey, "Xwrap Gamepad Mapper");
    }

    /* cleanup */
    RegCloseKey(hkey);

    fprintf(stderr, "Updated registry.\n");
    return enable;
}
