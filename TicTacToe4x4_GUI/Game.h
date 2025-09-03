#pragma once

#include "Constants.h"
#include <vector>
#include <climits>

struct MoveHistory {
    int row, col;
    char player;
    int moveNumber;
};

class Game {
private:
    static const int BOARD_SIZE_INTERNAL = 4;
    std::vector<std::vector<char>> grid;
    char currentPlayerSymbol;

    bool gameEnded;
    char winner;
    bool infiniteMode;

    std::vector<MoveHistory> moveHistory;
    int moveCounter;

    int xWins;
    int oWins;

    bool isValidPosition(int row, int col) const;

    void switchPlayer();
    void removeOldestMove(char player);
    bool isOldestMove(int row, int col, char player) const;
    bool checkWin(char player) const;
    bool isBoardFull() const;

public:
    Game();

    bool isGameEnded() const { return gameEnded; }
    char getWinner() const { return winner; }
    char getCurrentPlayer() const { return currentPlayerSymbol; }
    bool isInfiniteMode() const { return infiniteMode; }

    int getXWins() const { return xWins; }
    int getOWins() const { return oWins; }
    void resetScore();

    bool makeMove(int row, int col);
    void restart();
    void toggleInfiniteMode();

    char getSymbolAt(int row, int col) const;
    bool isPositionEmpty(int row, int col) const;
    bool isSymbolExpiring(int row, int col) const;
    bool canPlaceAt(int row, int col) const;
};