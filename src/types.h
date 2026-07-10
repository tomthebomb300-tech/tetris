#ifndef TYPES_H
#define TYPES_H

typedef enum gameState {
    GAME_STATE_PLAYING, 
    GAME_STATE_PAUSED,
    GAME_STATE_OVER
} GAMESTATE;

typedef enum cellState{
    CELL_EMPTY,
    CELL_OCCUPIED
} CELLSTATE;

typedef enum keyDown{
    RIGHT,
    LEFT,
    UP,
    DOWN
} KEYDOWN;

typedef struct coordinates{
    int x;
    int y;
} Coordinates;

typedef struct piece{
    int rotations[4];
} Piece;

typedef struct currentPiece{
    Piece piece;
    int rotation;           //Current orientation of piece.
    Coordinates drawOrigin; //Origin of where the piece is drawen from.
    Coordinates coords[4];  //Coordinates of piece in relation to drawOrgin
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
    GAMESTATE state;
    Board board;
    int score;
} Game;

#endif