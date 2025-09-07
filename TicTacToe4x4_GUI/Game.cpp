#include "Game.h"
#include <algorithm>
#include <numeric>

Game::Game() : grid(BOARD_SIZE, std::vector<char>(BOARD_SIZE, ' ')),
currentPlayerSymbol('X'), gameEnded(false), winner(' '),
infiniteMode(false), moveCounter(0), xWins(0), oWins(0) {
}

bool Game::isValidPosition(int row, int col) const {
    return (row >= 1 && row <= BOARD_SIZE && col >= 1 && col <= BOARD_SIZE);
}

bool Game::makeMove(int row, int col) {
    if (gameEnded || !isValidPosition(row, col)) return false;

    if (!canPlaceAt(row, col)) return false;

    if (infiniteMode) {
        int playerMoveCount = 0;
        for (const auto& move : moveHistory) {
            if (move.player == currentPlayerSymbol) playerMoveCount++;
        }

        if (playerMoveCount >= MAX_SYMBOLS_PER_PLAYER) {
            removeOldestMove(currentPlayerSymbol);
        }
    }

    const int arrayRow = row - 1;
    const int arrayCol = col - 1;

    if (grid[arrayRow][arrayCol] != ' ') return false;

    grid[arrayRow][arrayCol] = currentPlayerSymbol;
    moveHistory.push_back({ row, col, currentPlayerSymbol, moveCounter++ });

    if (checkWin(currentPlayerSymbol)) {
        winner = currentPlayerSymbol;
        gameEnded = true;

        if (winner == 'X') xWins++;
        else if (winner == 'O') oWins++;
    }
    else if (isBoardFull()) {
        winner = ' ';
        gameEnded = true;
    }
    else {
        switchPlayer();
    }

    return true;
}

bool Game::checkWin(char player) const {
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (std::all_of(grid[i].begin(), grid[i].end(),
            [player](char cell) { return cell == player; })) {
            return true;
        }
    }

    for (int j = 0; j < BOARD_SIZE; j++) {
        if (std::all_of(grid.begin(), grid.end(),
            [j, player](const std::vector<char>& row) { return row[j] == player; })) {
            return true;
        }
    }

    bool mainDiagWin = true;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (grid[i][i] != player) {
            mainDiagWin = false;
            break;
        }
    }
    if (mainDiagWin) return true;

    bool antiDiagWin = true;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (grid[i][BOARD_SIZE - 1 - i] != player) {
            antiDiagWin = false;
            break;
        }
    }
    return antiDiagWin;
}

bool Game::isBoardFull() const {
    return std::all_of(grid.begin(), grid.end(),
        [](const std::vector<char>& row) {
            return std::all_of(row.begin(), row.end(),
                [](char cell) { return cell != ' '; });
        });
}

void Game::restart() {
    for (auto& row : grid) {
        std::fill(row.begin(), row.end(), ' ');
    }

    currentPlayerSymbol = 'X';
    gameEnded = false;
    winner = ' ';
    moveHistory.clear();
    moveCounter = 0;
}

void Game::toggleInfiniteMode() {
    infiniteMode = !infiniteMode;
    restart();
}

char Game::getSymbolAt(int row, int col) const {
    if (infiniteMode) {
        for (const auto& move : moveHistory) {
            if (move.row == row && move.col == col) {
                return move.player;
            }
        }
        return ' ';
    }
    else {
        if (!isValidPosition(row, col)) return ' ';
        return grid[row - 1][col - 1];
    }
}

bool Game::isPositionEmpty(int row, int col) const {
    return getSymbolAt(row, col) == ' ';
}

bool Game::canPlaceAt(int row, int col) const {
    if (infiniteMode && isSymbolExpiring(row, col)) {
        return false;
    }
    return isPositionEmpty(row, col);
}

bool Game::isSymbolExpiring(int row, int col) const {
    if (!infiniteMode) return false;

    const char symbol = getSymbolAt(row, col);
    if (symbol == ' ' || symbol != currentPlayerSymbol) return false;

    return isOldestMove(row, col, symbol);
}

bool Game::isOldestMove(int row, int col, char player) const {
    int playerMoveCount = 0;
    int oldestMoveNumber = INT_MAX;

    for (const auto& move : moveHistory) {
        if (move.player == player) {
            playerMoveCount++;
            if (move.moveNumber < oldestMoveNumber) {
                oldestMoveNumber = move.moveNumber;
            }
        }
    }

    if (playerMoveCount < MAX_SYMBOLS_PER_PLAYER) return false;

    for (const auto& move : moveHistory) {
        if (move.row == row && move.col == col &&
            move.player == player && move.moveNumber == oldestMoveNumber) {
            return true;
        }
    }

    return false;
}

void Game::switchPlayer() {
    currentPlayerSymbol = (currentPlayerSymbol == 'X') ? 'O' : 'X';
}

void Game::removeOldestMove(char player) {
    auto it = std::find_if(moveHistory.begin(), moveHistory.end(),
        [player](const MoveHistory& move) { return move.player == player; });

    if (it != moveHistory.end()) {
        for (auto& row : grid) {
            std::fill(row.begin(), row.end(), ' ');
        }
        moveHistory.erase(it);

        for (const auto& move : moveHistory) {
            grid[move.row - 1][move.col - 1] = move.player;
        }
    }
}

void Game::resetScore() {
    xWins = 0;
    oWins = 0;
}