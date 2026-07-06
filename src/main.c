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

void drawGrid(){
    glColor3f(1.0f, 1.0f, 1.0f);    // Grey lines

    glBegin(GL_LINES);

    // Vertical lines
    for (int x = 0; x <= BOARD_COLS; x++){
        glVertex2i(x, 0);
        glVertex2i(x, BOARD_ROWS);
    }

    // Horizontal lines
    for (int y = 0; y <= BOARD_ROWS; y++){
        glVertex2i(0, y);
        glVertex2i(BOARD_COLS, y);
    }
    glEnd();
}

void initializeGame(){
    game = malloc(sizeof(Game));
    game->state = GAME_STATE_PLAYING;
    game->score = 0;
    Board board = {
        .rows = BOARD_ROWS,
        .cols = BOARD_COLS
    };
    game->board = board;
}

void setupGraphics(){
    PIXELFORMATDESCRIPTOR pfd = {sizeof(PIXELFORMATDESCRIPTOR), 1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA, 32};
    int format = ChoosePixelFormat(CLIENT_AREA_HANDLE, &pfd);
    SetPixelFormat(CLIENT_AREA_HANDLE, format, &pfd);
    HGLRC glrc = wglCreateContext(CLIENT_AREA_HANDLE);
    wglMakeCurrent(CLIENT_AREA_HANDLE, glrc);

    glMatrixMode(GL_PROJECTION); //Telling openGL next lines affect projection matrix.
    glLoadIdentity();
    glOrtho(0, 10, 20, 0, -1, 1); //left, right, bottom, top, near, far
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
    switch(msg){
        case WM_KEYDOWN:
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

    setupGraphics();
    initializeGame();
    

    int run = 1;
    while(run){
        //The Message Loop
        while(PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE)){
            if(Msg.message == WM_QUIT)
                run = 0;
            TranslateMessage(&Msg);
            DispatchMessage(&Msg);
        }
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        drawGrid();

        SwapBuffers(CLIENT_AREA_HANDLE);
    }
    return Msg.wParam;
}