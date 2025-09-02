#pragma once

class Player {
private:
    char symbol;

public:
    explicit Player(char playerSymbol);
    char getSymbol() const;
};