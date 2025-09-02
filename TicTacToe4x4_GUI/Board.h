#pragma once

#include <vector>

class Board {
private:
    std::vector<std::vector<char>> grid;

    int convertToIndex(int userInput) const;
    bool isValidInput(int row, int col) const;

public:
    static const int SIZE = 4;

    Board();
    int getSize() const;
    bool makeMove(int row, int col, char player);
    bool checkWin(char player) const;
    bool isFull() const;
    void reset();

    char getSymbolAt(int row, int col) const;
    bool isPositionEmpty(int row, int col) const;
};