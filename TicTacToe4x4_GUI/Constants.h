#pragma once

#include <windows.h>

constexpr int IDM_GAME_RESTART = 201;
constexpr int IDM_GAME_INFINITE = 202;
constexpr int IDM_GAME_EXIT = 203;
constexpr int IDM_GAME_RESET_SCORE = 204;

constexpr int BOARD_SIZE = 4;
constexpr int MAX_SYMBOLS_PER_PLAYER = 4;

constexpr int BOARD_MARGIN = 40;
constexpr int BOARD_TOP_OFFSET = 80;
constexpr int TURN_TEXT_Y = 20;
constexpr int TURN_TEXT_HEIGHT = 30;
constexpr int SCORE_MARGIN = 20;
constexpr int SCORE_TEXT_WIDTH = 100;
constexpr int WIN_DIALOG_WIDTH = 300;
constexpr int WIN_DIALOG_HEIGHT = 100;

constexpr int CELL_BORDER_WIDTH = 2;
constexpr int SYMBOL_PEN_WIDTH = 4;
constexpr int SYMBOL_SIZE_RATIO = 3;
constexpr int TURN_FONT_SIZE = 24;
constexpr int SCORE_FONT_SIZE = 18;
constexpr int WIN_FONT_SIZE = 28;

constexpr COLORREF BLACK_COLOR = RGB(0, 0, 0);
constexpr COLORREF WHITE_COLOR = RGB(255, 255, 255);
constexpr COLORREF GRAY_COLOR = RGB(128, 128, 128);
constexpr COLORREF GREEN_COLOR = RGB(0, 128, 0);
constexpr COLORREF RED_COLOR = RGB(255, 0, 0);
constexpr COLORREF BLUE_COLOR = RGB(0, 0, 255);
constexpr COLORREF PURPLE_COLOR = RGB(128, 0, 128);

constexpr COLORREF BOARD_BLACK_COLOR = BLACK_COLOR;
constexpr COLORREF BOARD_WHITE_COLOR = WHITE_COLOR;
constexpr COLORREF BOARD_GRAY_COLOR = GRAY_COLOR;
constexpr COLORREF TEXT_WHITE_COLOR = WHITE_COLOR;
constexpr COLORREF TEXT_GREEN_COLOR = GREEN_COLOR;
constexpr COLORREF PLAYER_X_COLOR = RED_COLOR;
constexpr COLORREF PLAYER_O_COLOR = BLUE_COLOR;
constexpr COLORREF EXPIRING_COLOR = PURPLE_COLOR;