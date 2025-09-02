#include "Board.h"
#include <algorithm>

Board::Board() : grid(SIZE, std::vector<char>(SIZE, ' ')) {}

int Board::convertToIndex(int userInput) const {
    return userInput - 1;
}

bool Board::isValidInput(int row, int col) const {
    return (row >= 1 && row <= SIZE && col >= 1 && col <= SIZE);
}

int Board::getSize() const {
    return SIZE;
}

bool Board::makeMove(int row, int col, char player) {
    if (!isValidInput(row, col)) {
        return false;
    }

    const int arrayRow = convertToIndex(row);
    const int arrayCol = convertToIndex(col);

    if (grid[arrayRow][arrayCol] != ' ') {
        return false;
    }

    grid[arrayRow][arrayCol] = player;
    return true;
}

bool Board::checkWin(char player) const {
    for (int i = 0; i < SIZE; i++) {
        if (std::all_of(grid[i].begin(), grid[i].end(),
            [player](char cell) { return cell == player; })) {
            return true;
        }
    }

    for (int j = 0; j < SIZE; j++) {
        if (std::all_of(grid.begin(), grid.end(),
            [j, player](const std::vector<char>& row) { return row[j] == player; })) {
            return true;
        }
    }

    bool mainDiagWin = true;
    for (int i = 0; i < SIZE; i++) {
        if (grid[i][i] != player) {
            mainDiagWin = false;
            break;
        }
    }
    if (mainDiagWin) return true;

    bool antiDiagWin = true;
    for (int i = 0; i < SIZE; i++) {
        if (grid[i][SIZE - 1 - i] != player) {
            antiDiagWin = false;
            break;
        }
    }
    return antiDiagWin;
}

bool Board::isFull() const {
    return std::all_of(grid.begin(), grid.end(),
        [](const std::vector<char>& row) {
            return std::all_of(row.begin(), row.end(),
                [](char cell) { return cell != ' '; });
        });
}

void Board::reset() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            grid[i][j] = ' ';
        }
    }
}

char Board::getSymbolAt(int row, int col) const {
    if (!isValidInput(row, col)) {
        return ' ';
    }

    const int arrayRow = convertToIndex(row);
    const int arrayCol = convertToIndex(col);
    return grid[arrayRow][arrayCol];
}

bool Board::isPositionEmpty(int row, int col) const {
    return getSymbolAt(row, col) == ' ';
}