#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>


int board[4][4];
int score = 0;

int getch() {
    struct termios oldt, newt;
    int input;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    input = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return input;
}

void printMainMenu(){
    system("clear"); // for windows use system("cls")
    printf("|----------------2048 by matthewtop ----------------|\n\n");
    printf("|Use 'WSAD' to move the tiles.                      |\n");
    printf("|Use 'Q' to quit                                    |\n");
    printf("|Press any key to start the game                    |\n\n");
    printf("|---------------------------------------------------|\n");
}

void addRandomTile() {
    int emptyCells[4 * 4][2];
    int emptyCount = 0;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (board[i][j] == 0) {
                emptyCells[emptyCount][0] = i;
                emptyCells[emptyCount][1] = j;
                emptyCount++;
            }
        }
    }

    if (emptyCount > 0) {
        int randomIndex = rand() % emptyCount;
        int x = emptyCells[randomIndex][0];
        int y = emptyCells[randomIndex][1];
        board[x][y] = (rand() % 10 == 0) ? 4 : 2;
    }
}

void initBoard() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            board[i][j] = 0;

    addRandomTile();
    addRandomTile();
}



void printBoard() {
    system("clear"); // for windows use system("cls")
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (board[i][j] == 0) printf(".\t");
            else printf("%d\t", board[i][j]);
        }
        printf("\n");
    }
    printf("Score: %d\n", score);
}

void rotateBoard() {
    int temp[4][4];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            temp[i][j] = board[4 - j - 1][i];
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            board[i][j] = temp[i][j];
        }
    }
}

int slideRow(int row[]) {
    int moved = 0;
    for (int i = 0; i < 4 - 1; i++) {
        if (row[i] == 0 && row[i + 1] != 0) {
            row[i] = row[i + 1];
            row[i + 1] = 0;
            moved = 1;
        }
    }
    return moved;
}

int mergeRow(int row[]) {
    int merged = 0;
    for (int i = 0; i < 4 - 1; i++) {
        if (row[i] == row[i + 1] && row[i] != 0) {
            row[i] *= 2;
            score += row[i];
            row[i + 1] = 0;
            merged = 1;
        }
    }
    return merged;
}

int moveLeft() {
    int moved = 0;
    for (int i = 0; i < 4; i++) {
        int merged = mergeRow(board[i]);
        int slid = 1;
        while (slid) {
            slid = slideRow(board[i]);
            moved |= slid;
        }
        moved |= merged;
    }
    return moved;
}

void move(char direction) {
    int rotations = 0;

    switch (direction) {
        case 'w': rotations = 3; break;
        case 's': rotations = 1; break;
        case 'a': rotations = 0; break;
        case 'd': rotations = 2; break;
        default: return;
    }

    for (int i = 0; i < rotations; i++)
        rotateBoard();

    if (moveLeft())
        addRandomTile();

    for (int i = 0; i < (4 - rotations) % 4; i++)
        rotateBoard();
}

int checkGameOver() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (board[i][j] == 0) return 0;
            if (j < 4 - 1 && board[i][j] == board[i][j + 1]) return 0;
            if (i < 4 - 1 && board[i][j] == board[i + 1][j]) return 0;
        }
    }
    return 1;
}



int main() {
    srand(time(NULL));
    printMainMenu();
    getch();
    initBoard();

    while (1) {
        printBoard();

        if (checkGameOver()) {
            printf("You lose.\n");
            break;
        }
        char input = getch(); 

        if (input == 'q') break; 

        move(input);
    }

    return 0;
}


