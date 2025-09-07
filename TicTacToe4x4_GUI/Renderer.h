#pragma once

#include <windows.h>
#include <string>
#include "Constants.h"

class Game;

std::wstring LoadStringResource(UINT stringId);

class Renderer {
private:
    Game* game;
    RECT winDialogRect;

    void drawBoard(HDC hdc, const RECT& clientRect) const;
    void drawSymbols(HDC hdc, const RECT& clientRect) const;
    void drawPlayerTurn(HDC hdc, const RECT& clientRect) const;
    void drawScore(HDC hdc, const RECT& clientRect) const;
    void drawWinDialog(HDC hdc, const RECT& clientRect);

    void calculateBoardMetrics(const RECT& clientRect, int& boardSize, int& startX, int& startY, int& cellSize) const;

public:
    explicit Renderer(Game* gameInstance);

    void render(HDC hdc, const RECT& clientRect);
    void getBoardCellFromPoint(int x, int y, const RECT& clientRect, int& row, int& col) const;

    const RECT& getWinDialogRect() const { return winDialogRect; }
};