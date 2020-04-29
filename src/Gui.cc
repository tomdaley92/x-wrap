#include "Gui.h"
#include "Display.h"
#include "Gamepad.h"
#include "focus.h" /* get_current_focus */
#include "config.h"
#include <stdio.h> /* sprintf */
#include <imgui.h>
#include "imgui_impl_sdl.h" /* must be imported after imgui.h */


Gui::Gui() {
    controller_window_flags = 0;

    show_left_pane = 1;
    show_demo = 0;
    //show_license = 0;
    //show_about = 0;

    memset(show_controller_window, 1, NUM_WINDOWS * sizeof(bool));
}

Gui::~Gui() {
    ImGui_ImplSdl_Shutdown();
}

void Gui::initialize(Display *display, Gamepad *gamepad, XWRAP_CONFIG *config) {
    this->display = display;
    this->gamepad = gamepad;
    this->config = config;

    ImGui_ImplSdl_Init(display->window);
    
    /* disable imgui.ini file saving */
    ImGui::GetIO().IniFilename = NULL;

    /* init window flags */
    controller_window_flags |= ImGuiWindowFlags_NoTitleBar;
    controller_window_flags |= ImGuiWindowFlags_ShowBorders;
    controller_window_flags |= ImGuiWindowFlags_NoResize;
    controller_window_flags |= ImGuiWindowFlags_NoMove;
    controller_window_flags |= ImGuiWindowFlags_NoCollapse; 
    controller_window_flags |= ImGuiWindowFlags_NoScrollbar; 

    left_pane_flags |= ImGuiWindowFlags_NoTitleBar;
    left_pane_flags |= ImGuiWindowFlags_ShowBorders;
    left_pane_flags |= ImGuiWindowFlags_NoResize;
    left_pane_flags |= ImGuiWindowFlags_NoMove;
    //left_pane_flags |= ImGuiWindowFlags_NoCollapse; 
}

void Gui::processEvents(SDL_Event *event) {
    ImGui_ImplSdl_ProcessEvent(event);
}

void Gui::newFrame() {
    ImGui_ImplSdl_NewFrame(display->window);
    leftPane();
    controllerWindows();
}


void Gui::leftPane() {
    if (show_left_pane) {
        bool before;
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(400, 800), ImGuiSetCond_FirstUseEver);
        ImGui::Begin("Main Window", &show_left_pane, left_pane_flags);

        ImGui::TextWrapped("Xwrap beta - \"Gamepad to Keyboard/Mouse Mapper\"");

        if (ImGui::TreeNode("Preferences")) {
            
            before = config->start_in_tray;
            ImGui::Checkbox("Start in system tray", &(config->start_in_tray));
            if (before != config->start_in_tray) save_config(config);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip(
                "When enabled, the application will start minimized in the\n"
                "system tray. You can left-click the tray icon to bring up\n"
                "the window or right-click the tray icon for more options.");

            before = config->minimize_on_exit;
            ImGui::Checkbox("Minimize on exit", &(config->minimize_on_exit));
            if (before != config->minimize_on_exit) save_config(config);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip(
                "When enabled, the application will minimize to the system\n"
                "tray (instead of closing) when you click on 'X'. You may \n"
                "always right-click the tray icon and select 'Quit' to \n"
                "close the application at any time.");
            
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Targets")) {
            // TODO: Associate saved profiles with target applications (window focus method)
            ImGui::TextWrapped("Associate saved profiles with target applications");
            wchar_t filename[MAX_PATH];
            get_current_focus(filename, MAX_PATH);
            ImGui::Separator();
            ImGui::Text("current window focus: ");
            ImGui::TextWrapped("%S", filename);
            ImGui::Separator();
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Profiles")) {
            // TODO: GUI for creating a gamepad->keyboard mapping profile
            ImGui::TextWrapped("Current: %%ProfileName%%");
            ImGui::SmallButton("Load"); 
            ImGui::SameLine(); ImGui::SmallButton("New");
            ImGui::TreePop();
        }

        /* debugging window */
        if (ImGui::TreeNode("Debug")) {
            ImGui::Text("Application average: %.3f ms/frame (%.1f FPS)", 
                1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            if (ImGui::Button("Demo")) show_demo ^= 1;

            for (int i = 0; i < NUM_WINDOWS; i++) {
                char title[12] = "X-input (?)";
                title[9] = 0x30 + i;
                if (ImGui::Button(title)) show_controller_window[i] ^= 1;
            }
            ImGui::TreePop();

            
            /* testing gamepad multiple button pressing */
            short button_macro = (XINPUT_GAMEPAD_START | XINPUT_GAMEPAD_BACK);
            if ((gamepad->buttons(0) & button_macro) == button_macro) {
            //if (gamepad->Buttons(0) & button_macro) {
                ImGui::TextWrapped("[START + BACK]");
            }
  
        }
        ImGui::End();
    }

    /* show ImGui test window (Demo) */
    if (show_demo) {
        ImGui::SetNextWindowPos(ImVec2(120, 60), ImGuiSetCond_FirstUseEver);
        ImGui::ShowTestWindow(&show_demo);
    }
}

void Gui::controllerWindows() {
    for (int i = 0; i < NUM_WINDOWS; i++) {
        if(show_controller_window[i]) {
            /* set window size and title */
            ImGui::SetNextWindowPos(ImVec2(400, (200 * (float)i)), ImGuiSetCond_FirstUseEver);
            ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiSetCond_FirstUseEver);
            char title[12] = "X-input (?)";
            title[9] = 0x30 + i;
            ImGui::Begin(title, &show_controller_window[i], controller_window_flags);
            ImGui::Text(title);
            
            /* fill the window with visual data */
            if (!gamepad->connected(i)) {
                ImGui::Text("Disconnected!");
            } else {
                char data[32];

                sprintf_s(data, 32, "%d (%.0f%%)", gamepad->leftTrigger(i), TRIGGER_PERCENT(gamepad->leftTrigger(i)));
                ImGui::ProgressBar( TRIGGER_SIGMOID(gamepad->leftTrigger(i)), ImVec2(0.0f,0.0f), data);
                ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
                ImGui::Text("LEFT TRIGGER");
                memset(data, '\0', sizeof(char) * 32);
                
                sprintf_s(data, 32, "%d (%.0f%%)", gamepad->rightTrigger(i), TRIGGER_PERCENT(gamepad->rightTrigger(i)));
                ImGui::ProgressBar( TRIGGER_SIGMOID(gamepad->rightTrigger(i)), ImVec2(0.0f,0.0f), data);
                ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
                ImGui::Text("RIGHT TRIGGER");
                memset(data, '\0', sizeof(char) * 32);

                //sprintf_s(data, 32, "%d (%.0f%%)", gamepad->ThumbLX(i), JOYSTICK_PERCENT(gamepad->ThumbLX(i)));
                sprintf_s(data, 32, "%d", gamepad->thumbLX(i));
                ImGui::ProgressBar( JOYSTICK_SIGMOID(gamepad->thumbLX(i)), ImVec2(0.0f,0.0f), data);
                ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
                ImGui::Text("LEFT STICK (X)");
                memset(data, '\0', sizeof(char) * 32);
                
                //sprintf_s(data, 32, "%d (%.0f%%)", gamepad->ThumbLY(i), JOYSTICK_PERCENT(gamepad->ThumbLY(i))); 
                sprintf_s(data, 32, "%d", gamepad->thumbLY(i));              
                ImGui::ProgressBar( JOYSTICK_SIGMOID(gamepad->thumbLY(i)), ImVec2(0.0f,0.0f), data);
                ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
                ImGui::Text("LEFT STICK (Y)");
                memset(data, '\0', sizeof(char) * 32);
                
                //sprintf_s(data, 32, "%d (%.0f%%)", gamepad->ThumbRX(i), JOYSTICK_PERCENT(gamepad->ThumbRX(i)));
                sprintf_s(data, 32, "%d", gamepad->thumbRX(i));
                ImGui::ProgressBar( JOYSTICK_SIGMOID(gamepad->thumbRX(i)), ImVec2(0.0f,0.0f), data);
                ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
                ImGui::Text("RIGHT STICK (X)");
                memset(data, '\0', sizeof(char) * 32);
                
                //sprintf_s(data, 32, "%d (%.0f%%)", gamepad->ThumbRY(i), JOYSTICK_PERCENT(gamepad->ThumbRY(i)));
                sprintf_s(data, 32, "%d", gamepad->thumbRY(i));
                ImGui::ProgressBar( JOYSTICK_SIGMOID(gamepad->thumbRY(i)), ImVec2(0.0f,0.0f), data);
                ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
                ImGui::Text("RIGHT STICK (Y)");
                memset(data, '\0', sizeof(char) * 32);

                if (!gamepad->buttons(i)) {
                    ImGui::Text("Press a button on the gamepad!");
                } else {
                    ImGui::BeginGroup();
                    ImGui::BeginGroup();
                    if (gamepad->buttons(i) & XINPUT_GAMEPAD_DPAD_UP) { ImGui::SmallButton("DPAD UP"); ImGui::SameLine(); }
                    if (gamepad->buttons(i) & XINPUT_GAMEPAD_DPAD_DOWN) { ImGui::SmallButton("DPAD DOWN"); ImGui::SameLine(); }
                    if (gamepad->buttons(i) & XINPUT_GAMEPAD_DPAD_LEFT) { ImGui::SmallButton("DPAD LEFT"); ImGui::SameLine(); }
                    if (gamepad->buttons(i) & XINPUT_GAMEPAD_DPAD_RIGHT) { ImGui::SmallButton("DPAD RIGHT"); ImGui::SameLine(); }
                    if (gamepad->buttons(i) & XINPUT_GAMEPAD_BACK) { ImGui::SmallButton("BACK"); ImGui::SameLine(); }
                    if (gamepad->buttons(i) & XINPUT_GAMEPAD_GUIDE) { ImGui::SmallButton("GUIDE"); ImGui::SameLine(); }
                    if (gamepad->buttons(i) & XINPUT_GAMEPAD_START) { ImGui::SmallButton("START"); ImGui::SameLine(); }
                    ImGui::EndGroup();
                    ImGui::BeginGroup();
                    if (gamepad->buttons(i) & XINPUT_GAMEPAD_LEFT_THUMB) { ImGui::SmallButton("LEFT THUMB"); ImGui::SameLine(); }
                    if (gamepad->buttons(i) & XINPUT_GAMEPAD_RIGHT_THUMB) { ImGui::SmallButton("RIGHT THUMB"); ImGui::SameLine(); }
                    if (gamepad->buttons(i) & XINPUT_GAMEPAD_LEFT_SHOULDER) { ImGui::SmallButton("LEFT SHOULDER"); ImGui::SameLine(); }
                    if (gamepad->buttons(i) & XINPUT_GAMEPAD_RIGHT_SHOULDER) { ImGui::SmallButton("RIGHT SHOULDER"); ImGui::SameLine(); }
                    if (gamepad->buttons(i) & XINPUT_GAMEPAD_A) { ImGui::SmallButton("A"); ImGui::SameLine(); }
                    if (gamepad->buttons(i) & XINPUT_GAMEPAD_B) { ImGui::SmallButton("B"); ImGui::SameLine(); }
                    if (gamepad->buttons(i) & XINPUT_GAMEPAD_X) { ImGui::SmallButton("X"); ImGui::SameLine(); }
                    if (gamepad->buttons(i) & XINPUT_GAMEPAD_Y) { ImGui::SmallButton("Y"); ImGui::SameLine(); }
                    ImGui::EndGroup();
                    ImGui::EndGroup();
                }
            }
            ImGui::End();
        }
    }
}

void Gui::render() {
    ImGui::Render();
}