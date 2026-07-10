#include <stdio.h>
#include <windows.h>
#include <GL/gl.h>

#include "types.h"

const char WINDOW_CLASS_NAME[] = "tetris_window";
HDC CLIENT_AREA_HANDLE;

const int CELL_SIZE = 50;
const int BOARD_ROWS = 20;
const int BOARD_COLS = 10;
const int CLIENT_AREA_HEIGHT = BOARD_ROWS * CELL_SIZE; //Height of client area
const int CLIENT_AREA_WIDTH = BOARD_COLS * CELL_SIZE;  //Width of client area
const int WINDOW_HEIGHT = CLIENT_AREA_HEIGHT + 20; //Window width to get desired client area
const int WINDOW_WIDTH = CLIENT_AREA_WIDTH + 43; //Window height to get desired client area


Game *game;
int key_down[sizeof(KEYDOWN)];

Piece PIECE_O = {
    .rotations = {
        0b0000011001100000,
        0b0000011001100000,
        0b0000011001100000,
        0b0000011001100000
    }
};

Piece PIECE_I = {
    .rotations = {
        0b0000111100000000,
        0b0010001000100010,
        0b0000000011110000,
        0b0100010001000100
    }
};

Piece PIECE_S = {
    .rotations = {
        0b0000011011000000,
        0b0000010001100010,
        0b0000011011000000,
        0b0000010001100010
    }
};

Piece PIECE_Z = {
    .rotations = {
        0b0000011000110000,
        0b0000001001100100,
        0b0000011000110000,
        0b0000001001100100
    }
};

Piece PIECE_L = {
    .rotations = {
        0b0100010001100000,
        0b0000011101000000,
        0b0000011000100010,
        0b0000001011100000
    }
};

Piece PIECE_J = {
    .rotations = {
        0b0010001001100000,
        0b0000010001110000,
        0b0000011001000100,
        0b0000111000100000
    }
};

Piece PIECE_T = {
    .rotations = {
        0b0000011100100000,
        0b0010011000100000,
        0b0000010011100000,
        0b0100011001000000,
    }
};

void drawGrid(){
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);

    // Vertical lines
    for (int x = 0; x <= game->board.cols; x++){
        glVertex2i(x, 0);
        glVertex2i(x, game->board.rows);
    }

    // Horizontal lines
    for (int y = 0; y <= game->board.rows; y++){
        glVertex2i(0, y);
        glVertex2i(game->board.cols, y);
    }
    glEnd();
}

void printBoard(){
    for(int row = 0; row < game->board.rows; row++){
        for(int col = 0; col < game->board.cols; col++){
            if(game->board.cells[row][col] == CELL_EMPTY){
                printf("0,");
            }
            else{
                printf("1,");
            }
        }
        printf("\n");
    }
}

void drawSquare(int x, int y){
    glBegin(GL_QUADS);
    glVertex2f(x,     y);
    glVertex2f(x + 1, y);
    glVertex2f(x + 1, y + 1);
    glVertex2f(x,     y + 1);
    glEnd();
}

void drawCurrentPiece(){
    for(int i = 0; i < 4; i++){
        Coordinates c = game->board.currPiece.coords[i];
        Coordinates drawOrigin = game->board.currPiece.drawOrigin;
        drawSquare(drawOrigin.x + c.x, drawOrigin.y + c.y);
    }
    
}

void initializeGame(){
    game = malloc(sizeof(Game));
    game->state = GAME_STATE_PLAYING;
    game->score = 0;
    game->numPieces = 7;
    game->pieces = malloc(game->numPieces * sizeof(Piece));
    game->pieces[0] = PIECE_I;
    game->pieces[1] = PIECE_L;
    game->pieces[2] = PIECE_J;
    game->pieces[3] = PIECE_T;
    game->pieces[4] = PIECE_O;
    game->pieces[5] = PIECE_S;
    game->pieces[6] = PIECE_Z;
    
    Board board = {.rows = 20, .cols = 10};
    board.cells = malloc(board.rows * sizeof(int *));
    for(int row = 0; row < board.rows; row++){
        board.cells[row] = malloc(board.cols * sizeof(int));
        for(int col = 0; col < board.cols; col++){
            board.cells[row][col] = CELL_EMPTY;
        }
    }
    game->board = board;
}

void destroyGame(){
    for(int y = 0; y < game->board.rows; y++){
        free(game->board.cells[y]);
        game->board.cells[y] = NULL;
    }
    free(game->board.cells);
    game->board.cells = NULL;
    free(game->pieces);
    game->pieces = NULL;
    free(game);
    game = NULL;
}

int leftWallCollision(int x){
    if(x < 0){
        return 1;
    }
    return 0;
}

int rightWallColision(int x){
    if(x >= game->board.cols){
        return 1;
    }
    return 0;
}

int wallCollision(){
    CurrentPiece currPiece = game->board.currPiece;
    for(int i = 0; i < 4; i++){
        if(rightWallColision(currPiece.drawOrigin.x + currPiece.coords[i].x)){
            return 1;
        }
        if(leftWallCollision(currPiece.drawOrigin.x + currPiece.coords[i].x)){
            return 1;
        }
    }
    return 0;
}

void update(){
    if(key_down[DOWN]){
        printf("DOWN\n");
        key_down[DOWN]=0;
    }
    if(key_down[UP]){
        printf("UP\n");
        key_down[UP]=0;
        if(game->board.currPiece.rotation == 3){
            game->board.currPiece.rotation = 0;
        }
        else{
            game->board.currPiece.rotation++;
        }
    }
    if(key_down[RIGHT]){
        printf("RIGHT\n");
        key_down[RIGHT]=0;
        game->board.currPiece.drawOrigin.x++;
        if(wallCollision()){
            game->board.currPiece.drawOrigin.x--;   
        }
            
        }
        if(key_down[LEFT]){
            printf("LEFT\n");
            key_down[LEFT]=0;
            game->board.currPiece.drawOrigin.x--;
            if(wallCollision()){
                game->board.currPiece.drawOrigin.x++;   
            }
    }

    //Adjusting tetris piece in relation to the rotation choice.
    CurrentPiece currPiece = game->board.currPiece;
    int index = 0;
    for(int row = 0; row < 4; row++){
        for(int col = 0; col < 4; col++){
            int shiftBit = row*4+col;
            if(currPiece.piece.rotations[currPiece.rotation] & (0b1000000000000000 >> shiftBit)){
                Coordinates c = {.x = row, .y = col};
                if(leftWallCollision(c.x + game->board.currPiece.drawOrigin.x)){
                    game->board.currPiece.drawOrigin.x++;
                }
                if(rightWallColision(c.x + game->board.currPiece.drawOrigin.x)){
                    game->board.currPiece.drawOrigin.x--;
                }
                game->board.currPiece.coords[index++] = c;
            }
        }
    }

}

void render(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    drawGrid();
    drawCurrentPiece();
    SwapBuffers(CLIENT_AREA_HANDLE);
}

void setupGraphics(){
    PIXELFORMATDESCRIPTOR pfd = {sizeof(PIXELFORMATDESCRIPTOR), 1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA, 32};
    int format = ChoosePixelFormat(CLIENT_AREA_HANDLE, &pfd);
    SetPixelFormat(CLIENT_AREA_HANDLE, format, &pfd);
    HGLRC glrc = wglCreateContext(CLIENT_AREA_HANDLE);
    wglMakeCurrent(CLIENT_AREA_HANDLE, glrc);

    glMatrixMode(GL_PROJECTION); //Telling openGL next lines affect projection matrix.
    glLoadIdentity();
    glOrtho(0, game->board.cols, game->board.rows, 0, -1, 1); //Re-assign coords left, right, bottom, top, near, far
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
    switch(msg){
        case WM_KEYDOWN:
            if(wParam == VK_DOWN){key_down[DOWN]=1;}
            else if(wParam == VK_UP){key_down[UP]=1;}
            else if(wParam == VK_RIGHT){key_down[RIGHT]=1;}
            else if(wParam == VK_LEFT){key_down[LEFT]=1;}
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    WNDCLASSEX wc;
    MSG Msg;

    HBRUSH backgroundBrush = CreateSolidBrush(RGB(0,0,0));

    //Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = backgroundBrush;
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = WINDOW_CLASS_NAME;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    DeleteObject(backgroundBrush);

    if(!RegisterClassEx(&wc)){
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    //Creating the Window
    HWND hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        WINDOW_CLASS_NAME,
        "Tetris",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL){
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    CLIENT_AREA_HANDLE = GetDC(hwnd);

    initializeGame();
    setupGraphics();

    
    
    CurrentPiece currPiece = {
        .rotation = 0,
        .piece = game->pieces[0]
    };
    game->board.currPiece = currPiece;
    Coordinates drawOrigin = {.x = 0, .y = 0};
    game->board.currPiece.drawOrigin = drawOrigin;

    int run = 1;
    while(run){
        //The Message Loop
        while(PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE)){
            if(Msg.message == WM_QUIT)
                run = 0;
            TranslateMessage(&Msg);
            DispatchMessage(&Msg);
        }
        update();
        render();
    }
    destroyGame();
    return Msg.wParam;
}