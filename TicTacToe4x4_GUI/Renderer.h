#pragma once

#include <windows.h>
#include "Constants.h"

class Game;

class Renderer {
private:
    Game* game;
    RECT winDialogRect;

    void drawBoard(HDC hdc, const RECT& clientRect) const;
    void drawSymbols(HDC hdc, const RECT& clientRect) const;
    void drawPlayerTurn(HDC hdc, const RECT& clientRect) const;
    void drawWinDialog(HDC hdc, const RECT& clientRect);

public:
    explicit Renderer(Game* gameInstance);

    void render(HDC hdc, const RECT& clientRect);
    void getBoardCellFromPoint(int x, int y, const RECT& clientRect, int& row, int& col) const;

    const RECT& getWinDialogRect() const { return winDialogRect; }
};