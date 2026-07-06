#ifndef TYPES_H
#define TYPES_H

typedef enum gameState {
    GAME_STATE_PLAYING, 
    GAME_STATE_PAUSED,
    GAME_STATE_OVER
} GameState;

typedef struct piece{
    int rotations[4];
} Piece;

typedef struct board {
    int rows;
    int cols;
} Board;

typedef struct game {
    GameState state;
    Board board;
    int score;
} Game;

#endif