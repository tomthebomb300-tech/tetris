#ifndef TYPES_H
#define TYPES_H

typedef enum gameState {
    GAME_STATE_PLAYING, 
    GAME_STATE_PAUSED,
    GAME_STATE_OVER
} GAMESTATE;

typedef enum cellState{
    CELL_EMPTY,
    CELL_RED,
    CELL_GREEN,
    CELL_BLUE,
    CELL_PURPLE,
    CELL_CYAN,
    CELL_ORANGE,
    CELL_YELLOW,
    CELL_OCCUPIED
} CELLSTATE;

typedef enum collisionType{
    RIGHT_WALL,
    LEFT_WALL,
    BOTTOM_WALL,
    TOP_WALL,
    NONE,
    LAYER
} COLLISIONTYPE;

typedef enum keyDown{
    RIGHT,
    LEFT,
    UP,
    DOWN
} KEYDOWN;

typedef struct colour{
    float red;
    float green;
    float blue;
} Colour;

typedef struct coordinates{
    int x;
    int y;
} Coordinates;

typedef struct piece{
    int rotations[4];
    Colour colour;
    CELLSTATE cellState;
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
    CELLSTATE **cells;
} Board;

typedef struct game {
    Piece *pieces;
    Colour *colours;
    int numPieces;
    GAMESTATE state;
    Board board;
    int score;
    DWORD lastMoveTime;
} Game;

#endif