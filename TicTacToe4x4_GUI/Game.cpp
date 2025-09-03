#include "Game.h"
#include <algorithm>

Game::Game() : currentPlayerSymbol('X'), gameEnded(false), winner(' '),
infiniteMode(false), moveCounter(0), xWins(0), oWins(0) {
}

bool Game::canPlaceAt(int row, int col) const {
    if (infiniteMode && isSymbolExpiring(row, col)) {
        return false;
    }
    return isPositionEmpty(row, col);
}

bool Game::makeMove(int row, int col) {
    if (gameEnded) return false;

    if (!canPlaceAt(row, col)) return false;

    if (infiniteMode) {
        int playerMoveCount = 0;
        for (const auto& move : moveHistory) {
            if (move.player == currentPlayerSymbol)
                playerMoveCount++;
        }

        if (playerMoveCount >= MAX_SYMBOLS_PER_PLAYER) {
            removeOldestMove(currentPlayerSymbol);
        }
    }

    if (gameBoard.makeMove(row, col, currentPlayerSymbol)) {
        moveHistory.push_back({ row, col, currentPlayerSymbol, moveCounter++ });

        if (gameBoard.checkWin(currentPlayerSymbol)) {
            winner = currentPlayerSymbol;
            gameEnded = true;

            if (winner == 'X') {
                xWins++;
            }
            else if (winner == 'O') {
                oWins++;
            }
        }
        else if (gameBoard.isFull()) {
            winner = ' ';
            gameEnded = true;
        }
        else {
            switchPlayer();
        }

        return true;
    }

    return false;
}

void Game::restart() {
    gameBoard.reset();
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
        return gameBoard.getSymbolAt(row, col);
    }
}

bool Game::isPositionEmpty(int row, int col) const {
    return getSymbolAt(row, col) == ' ';
}

bool Game::isSymbolExpiring(int row, int col) const {
    if (!infiniteMode) return false;

    const char symbol = getSymbolAt(row, col);
    if (symbol == ' ') return false;

    if (symbol != currentPlayerSymbol) return false;

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

    if (playerMoveCount < MAX_SYMBOLS_PER_PLAYER) {
        return false;
    }

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
        gameBoard.reset();
        moveHistory.erase(it);

        for (const auto& move : moveHistory) {
            gameBoard.makeMove(move.row, move.col, move.player);
        }
    }
}

void Game::resetScore() {
    xWins = 0;
    oWins = 0;
}