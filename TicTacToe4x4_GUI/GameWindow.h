#pragma once

#include <windows.h>
#include "Game.h"
#include "Renderer.h"
#include "Constants.h"

// External functions available to other modules
HINSTANCE GetAppInstance();
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

class GameWindow {
private:
    Game game;
    Renderer renderer;

    void updateMenuCheckmark(HWND hWnd) const;

public:
    GameWindow();
    ~GameWindow() = default;

    LRESULT handleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};