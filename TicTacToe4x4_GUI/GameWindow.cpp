#include "GameWindow.h"

GameWindow::GameWindow() : renderer(&game) {}

void GameWindow::updateMenuCheckmark(HWND hWnd) const {
    HMENU hMenu = GetMenu(hWnd);
    if (hMenu) {
        const UINT flags = game.isInfiniteMode() ? MF_CHECKED : MF_UNCHECKED;
        CheckMenuItem(hMenu, IDM_GAME_INFINITE, MF_BYCOMMAND | flags);
    }
}

LRESULT GameWindow::handleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_COMMAND:
    {
        const int wmId = LOWORD(wParam);
        switch (wmId) {
        case IDM_GAME_RESTART:
            game.restart();
            InvalidateRect(hWnd, nullptr, TRUE);
            return 0;

        case IDM_GAME_INFINITE:
            game.toggleInfiniteMode();
            updateMenuCheckmark(hWnd);
            InvalidateRect(hWnd, nullptr, TRUE);
            return 0;

        case IDM_GAME_EXIT:
            DestroyWindow(hWnd);
            return 0;

        case IDM_ABOUT:
        {
            DialogBox(GetAppInstance(), MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            return 0;
        }
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        if (hdc) {
            RECT clientRect;
            GetClientRect(hWnd, &clientRect);
            renderer.render(hdc, clientRect);
            EndPaint(hWnd, &ps);
        }
        return 0;
    }

    case WM_LBUTTONDOWN:
    {
        if (game.isGameEnded()) {
            // Check if click is on win dialog
            const int x = LOWORD(lParam);
            const int y = HIWORD(lParam);
            const RECT& winDialogRect = renderer.getWinDialogRect();

            if (x >= winDialogRect.left && x <= winDialogRect.right &&
                y >= winDialogRect.top && y <= winDialogRect.bottom) {
                game.restart();
                InvalidateRect(hWnd, nullptr, TRUE);
            }
        }
        else {
            RECT clientRect;
            GetClientRect(hWnd, &clientRect);

            int row, col;
            renderer.getBoardCellFromPoint(LOWORD(lParam), HIWORD(lParam), clientRect, row, col);

            if (row != -1 && col != -1) {
                if (game.makeMove(row, col)) {
                    InvalidateRect(hWnd, nullptr, TRUE);
                }
            }
        }
        return 0;
    }

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}