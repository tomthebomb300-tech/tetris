#ifndef TYPES_H
#define TYPES_H

typedef enum gameState {
    GAME_STATE_PLAYING, 
    GAME_STATE_PAUSED,
    GAME_STATE_OVER
} GameState;

typedef enum cellState{
    CELL_EMPTY,
    CELL_OCCUPIED
} CELLSTATE;

typedef struct piece{
    int rotations[4];
} Piece;

typedef struct currentPiece{
    Piece piece;
    int rotation;
    int x;
    int y;
} CurrentPiece;

typedef struct board {
    Piece nextPiece;
    CurrentPiece currPiece;
    int rows;
    int cols;
    int **cells;
} Board;

typedef struct game {
    Piece *pieces;
    int numPieces;
    GameState state;
    Board board;
    int score;
} Game;

#endif