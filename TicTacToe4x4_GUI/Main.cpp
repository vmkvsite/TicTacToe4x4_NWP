#include <windows.h>
#include "resource.h"
#include "Game.h"
#include "Renderer.h"
#include "Constants.h"

namespace {
    const int MAX_LOADSTRING = 100;

    HINSTANCE hInst;
    TCHAR szTitle[MAX_LOADSTRING];
    TCHAR szWindowClass[MAX_LOADSTRING];

    Game game;
    Renderer renderer(&game);

    ATOM RegisterWindowClass(HINSTANCE hInstance);
    BOOL InitInstance(HINSTANCE, int);
    LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
    INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
    void UpdateMenuCheckmark(HWND hWnd);
}

HINSTANCE GetAppInstance() {
    return hInst;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE,
    _In_ LPWSTR,
    _In_ int nCmdShow)
{
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_TICTACTOE4X4GUI, szWindowClass, MAX_LOADSTRING);
    RegisterWindowClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow)) {
        return FALSE;
    }

    const HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TICTACTOE4X4GUI));
    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return static_cast<int>(msg.wParam);
}

namespace {
    ATOM RegisterWindowClass(HINSTANCE hInstance) {
        WNDCLASSEX wcex{};
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WndProc;
        wcex.hInstance = hInstance;
        wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TICTACTOE4X4GUI));
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
        wcex.lpszMenuName = MAKEINTRESOURCE(IDC_TICTACTOE4X4GUI);
        wcex.lpszClassName = szWindowClass;
        wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));

        return RegisterClassEx(&wcex);
    }

    BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
        hInst = hInstance;

        HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, 0, 600, 700, nullptr, nullptr, hInstance, nullptr);

        if (!hWnd) {
            MessageBox(nullptr, TEXT("Failed to create main window"), TEXT("Error"), MB_OK | MB_ICONERROR);
            return FALSE;
        }

        UpdateMenuCheckmark(hWnd);
        ShowWindow(hWnd, nCmdShow);
        UpdateWindow(hWnd);

        return TRUE;
    }

    void UpdateMenuCheckmark(HWND hWnd) {
        HMENU hMenu = GetMenu(hWnd);
        if (hMenu) {
            const UINT flags = game.isInfiniteMode() ? MF_CHECKED : MF_UNCHECKED;
            CheckMenuItem(hMenu, IDM_GAME_INFINITE, MF_BYCOMMAND | flags);
        }
    }

    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
        switch (message) {
        case WM_COMMAND: {
            const int wmId = LOWORD(wParam);
            switch (wmId) {
            case IDM_GAME_RESTART:
                game.restart();
                InvalidateRect(hWnd, nullptr, FALSE);
                break;
            case IDM_GAME_INFINITE:
                game.toggleInfiniteMode();
                UpdateMenuCheckmark(hWnd);
                InvalidateRect(hWnd, nullptr, FALSE);
                break;
            case IDM_GAME_RESET_SCORE:
                game.resetScore();
                InvalidateRect(hWnd, nullptr, FALSE);
                break;
            case IDM_GAME_EXIT:
                DestroyWindow(hWnd);
                break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
            break;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            RECT clientRect;
            GetClientRect(hWnd, &clientRect);
            renderer.render(hdc, clientRect);
            EndPaint(hWnd, &ps);
            break;
        }
        case WM_LBUTTONDOWN: {
            if (game.isGameEnded()) {
                const int x = LOWORD(lParam);
                const int y = HIWORD(lParam);
                const RECT& winDialogRect = renderer.getWinDialogRect();

                if (x >= winDialogRect.left && x <= winDialogRect.right &&
                    y >= winDialogRect.top && y <= winDialogRect.bottom) {
                    game.restart();
                    InvalidateRect(hWnd, nullptr, FALSE);
                }
            }
            else {
                RECT clientRect;
                GetClientRect(hWnd, &clientRect);
                int row, col;
                renderer.getBoardCellFromPoint(LOWORD(lParam), HIWORD(lParam), clientRect, row, col);

                if (row != -1 && col != -1 && game.makeMove(row, col)) {
                    InvalidateRect(hWnd, nullptr, FALSE);
                }
            }
            break;
        }
        case WM_ERASEBKGND:
            return 1;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        return 0;
    }

    INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM) {
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
}