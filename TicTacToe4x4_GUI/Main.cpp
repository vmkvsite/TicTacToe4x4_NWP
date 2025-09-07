#include <windows.h>
#include "resource.h"
#include "Game.h"
#include "Renderer.h"
#include "Constants.h"

class application {
public:
    int run() {
        MSG msg;
        while (GetMessage(&msg, nullptr, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        return static_cast<int>(msg.wParam);
    }
};

class main_window {
private:
    HWND hw;
    Game game;
    Renderer renderer;

public:
    main_window() : hw(nullptr), renderer(&game) {}

    operator HWND() const { return hw; }

    bool create(HWND parent, DWORD style, LPCTSTR caption) {
        WNDCLASSEX wcex{};
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = proc;
        wcex.hInstance = GetModuleHandle(nullptr);
        wcex.hIcon = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_TICTACTOE4X4GUI));
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
        wcex.lpszMenuName = MAKEINTRESOURCE(IDC_TICTACTOE4X4GUI);
        wcex.lpszClassName = L"TicTacToe4x4";
        wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

        RegisterClassEx(&wcex);

        hw = CreateWindow(L"TicTacToe4x4", caption, style,
            CW_USEDEFAULT, 0, 600, 700,
            parent, LoadMenu(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDC_TICTACTOE4X4GUI)),
            GetModuleHandle(nullptr), this);
        return hw != nullptr;
    }

protected:
    void on_command(int id) {
        switch (id) {
        case IDM_GAME_RESTART:
            game.restart();
            InvalidateRect(hw, nullptr, FALSE);
            break;
        case IDM_GAME_INFINITE:
            game.toggleInfiniteMode();
            update_menu_checkmark();
            InvalidateRect(hw, nullptr, FALSE);
            break;
        case IDM_GAME_RESET_SCORE:
            game.resetScore();
            InvalidateRect(hw, nullptr, FALSE);
            break;
        case IDM_GAME_EXIT:
            DestroyWindow(hw);
            break;
        case IDM_ABOUT:
            DialogBox(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDD_ABOUTBOX), hw, about_proc);
            break;
        }
    }

    void on_paint(HDC hdc) {
        RECT clientRect;
        GetClientRect(hw, &clientRect);
        renderer.render(hdc, clientRect);
    }

    void on_left_button_down(int x, int y) {
        if (game.isGameEnded()) {
            const RECT& winDialogRect = renderer.getWinDialogRect();

            if (x >= winDialogRect.left && x <= winDialogRect.right &&
                y >= winDialogRect.top && y <= winDialogRect.bottom) {
                game.restart();
                InvalidateRect(hw, nullptr, FALSE);
            }
        }
        else {
            RECT clientRect;
            GetClientRect(hw, &clientRect);
            int row, col;
            renderer.getBoardCellFromPoint(x, y, clientRect, row, col);

            if (row != -1 && col != -1 && game.makeMove(row, col)) {
                InvalidateRect(hw, nullptr, FALSE);
            }
        }
    }

    void on_destroy() {
        PostQuitMessage(0);
    }

private:
    void update_menu_checkmark() {
        HMENU hMenu = GetMenu(hw);
        if (hMenu) {
            const UINT flags = game.isInfiniteMode() ? MF_CHECKED : MF_UNCHECKED;
            CheckMenuItem(hMenu, IDM_GAME_INFINITE, MF_BYCOMMAND | flags);
        }
    }

    static LRESULT CALLBACK proc(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
        if (msg == WM_CREATE) {
            CREATESTRUCT* pcs = reinterpret_cast<CREATESTRUCT*>(lp);
            main_window* pw = reinterpret_cast<main_window*>(pcs->lpCreateParams);
            SetWindowLongPtr(hw, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pw));
            pw->hw = hw;
            pw->update_menu_checkmark();
            return 0;
        }

        main_window* pw = reinterpret_cast<main_window*>(GetWindowLongPtr(hw, GWLP_USERDATA));
        if (pw) {
            switch (msg) {
            case WM_COMMAND:
                pw->on_command(LOWORD(wp));
                return 0;
            case WM_DESTROY:
                pw->on_destroy();
                return 0;
            case WM_LBUTTONDOWN:
                pw->on_left_button_down(LOWORD(lp), HIWORD(lp));
                return 0;
            case WM_PAINT: {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hw, &ps);
                pw->on_paint(hdc);
                EndPaint(hw, &ps);
                return 0;
            }
            }
        }
        return DefWindowProc(hw, msg, wp, lp);
    }

    static INT_PTR CALLBACK about_proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM) {
        switch (message) {
        case WM_INITDIALOG:
            return TRUE;
        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
                EndDialog(hDlg, LOWORD(wParam));
                return TRUE;
            }
            break;
        }
        return FALSE;
    }
};

HINSTANCE GetAppInstance() {
    return GetModuleHandle(nullptr);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    application app;
    main_window w;

    TCHAR title[100];
    LoadString(hInstance, IDS_APP_TITLE, title, 100);

    w.create(0, WS_OVERLAPPEDWINDOW, title);
    ShowWindow(w, nCmdShow);
    UpdateWindow(w);

    return app.run();
}