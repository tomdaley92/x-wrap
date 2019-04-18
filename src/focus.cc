#include "focus.h"
#include <windows.h>
#include <Psapi.h> /* GetModuleFileNameExW */

void get_current_focus(wchar_t *filename, int size) {
    /* get focused-window exe path */
    unsigned long pid; 
    HWND pHandle = GetForegroundWindow();
    if (pHandle) GetWindowThreadProcessId(pHandle, &pid);

    HANDLE processHandle = NULL;
    processHandle = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);

    if (processHandle) {
        GetModuleFileNameExW(processHandle, NULL, filename, size);
    }

    CloseHandle(pHandle);
    CloseHandle(processHandle);
}
