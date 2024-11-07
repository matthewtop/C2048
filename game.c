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

void printBanner() {
    system("clear"); // for windows use system("cls")
    printf("\033[0;32m");
    printf("  _______  ________  ___   ___  ________     \n");
    printf(" /  ___  \\|\\   __  \\|\\  \\ |\\  \\|\\   __  \\    \n");
    printf("/__/|_/  /\\ \\  \\|\\  \\ \\  \\\\_\\  \\ \\  \\|\\  \\   \n");
    printf("|__|//  / /\\ \\  \\\\\\  \\ \\______  \\ \\   __  \\  \n");
    printf("    /  /_/__\\ \\  \\\\\\  \\|_____|\\  \\ \\  \\ |\\ \\\n");
    printf("   |\\________\\ \\_______\\     \\ \\__\\ \\_______\\\n");
    printf("    \\|_______|\\|_______|      \\|__|\\|_______|      by matthewtop\n\n");
    printf("\033[0m");
}

void printMainMenu(){
    printBanner();
    printf("Use WSAD to move\n");
    printf("Press Q to quit\n");
    printf("Press any key to start\n");
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
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            board[i][j] = 0;
        }
    }
    addRandomTile();
    addRandomTile();
}

const char* colorNumber(int number){
    switch(number) {
        case 2: return "\033[0;34m";  // blue 
        case 4: return "\033[0;33m";  // yellow 
        case 8: return "\033[0;32m";  // green 
        case 16: return "\033[0;35m"; // magenta 
        case 32: return "\033[0;36m"; // cyan 
        case 64: return "\033[1;31m"; // red 
        case 128: return "\033[1;33m"; // lyellow 
        case 256: return "\033[1;34m"; // lblue 
        case 512: return "\033[1;35m"; // lmagenta 
        case 1024: return "\033[1;36m"; // lcyan 
        case 2048: return "\033[1;37m"; // lwhite 
        default: return "\033[0m";
    }
}

void printBoard() {
    system("clear"); // for windows use system("cls")
    printBanner();
    printf("+-------+-------+-------+-------+\n");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int value = board[i][j];
            if (value == 0) {
                printf("|       ");  
            } else {
                printf("|%s%*d\033[0m ", colorNumber(value), 5, value);  
            }
        }
        printf("|\n");
        printf("+-------+-------+-------+-------+\n");
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


