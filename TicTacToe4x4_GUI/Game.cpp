#include "Game.h"
#include <algorithm>

Game::Game() : player1('X'), player2('O'), currentPlayer(&player1),
gameEnded(false), winner(' '), infiniteMode(false), moveCounter(0) {
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
            if (move.player == currentPlayer->getSymbol())
                playerMoveCount++;
        }

        if (playerMoveCount >= MAX_SYMBOLS_PER_PLAYER) {
            removeOldestMove(currentPlayer->getSymbol());
        }
    }

    if (gameBoard.makeMove(row, col, currentPlayer->getSymbol())) {
        moveHistory.push_back({ row, col, currentPlayer->getSymbol(), moveCounter++ });

        if (gameBoard.checkWin(currentPlayer->getSymbol())) {
            winner = currentPlayer->getSymbol();
            gameEnded = true;
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
    currentPlayer = &player1;
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

    if (symbol != currentPlayer->getSymbol()) return false;

    int playerMoveCount = 0;
    int oldestMoveNumber = INT_MAX;

    for (const auto& move : moveHistory) {
        if (move.player == symbol) {
            playerMoveCount++;
            if (move.moveNumber < oldestMoveNumber) {
                oldestMoveNumber = move.moveNumber;
            }
        }
    }

    if (playerMoveCount >= MAX_SYMBOLS_PER_PLAYER) {
        for (const auto& move : moveHistory) {
            if (move.row == row && move.col == col &&
                move.player == symbol && move.moveNumber == oldestMoveNumber) {
                return true;
            }
        }
    }

    return false;
}

void Game::switchPlayer() {
    currentPlayer = (currentPlayer == &player1) ? &player2 : &player1;
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