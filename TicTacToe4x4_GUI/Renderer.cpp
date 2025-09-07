#include "Renderer.h"
#include "Game.h"
#include "Constants.h"
#include "Resource.h"
#include <algorithm>
#include <string>
#include <format>

std::wstring LoadStringResource(UINT stringId) {
    extern HINSTANCE GetAppInstance();

    wchar_t buffer[256];
    int length = LoadString(GetAppInstance(), stringId, buffer, sizeof(buffer) / sizeof(wchar_t));
    if (length > 0) {
        return std::wstring(buffer, length);
    }
    return L"";
}

std::wstring format_wstring_runtime(const std::wstring& format_str, char playerSymbol) {
    std::wstring result = format_str;
    size_t pos = result.find(L"{}");
    if (pos != std::wstring::npos) {
        result.replace(pos, 2, 1, static_cast<wchar_t>(playerSymbol));
    }
    return result;
}

std::wstring format_wstring_runtime(const std::wstring& format_str, int value) {
    std::wstring result = format_str;
    size_t pos = result.find(L"{}");
    if (pos != std::wstring::npos) {
        result.replace(pos, 2, std::to_wstring(value));
    }
    return result;
}

namespace {
    class GDIObject {
    private:
        HGDIOBJ obj;
    public:
        explicit GDIObject(HGDIOBJ o) : obj(o) {}
        ~GDIObject() { if (obj) DeleteObject(obj); }

        GDIObject(const GDIObject&) = delete;
        GDIObject& operator=(const GDIObject&) = delete;

        operator HGDIOBJ() const { return obj; }

        template<typename T>
        operator T() const { return static_cast<T>(obj); }
    };
}

Renderer::Renderer(Game* gameInstance) : game(gameInstance), winDialogRect{ 0 } {}

void Renderer::render(HDC hdc, const RECT& clientRect) {
    HBRUSH blackBrush = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    FillRect(hdc, const_cast<LPRECT>(&clientRect), blackBrush);

    drawBoard(hdc, clientRect);
    drawSymbols(hdc, clientRect);

    if (!game->isGameEnded()) {
        drawPlayerTurn(hdc, clientRect);
    }
    else {
        drawWinDialog(hdc, clientRect);
    }

    drawScore(hdc, clientRect);
}

void Renderer::calculateBoardMetrics(const RECT& clientRect, int& boardSize, int& startX, int& startY, int& cellSize) const {
    boardSize = std::min(static_cast<int>(clientRect.right - BOARD_MARGIN),
        static_cast<int>(clientRect.bottom - BOARD_TOP_OFFSET - 20));
    startX = (clientRect.right - boardSize) / 2;
    startY = BOARD_TOP_OFFSET;
    cellSize = boardSize / BOARD_SIZE;
}

void Renderer::drawBoard(HDC hdc, const RECT& clientRect) const {
    int boardSize, startX, startY, cellSize;
    calculateBoardMetrics(clientRect, boardSize, startX, startY, cellSize);

    GDIObject whiteBrush(CreateSolidBrush(BOARD_WHITE_COLOR));
    GDIObject grayBrush(CreateSolidBrush(BOARD_GRAY_COLOR));
    GDIObject blackPen(CreatePen(PS_SOLID, CELL_BORDER_WIDTH, BOARD_BLACK_COLOR));

    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            RECT cellRect;
            cellRect.left = startX + col * cellSize;
            cellRect.top = startY + row * cellSize;
            cellRect.right = cellRect.left + cellSize;
            cellRect.bottom = cellRect.top + cellSize;

            HBRUSH currentBrush = ((row + col) % 2 == 0) ? whiteBrush : grayBrush;
            FillRect(hdc, &cellRect, currentBrush);

            HPEN oldPen = static_cast<HPEN>(SelectObject(hdc, blackPen));

            MoveToEx(hdc, cellRect.left, cellRect.top, nullptr);
            LineTo(hdc, cellRect.right, cellRect.top);
            LineTo(hdc, cellRect.right, cellRect.bottom);
            LineTo(hdc, cellRect.left, cellRect.bottom);
            LineTo(hdc, cellRect.left, cellRect.top);

            SelectObject(hdc, oldPen);
        }
    }
}

COLORREF Renderer::getSymbolColor(char symbol, bool isExpiring) const {
    if (isExpiring) return EXPIRING_COLOR;
    return (symbol == 'X') ? PLAYER_X_COLOR : PLAYER_O_COLOR;
}

void Renderer::drawSymbols(HDC hdc, const RECT& clientRect) const {
    int boardSize, startX, startY, cellSize;
    calculateBoardMetrics(clientRect, boardSize, startX, startY, cellSize);

    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            const char symbol = game->getSymbolAt(row + 1, col + 1);

            if (symbol != ' ') {
                const int centerX = startX + col * cellSize + cellSize / 2;
                const int centerY = startY + row * cellSize + cellSize / 2;
                const int symbolSize = cellSize / SYMBOL_SIZE_RATIO;

                const bool isExpiring = game->isSymbolExpiring(row + 1, col + 1);
                const COLORREF symbolColor = getSymbolColor(symbol, isExpiring);

                GDIObject symbolPen(CreatePen(PS_SOLID, SYMBOL_PEN_WIDTH, symbolColor));
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
            }
        }
    }
}

void Renderer::drawPlayerTurn(HDC hdc, const RECT& clientRect) const {
    SetTextColor(hdc, TEXT_WHITE_COLOR);
    SetBkMode(hdc, TRANSPARENT);

    GDIObject font(CreateFont(TURN_FONT_SIZE, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial"));
    HFONT oldFont = static_cast<HFONT>(SelectObject(hdc, font));

    const char playerSymbol = game->getCurrentPlayer();

    std::wstring formatString = LoadStringResource(IDS_PLAYER_TURN);
    std::wstring turnText = format_wstring_runtime(formatString, playerSymbol);

    const int textWidth = 200;
    const int x = (clientRect.right - textWidth) / 2;
    const int y = TURN_TEXT_Y;

    RECT textRect = { x, y, x + textWidth, y + TURN_TEXT_HEIGHT };
    DrawText(hdc, turnText.c_str(), -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    SelectObject(hdc, oldFont);
}

void Renderer::drawScore(HDC hdc, const RECT& clientRect) const {
    SetTextColor(hdc, TEXT_WHITE_COLOR);
    SetBkMode(hdc, TRANSPARENT);

    GDIObject font(CreateFont(SCORE_FONT_SIZE, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial"));
    HFONT oldFont = static_cast<HFONT>(SelectObject(hdc, font));

    std::wstring xScoreFormat = LoadStringResource(IDS_X_WINS);
    std::wstring xScoreText = format_wstring_runtime(xScoreFormat, game->getXWins());

    RECT xScoreRect = { SCORE_MARGIN, TURN_TEXT_Y, SCORE_MARGIN + SCORE_TEXT_WIDTH, TURN_TEXT_Y + TURN_TEXT_HEIGHT };
    DrawText(hdc, xScoreText.c_str(), -1, &xScoreRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    std::wstring oScoreFormat = LoadStringResource(IDS_O_WINS);
    std::wstring oScoreText = format_wstring_runtime(oScoreFormat, game->getOWins());

    RECT oScoreRect = { clientRect.right - SCORE_MARGIN - SCORE_TEXT_WIDTH, TURN_TEXT_Y,
                       clientRect.right - SCORE_MARGIN, TURN_TEXT_Y + TURN_TEXT_HEIGHT };
    DrawText(hdc, oScoreText.c_str(), -1, &oScoreRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    SelectObject(hdc, oldFont);
}

void Renderer::drawWinDialog(HDC hdc, const RECT& clientRect) {
    winDialogRect.left = (clientRect.right - WIN_DIALOG_WIDTH) / 2;
    winDialogRect.top = (clientRect.bottom - WIN_DIALOG_HEIGHT) / 2;
    winDialogRect.right = winDialogRect.left + WIN_DIALOG_WIDTH;
    winDialogRect.bottom = winDialogRect.top + WIN_DIALOG_HEIGHT;

    GDIObject whiteBrush(CreateSolidBrush(BOARD_WHITE_COLOR));
    FillRect(hdc, &winDialogRect, whiteBrush);

    GDIObject blackPen(CreatePen(PS_SOLID, CELL_BORDER_WIDTH, BOARD_BLACK_COLOR));
    HPEN oldPen = static_cast<HPEN>(SelectObject(hdc, blackPen));
    Rectangle(hdc, winDialogRect.left, winDialogRect.top, winDialogRect.right, winDialogRect.bottom);
    SelectObject(hdc, oldPen);

    SetTextColor(hdc, TEXT_GREEN_COLOR);
    SetBkMode(hdc, TRANSPARENT);

    GDIObject font(CreateFont(WIN_FONT_SIZE, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial"));
    HFONT oldFont = static_cast<HFONT>(SelectObject(hdc, font));

    std::wstring winText;
    const char winner = game->getWinner();
    if (winner != ' ') {
        std::wstring formatString = LoadStringResource(IDS_PLAYER_WINS);
        winText = format_wstring_runtime(formatString, winner);
    }
    else {
        winText = LoadStringResource(IDS_DRAW);
    }

    DrawText(hdc, winText.c_str(), -1, &winDialogRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    SelectObject(hdc, oldFont);
}

void Renderer::getBoardCellFromPoint(int x, int y, const RECT& clientRect, int& row, int& col) const {
    int boardSize, startX, startY, cellSize;
    calculateBoardMetrics(clientRect, boardSize, startX, startY, cellSize);

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