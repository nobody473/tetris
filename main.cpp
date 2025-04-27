#include <iostream>
#include <vector>
#include <conio.h>
#include <Windows.h>
#include <ctime>
#include <cstdlib>

using namespace std;

// 게임 상수 정의
const int WIDTH = 12;
const int HEIGHT = 22;
const int BOARD_X = 2;
const int BOARD_Y = 1;
const int BLOCK_TYPES = 7;

// 테트리스 블록 정의 (7가지 모양)
const int blocks[BLOCK_TYPES][4][4][2] = {
    // I 블록
    {
        {{0, 0}, {0, 1}, {0, 2}, {0, 3}},
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}},
        {{0, 0}, {0, 1}, {0, 2}, {0, 3}},
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}}
    },
    // O 블록
    {
        {{0, 0}, {0, 1}, {1, 0}, {1, 1}},
        {{0, 0}, {0, 1}, {1, 0}, {1, 1}},
        {{0, 0}, {0, 1}, {1, 0}, {1, 1}},
        {{0, 0}, {0, 1}, {1, 0}, {1, 1}}
    },
    // T 블록
    {
        {{0, 0}, {0, 1}, {0, 2}, {1, 1}},
        {{0, 1}, {1, 0}, {1, 1}, {2, 1}},
        {{1, 0}, {1, 1}, {1, 2}, {0, 1}},
        {{0, 0}, {1, 0}, {2, 0}, {1, 1}}
    },
    // L 블록
    {
        {{0, 0}, {0, 1}, {0, 2}, {1, 2}},
        {{0, 1}, {1, 1}, {2, 0}, {2, 1}},
        {{0, 0}, {1, 0}, {1, 1}, {1, 2}},
        {{0, 0}, {0, 1}, {1, 0}, {2, 0}}
    },
    // J 블록
    {
        {{0, 2}, {1, 0}, {1, 1}, {1, 2}},
        {{0, 0}, {0, 1}, {1, 1}, {2, 1}},
        {{0, 0}, {0, 1}, {0, 2}, {1, 0}},
        {{0, 0}, {1, 0}, {2, 0}, {2, 1}}
    },
    // S 블록
    {
        {{0, 1}, {0, 2}, {1, 0}, {1, 1}},
        {{0, 0}, {1, 0}, {1, 1}, {2, 1}},
        {{0, 1}, {0, 2}, {1, 0}, {1, 1}},
        {{0, 0}, {1, 0}, {1, 1}, {2, 1}}
    },
    // Z 블록
    {
        {{0, 0}, {0, 1}, {1, 1}, {1, 2}},
        {{0, 1}, {1, 0}, {1, 1}, {2, 0}},
        {{0, 0}, {0, 1}, {1, 1}, {1, 2}},
        {{0, 1}, {1, 0}, {1, 1}, {2, 0}}
    }
};

// 블록의 색상 정의
const char BLOCK_COLORS[BLOCK_TYPES + 1] = { 'I', 'O', 'T', 'L', 'J', 'S', 'Z', '.' };

// 게임 클래스
class Tetris {
private:
    vector<vector<char>> board;
    int currentBlock;
    int nextBlock;
    int rotation;
    int posX, posY;
    int score;
    bool gameOver;

public:
    Tetris() {
        // 게임 보드 초기화
        board = vector<vector<char>>(HEIGHT, vector<char>(WIDTH, '.'));

        // 외벽 설정
        for (int i = 0; i < HEIGHT; i++) {
            board[i][0] = '#';
            board[i][WIDTH - 1] = '#';
        }
        for (int i = 0; i < WIDTH; i++) {
            board[HEIGHT - 1][i] = '#';
        }

        // 게임 변수 초기화
        srand(static_cast<unsigned>(time(nullptr)));
        currentBlock = rand() % BLOCK_TYPES;
        nextBlock = rand() % BLOCK_TYPES;
        rotation = 0;
        posX = WIDTH / 2 - 1;
        posY = 0;
        score = 0;
        gameOver = false;
    }

    // 커서 위치 설정 함수
    void gotoxy(int x, int y) {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    // 블록 충돌 검사 함수
    bool checkCollision(int blockType, int rot, int x, int y) {
        for (int i = 0; i < 4; i++) {
            int newX = x + blocks[blockType][rot][i][1];
            int newY = y + blocks[blockType][rot][i][0];

            if (newX < 0 || newX >= WIDTH || newY < 0 || newY >= HEIGHT) {
                return true;
            }

            if (board[newY][newX] != '.') {
                return true;
            }
        }
        return false;
    }

    // 블록 그리기 함수
    void drawBlock(bool erase = false) {
        for (int i = 0; i < 4; i++) {
            int x = posX + blocks[currentBlock][rotation][i][1];
            int y = posY + blocks[currentBlock][rotation][i][0];

            gotoxy(BOARD_X + x * 2, BOARD_Y + y);
            if (!erase) {
                cout << BLOCK_COLORS[currentBlock] << BLOCK_COLORS[currentBlock];
            }
            else {
                cout << "  ";
            }
        }
    }

    // 게임 보드 그리기 함수
    void drawBoard() {
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                gotoxy(BOARD_X + j * 2, BOARD_Y + i);
                if (board[i][j] == '#') {
                    cout << "##";
                }
                else if (board[i][j] == '.') {
                    cout << "  ";
                }
                else {
                    cout << board[i][j] << board[i][j];
                }
            }
        }
    }

    // 게임 정보 표시 함수
    void drawInfo() {
        gotoxy(BOARD_X + WIDTH * 2 + 5, BOARD_Y);
        cout << "TETRIS GAME";
        gotoxy(BOARD_X + WIDTH * 2 + 5, BOARD_Y + 2);
        cout << "Score: " << score;
        gotoxy(BOARD_X + WIDTH * 2 + 5, BOARD_Y + 4);
        cout << "Next Block:";

        // 다음 블록 표시
        for (int i = 0; i < 4; i++) {
            int x = blocks[nextBlock][0][i][1];
            int y = blocks[nextBlock][0][i][0];
            gotoxy(BOARD_X + WIDTH * 2 + 10 + x * 2, BOARD_Y + 6 + y);
            cout << BLOCK_COLORS[nextBlock] << BLOCK_COLORS[nextBlock];
        }

        gotoxy(BOARD_X + WIDTH * 2 + 5, BOARD_Y + 10);
        cout << "Controls:";
        gotoxy(BOARD_X + WIDTH * 2 + 5, BOARD_Y + 12);
        cout << "← → : Move";
        gotoxy(BOARD_X + WIDTH * 2 + 5, BOARD_Y + 13);
        cout << "↑ : Rotate";
        gotoxy(BOARD_X + WIDTH * 2 + 5, BOARD_Y + 14);
        cout << "↓ : Drop";
        gotoxy(BOARD_X + WIDTH * 2 + 5, BOARD_Y + 15);
        cout << "ESC : Quit";
    }

    // 블록 고정 함수
    void fixBlock() {
        for (int i = 0; i < 4; i++) {
            int x = posX + blocks[currentBlock][rotation][i][1];
            int y = posY + blocks[currentBlock][rotation][i][0];

            board[y][x] = BLOCK_COLORS[currentBlock];
        }

        // 줄 제거 처리
        for (int i = 0; i < HEIGHT - 1; i++) {
            bool lineFull = true;
            for (int j = 1; j < WIDTH - 1; j++) {
                if (board[i][j] == '.') {
                    lineFull = false;
                    break;
                }
            }

            if (lineFull) {
                for (int k = i; k > 0; k--) {
                    for (int j = 1; j < WIDTH - 1; j++) {
                        board[k][j] = board[k - 1][j];
                    }
                }
                score += 100;
            }
        }

        // 새 블록 생성
        currentBlock = nextBlock;
        nextBlock = rand() % BLOCK_TYPES;
        rotation = 0;
        posX = WIDTH / 2 - 1;
        posY = 0;

        // 게임 오버 체크
        if (checkCollision(currentBlock, rotation, posX, posY)) {
            gameOver = true;
        }
    }

    // 게임 루프 함수
    void run() {
        system("cls");
        drawBoard();
        drawInfo();

        int frameCount = 0;

        while (!gameOver) {
            if (_kbhit()) {
                int key = _getch();
                drawBlock(true);

                switch (key) {
                case 224:  // 방향키 프리픽스
                    key = _getch();
                    switch (key) {
                    case 72:  // 상
                        if (!checkCollision(currentBlock, (rotation + 1) % 4, posX, posY)) {
                            rotation = (rotation + 1) % 4;
                        }
                        break;
                    case 75:  // 좌
                        if (!checkCollision(currentBlock, rotation, posX - 1, posY)) {
                            posX--;
                        }
                        break;
                    case 77:  // 우
                        if (!checkCollision(currentBlock, rotation, posX + 1, posY)) {
                            posX++;
                        }
                        break;
                    case 80:  // 하
                        if (!checkCollision(currentBlock, rotation, posX, posY + 1)) {
                            posY++;
                        }
                        break;
                    }
                    break;
                case 27:  // ESC
                    gameOver = true;
                    break;
                }

                drawBlock();
            }

            // 프레임 관리
            frameCount++;
            if (frameCount >= 20) {
                frameCount = 0;
                drawBlock(true);

                if (!checkCollision(currentBlock, rotation, posX, posY + 1)) {
                    posY++;
                }
                else {
                    fixBlock();
                    drawBoard();
                    drawInfo();
                }

                drawBlock();
            }

            Sleep(10);
        }

        // 게임 오버 메시지
        gotoxy(BOARD_X + WIDTH / 2 - 4, BOARD_Y + HEIGHT / 2);
        cout << "GAME OVER!";
        gotoxy(BOARD_X + WIDTH / 2 - 7, BOARD_Y + HEIGHT / 2 + 1);
        cout << "Final Score: " << score;
        gotoxy(0, BOARD_Y + HEIGHT + 2);
    }
};

int main() {
    // 콘솔 커서 숨기기
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

    Tetris game;
    game.run();

    return 0;
}