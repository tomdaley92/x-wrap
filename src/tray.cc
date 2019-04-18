#include "tray.h"
#include <windows.h>
#include <Shellapi.h> /* Shell_NotifyIcon, nid */
#include <stdio.h> /* fprintf */

void add_tray_icon(HWND hwnd, HICON hicon, char *hover_tip) {
    /* tray icon data */
    NOTIFYICONDATA nid;
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = TRAYICON_ID;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_SHOWTIP;
    nid.hIcon = hicon;
    nid.uCallbackMessage = TRAYICON_CALLBACKID;
    nid.uVersion = NOTIFYICON_VERSION_4;
    memcpy (nid.szTip, hover_tip, strlen(hover_tip));

    /* add the tray icon -
       At startup of your computer, Shell_NotifyIcon may have some difficulties to succeed. 
       So, when not successful, don't assume that it will not work at all - just try again */
    for (;;) {
        if (Shell_NotifyIcon(NIM_ADD, &nid)) break;
        else fprintf(stderr, "Error: failed to add notification icon.\nRetrying...\n"); 
    }
}

void remove_tray_icon(HWND hwnd) {
    /* we only need the tray icon ID to remove it */
    NOTIFYICONDATA nid;
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = TRAYICON_ID;
    
    /* remove the tray icon, if this fails, the OS will delete the icon on mouseover event */
    if (!Shell_NotifyIcon(NIM_DELETE, &nid)) {
        fprintf(stderr, "Error: failed to delete notification icon.\n");
    }
}

void show_tray_menu(HWND hwnd, HMENU hmenuTrackPopup) { 
    /* find out where the user clicked */
    POINT pt;
    GetCursorPos(&pt);

    /* so the menu disappears 
       if the user clicks off it */
    SetForegroundWindow(hwnd);

    /* display the shortcut menu, 
       track the right mouse button */ 
    TrackPopupMenu(hmenuTrackPopup, 
        TPM_LEFTALIGN | TPM_RIGHTBUTTON, 
        pt.x, pt.y, 0, hwnd, NULL);
}

void set_tray_menu_flag(HMENU hmenu, bool enable, short menuID) {
    /* check/uncheck the box */
    if (enable) CheckMenuItem(hmenu, menuID, MF_BYCOMMAND | MF_CHECKED);
    else CheckMenuItem(hmenu, menuID, MF_BYCOMMAND | MF_UNCHECKED);
}
