#ifndef TRAY_H
#define TRAY_H

#include <windows.h>

#define TRAYICON_ID 13
#define TRAYICON_CALLBACKID WM_USER + 1

void add_tray_icon(HWND hWnd, HICON hIcon, char *hover_tip);
void remove_tray_icon(HWND hWnd);
void show_tray_icon_menu(HWND hWnd, HMENU hmenuTrackPopup);
void set_tray_menu_flag(HMENU hmenu, bool enable, short int menuID);

#endif
