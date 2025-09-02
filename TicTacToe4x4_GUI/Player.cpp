#include "Player.h"

Player::Player(char playerSymbol) : symbol(playerSymbol) {}

char Player::getSymbol() const {
    return symbol;
}