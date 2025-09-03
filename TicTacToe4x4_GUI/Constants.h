#pragma once

#define IDM_GAME_RESTART     201
#define IDM_GAME_INFINITE    202  
#define IDM_GAME_EXIT        203
#define IDM_GAME_RESET_SCORE 204

#define BOARD_SIZE               4
#define MAX_SYMBOLS_PER_PLAYER   4

#define BOARD_MARGIN         40
#define BOARD_TOP_OFFSET     80
#define TURN_TEXT_Y          20
#define TURN_TEXT_HEIGHT     30
#define SCORE_MARGIN         20
#define SCORE_TEXT_WIDTH     100
#define WIN_DIALOG_WIDTH     300
#define WIN_DIALOG_HEIGHT    100

#define CELL_BORDER_WIDTH    2
#define SYMBOL_PEN_WIDTH     4
#define SYMBOL_SIZE_RATIO    3
#define TURN_FONT_SIZE       24
#define SCORE_FONT_SIZE      18
#define WIN_FONT_SIZE        28

#define BLACK_COLOR          RGB(0, 0, 0)
#define WHITE_COLOR          RGB(255, 255, 255) 
#define GRAY_COLOR           RGB(128, 128, 128)
#define GREEN_COLOR          RGB(0, 128, 0)
#define RED_COLOR            RGB(255, 0, 0)
#define BLUE_COLOR           RGB(0, 0, 255)
#define PURPLE_COLOR         RGB(128, 0, 128)

#define BOARD_BLACK_COLOR    BLACK_COLOR
#define BOARD_WHITE_COLOR    WHITE_COLOR
#define BOARD_GRAY_COLOR     GRAY_COLOR
#define TEXT_WHITE_COLOR     WHITE_COLOR
#define TEXT_GREEN_COLOR     GREEN_COLOR
#define PLAYER_X_COLOR       RED_COLOR
#define PLAYER_O_COLOR       BLUE_COLOR
#define EXPIRING_COLOR       PURPLE_COLOR