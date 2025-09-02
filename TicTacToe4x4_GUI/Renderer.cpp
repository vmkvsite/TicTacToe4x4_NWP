#include "Renderer.h"
#include "Game.h"
#include "Constants.h"
#include <algorithm>

Renderer::Renderer(Game* gameInstance) : game(gameInstance), winDialogRect{ 0 } {}

void Renderer::render(HDC hdc, const RECT& clientRect) {
    HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
    FillRect(hdc, const_cast<LPRECT>(&clientRect), blackBrush);
    DeleteObject(blackBrush);

    drawBoard(hdc, clientRect);
    drawSymbols(hdc, clientRect);

    if (!game->isGameEnded()) {
        drawPlayerTurn(hdc, clientRect);
    }
    else {
        drawWinDialog(hdc, clientRect);
    }
}

void Renderer::drawBoard(HDC hdc, const RECT& clientRect) const {
    const int boardSize = min(clientRect.right - BOARD_MARGIN, clientRect.bottom - BOARD_TOP_OFFSET - 20);
    const int startX = (clientRect.right - boardSize) / 2;
    const int startY = BOARD_TOP_OFFSET;
    const int cellSize = boardSize / BOARD_SIZE;

    HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
    HBRUSH grayBrush = CreateSolidBrush(RGB(128, 128, 128));

    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            RECT cellRect;
            cellRect.left = startX + col * cellSize;
            cellRect.top = startY + row * cellSize;
            cellRect.right = cellRect.left + cellSize;
            cellRect.bottom = cellRect.top + cellSize;

            HBRUSH currentBrush = ((row + col) % 2 == 0) ? whiteBrush : grayBrush;
            FillRect(hdc, &cellRect, currentBrush);

            HPEN blackPen = CreatePen(PS_SOLID, CELL_BORDER_WIDTH, RGB(0, 0, 0));
            HPEN oldPen = static_cast<HPEN>(SelectObject(hdc, blackPen));

            MoveToEx(hdc, cellRect.left, cellRect.top, nullptr);
            LineTo(hdc, cellRect.right, cellRect.top);
            LineTo(hdc, cellRect.right, cellRect.bottom);
            LineTo(hdc, cellRect.left, cellRect.bottom);
            LineTo(hdc, cellRect.left, cellRect.top);

            SelectObject(hdc, oldPen);
            DeleteObject(blackPen);
        }
    }

    DeleteObject(whiteBrush);
    DeleteObject(grayBrush);
}

void Renderer::drawSymbols(HDC hdc, const RECT& clientRect) const {
    const int boardSize = min(clientRect.right - BOARD_MARGIN, clientRect.bottom - BOARD_TOP_OFFSET - 20);
    const int startX = (clientRect.right - boardSize) / 2;
    const int startY = BOARD_TOP_OFFSET;
    const int cellSize = boardSize / BOARD_SIZE;

    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            const char symbol = game->getSymbolAt(row + 1, col + 1);

            if (symbol != ' ') {
                const int centerX = startX + col * cellSize + cellSize / 2;
                const int centerY = startY + row * cellSize + cellSize / 2;
                const int symbolSize = cellSize / SYMBOL_SIZE_RATIO;

                COLORREF symbolColor;
                if (game->isSymbolExpiring(row + 1, col + 1)) {
                    symbolColor = RGB(128, 0, 128);
                }
                else {
                    symbolColor = (symbol == 'X') ? RGB(255, 0, 0) : RGB(0, 0, 255);
                }

                HPEN symbolPen = CreatePen(PS_SOLID, SYMBOL_PEN_WIDTH, symbolColor);
                HPEN oldPen = static_cast<HPEN>(SelectObject(hdc, symbolPen));

                if (symbol == 'X') {
                    MoveToEx(hdc, centerX - symbolSize, centerY - symbolSize, nullptr);
                    LineTo(hdc, centerX + symbolSize, centerY + symbolSize);
                    MoveToEx(hdc, centerX + symbolSize, centerY - symbolSize, nullptr);
                    LineTo(hdc, centerX - symbolSize, centerY + symbolSize);
                }
                else if (symbol == 'O') {
                    Arc(hdc, centerX - symbolSize, centerY - symbolSize,
                        centerX + symbolSize, centerY + symbolSize, 0, 0, 0, 0);
                }

                SelectObject(hdc, oldPen);
                DeleteObject(symbolPen);
            }
        }
    }
}

void Renderer::drawPlayerTurn(HDC hdc, const RECT& clientRect) const {
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);

    HFONT font = CreateFont(TURN_FONT_SIZE, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
    HFONT oldFont = static_cast<HFONT>(SelectObject(hdc, font));

    wchar_t turnText[50];
    const wchar_t playerSymbol = (game->getCurrentPlayer() == 'X') ? L'X' : L'O';
    swprintf_s(turnText, L"Player %c's turn", playerSymbol);

    const int textWidth = 200;
    const int x = (clientRect.right - textWidth) / 2;
    const int y = TURN_TEXT_Y;

    RECT textRect = { x, y, x + textWidth, y + TURN_TEXT_HEIGHT };
    DrawText(hdc, turnText, -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    SelectObject(hdc, oldFont);
    DeleteObject(font);
}

void Renderer::drawWinDialog(HDC hdc, const RECT& clientRect) {
    winDialogRect.left = (clientRect.right - WIN_DIALOG_WIDTH) / 2;
    winDialogRect.top = (clientRect.bottom - WIN_DIALOG_HEIGHT) / 2;
    winDialogRect.right = winDialogRect.left + WIN_DIALOG_WIDTH;
    winDialogRect.bottom = winDialogRect.top + WIN_DIALOG_HEIGHT;

    HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
    FillRect(hdc, &winDialogRect, whiteBrush);
    DeleteObject(whiteBrush);

    HPEN blackPen = CreatePen(PS_SOLID, CELL_BORDER_WIDTH, RGB(0, 0, 0));
    HPEN oldPen = static_cast<HPEN>(SelectObject(hdc, blackPen));
    Rectangle(hdc, winDialogRect.left, winDialogRect.top, winDialogRect.right, winDialogRect.bottom);
    SelectObject(hdc, oldPen);
    DeleteObject(blackPen);

    SetTextColor(hdc, RGB(0, 128, 0));
    SetBkMode(hdc, TRANSPARENT);

    HFONT font = CreateFont(WIN_FONT_SIZE, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
    HFONT oldFont = static_cast<HFONT>(SelectObject(hdc, font));

    wchar_t winText[50];
    const char winner = game->getWinner();
    if (winner != ' ') {
        const wchar_t winnerSymbol = (winner == 'X') ? L'X' : L'O';
        swprintf_s(winText, L"Player %c wins!", winnerSymbol);
    }
    else {
        wcscpy_s(winText, L"It's a draw!");
    }

    DrawText(hdc, winText, -1, &winDialogRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    SelectObject(hdc, oldFont);
    DeleteObject(font);
}

void Renderer::getBoardCellFromPoint(int x, int y, const RECT& clientRect, int& row, int& col) const {
    const int boardSize = min(clientRect.right - BOARD_MARGIN, clientRect.bottom - BOARD_TOP_OFFSET - 20);
    const int startX = (clientRect.right - boardSize) / 2;
    const int startY = BOARD_TOP_OFFSET;
    const int cellSize = boardSize / BOARD_SIZE;

    if (x >= startX && x < startX + boardSize && y >= startY && y < startY + boardSize) {
        col = (x - startX) / cellSize;
        row = (y - startY) / cellSize;

        row++;
        col++;
    }
    else {
        row = -1;
        col = -1;
    }
}