#include <stdbool.h>
#include <stdio.h>
#include <windows.h>
#include <ctype.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define BOARD_LENGTH 7
#define MAX_DEPTH 7

/**
            [0][1]  [0][2]  [0][3]  [0][4]  [0][5]  [0][6]  
[0][0]                                                         [1][0]
            [1][6]  [1][5]  [1][4]  [1][3]  [1][2]  [1][1]  

**/
void printBoard(char [2][BOARD_LENGTH]);
int getBestMoveValue(char actualBoard[2][BOARD_LENGTH], bool currentSide, char cup, int depth);

bool SHOW_EVAL = false;

bool start = true;

bool dropped[2][BOARD_LENGTH] = {{false}, {false}};

char capture = -1;

bool captureSide = false;

void clear_input(void){
    char junk;
    do{
        scanf("%c", &junk);
    }while (junk != '\n');
}

bool makeMove(char board[2][BOARD_LENGTH], bool side, int cup) {

    //Makes a single move
    //Returns false if the player gets to go again

    bool start = side;
    char marbles = board[side][cup];

    for(char i = 0; i < BOARD_LENGTH; i ++) {

        dropped[0][i] = false;
        dropped[1][i] = false;
    }
    capture = -1;

    board[side][cup] = 0;

    while(marbles) {

        //Switches sides if necessary
        if(cup == 6) {

            side = !side;
            cup = 0;

        //Otherwise, increments the current cup
        }else {

            cup ++;
        }
        
        //Drops a marble, unless the current cup is the other player's scoring area
        if(cup || side != start) {

            board[side][cup] ++;
            marbles --;
            dropped[side][cup] = true;

            Sleep(500);
            printBoard(board);
        }
    }

    //The player gets to go again if the last marble was dropped in their scoring area
    if(!cup) {

        return false;

    //The opposite cup is captured if the last marble landed in an empty cup
    }else if(board[side][cup] == 1 && board[!side][BOARD_LENGTH - cup] && side == start) {

        board[!start][0] += board[!side][BOARD_LENGTH - cup] + 1;

        board[side][cup] = 0;
        board[!side][BOARD_LENGTH - cup] = 0;

        capture = side ? BOARD_LENGTH - cup : cup;
        captureSide = side;
        
        printBoard(board);
    }

    //By default, returns true to indicate that the player's turn is over
    return true;
}

bool quietMove(char board[2][BOARD_LENGTH], bool side, int cup) {

   //Makes a single move
    //Returns false if the player gets to go again

    bool start = side;
    char marbles = board[side][cup];

    board[side][cup] = 0;

    while(marbles) {

        //Switches sides if necessary
        if(cup == 6) {

            side = !side;
            cup = 0;

        //Otherwise, increments the current cup
        }else {

            cup ++;
        }
        
        //Drops a marble, unless the current cup is the other player's scoring area
        if(cup || side != start) {

            board[side][cup] ++;
            marbles --;
        }
    }

    //The player gets to go again if the last marble was dropped in their scoring area
    if(!cup) {

        return false;

    //The opposite cup is captured if the last marble landed in an empty cup
    }else if(board[side][cup] == 1 && board[!side][BOARD_LENGTH - cup] && side == start) {

        board[!start][0] += board[!side][BOARD_LENGTH - cup] + 1;

        board[side][cup] = 0;
        board[!side][BOARD_LENGTH - cup] = 0;
    }

    //By default, returns true to indicate that the player's turn is over
    return true;
}

bool gameNotOver(char board[2][BOARD_LENGTH]) {

    //Checks if the game is over, and applies the appropriate scoring if it is
    //Returns false if the game has ended

    bool oneOut = true, twoOut = true;

    for(char i = 1; i < BOARD_LENGTH; i ++) {

        if(board[0][i]) {

            oneOut = false;
        }
        
        if(board[1][i]) {

            twoOut = false;
        }
    }

    if(oneOut || twoOut) {

        for(char i = 1; i < BOARD_LENGTH; i ++) {

            board[1][0] += board[0][i];
            board[0][0] += board[1][i];

            board[0][i] = 0;
            board[1][i] = 0;
        }

        return false;
    }

    return true;
}

void printBoard(char board[2][BOARD_LENGTH]) {

    system("cls");
    for(char i = 0; i < 72; i ++) printf("*");
    printf("\n\t\t\t\t  Player Two\n\n");
    printf("\t\t\t  1    2    3    4    5    6\n\t\t\t");

    for(char i = 1; i < BOARD_LENGTH; i ++) {

        if(capture == i) {

            printf(ANSI_COLOR_GREEN);
        }else if(dropped[0][i]) {

            printf(ANSI_COLOR_BLUE);
        }
        
        printf("[%2d] ", board[0][i]);
        printf(ANSI_COLOR_RESET);
    }
    printf("-> \nPlayer 1 Score - ");

    if(capture != -1 && captureSide) {

        printf(ANSI_COLOR_GREEN);
    }else if(dropped[0][0]) {

        printf(ANSI_COLOR_BLUE);
    }
    printf("[%2d]\t\t                        ", board[0][0]);
    printf(ANSI_COLOR_RESET);

    if(capture != -1 && !captureSide) {

        printf(ANSI_COLOR_GREEN);
    }else if(dropped[1][0]) {

        printf(ANSI_COLOR_BLUE);
    }
    printf("[%2d]", board[1][0]);
    printf(ANSI_COLOR_RESET);
    printf(" - Player 2 Score\n\t\t     <-\t");

    for(char i = 1; i < BOARD_LENGTH; i ++) {

        if(capture == i) {

            printf(ANSI_COLOR_GREEN);
        }else if(dropped[1][BOARD_LENGTH - i]) {

            printf(ANSI_COLOR_BLUE);
        }
        printf("[%2d] ", board[1][BOARD_LENGTH - i]);
        printf(ANSI_COLOR_RESET);
    }
    
    printf("\n\n\t\t\t\t  Player One\n");
    for(char i = 0; i < 72; i ++) printf("*");
}

int getMove(char board[2][BOARD_LENGTH], bool side) {

    int move = 0;

    while(!move) {
    
        printf("\nPlayer %d, Pick a move: ", 1 + !side);
        
        while(!scanf("%d", &move)) {

            printf("Not a valid move\n");
            clear_input();
            printf("\nPlayer %d, Pick a move: ", 1 + !side);
        }

        if(side) {

            move = BOARD_LENGTH - move;
        }

        if(move > 6 || move < 1 || !board[side][move]) {

            printf("Not a valid move\n");
            move = 0;
        }
    }

    return move;
}

void printWinner(char board[2][BOARD_LENGTH], int depth) {

    printBoard(board);
    printf("\n");
    if(board[1][0] == board[0][0]) {

        printf("Tied Game!");
    }else if(board[1][0] > board[0][0]) {

        printf("Player 2 Wins!");
    }else {

        printf("Player 1 Wins!");
    }
    if(depth != -1) printf("\nVs. Computer, Level %d", depth);
    printf("\nFinal Score: %d-%d\n", board[0][0], board[1][0]);
}

void printIntro(void) {

    for(char i = 0; i < 40; i ++) printf("*");
    printf("\n\n\n\t\tMANCALA\n\n");
    printf("\tPress Enter to start\n\n");    
    for(char i = 0; i < 40; i ++) printf("*");
    
    while(getchar() != '\n');
}

int pickGameMode(void) {

    int choice = 0;

    printf("\n\n\n1. Two Player\n2. Vs. Computer\n");
    
    do {

        printf("Select game mode:");
        if(!scanf("%d", &choice)) {

            clear_input();
            printf("\nInvalid input\n");
        }

        if(choice == 314) {
            
            SHOW_EVAL = true;
            printf("CODE ACCEPTED-SHOWING EVALUATION\n");
        }

    }while(choice != 1 && choice != 2);

    return choice;
}

int selectComputerStrength(void) {

    int choice = 0;

    printf("\n\n\n");

    do {

        printf("Select Computer Strength(0-%d):", MAX_DEPTH);
        if(!scanf("%d", &choice)) {

            clear_input();
            printf("\nInvalid input\n");
        }
    }while(choice < 0 || choice > MAX_DEPTH);

    return choice;
}

char calculateMove(char actualBoard[2][BOARD_LENGTH], bool currentSide, int depth) {

    int max = -200;
    char choice = 1;

    for(char i = 1; i < BOARD_LENGTH; i ++) {

        if(actualBoard[currentSide][i]) {
            char value = getBestMoveValue(actualBoard, currentSide, i, depth);

            if(value > max) {

                max = value;
                choice = i;
            }

            if(SHOW_EVAL) printf("Evaluation of %d: %d\n", i, value);
        }

    }

    if(SHOW_EVAL) {
        
        char c;
        clear_input();
        scanf("%c", &c);
    }

    while(!actualBoard[currentSide][choice]) {

        choice ++;
    }

    return choice;
}

int getBestMoveValue(char actualBoard[2][BOARD_LENGTH], bool currentSide, char cup, int depth) {


    char board[2][BOARD_LENGTH];

    for(char i = 0; i < BOARD_LENGTH; i ++) {

        board[0][i] = actualBoard[0][i];
        board[1][i] = actualBoard[1][i];
    }

    char startValue = board[!currentSide][0];

    bool pass = quietMove(board, currentSide, cup);
    bool over = !gameNotOver(board);

    int evaluation = board[!currentSide][0] - startValue;
    if(board[!currentSide][0] > 24) {

        return 100 + evaluation;
    }else if(board[currentSide][0] > 24) {

        return evaluation - 100;
    }

    if(pass) {

        if(depth) {

            int bestDefense = -200;

            for(char i = 1; i < BOARD_LENGTH; i ++) {
                
                if(board[!currentSide][i]) {
                    int value = getBestMoveValue(board, !currentSide, i, depth - 1);

                    if(value > bestDefense) {

                        bestDefense = value;
                    }
                }
            }

            evaluation -= bestDefense;
        }

        return evaluation;
    }else {

        int maxValue = -200;
        for(char i = 1; i < BOARD_LENGTH; i ++) {
            
            if(board[currentSide][i]) {

                char currentValue = getBestMoveValue(board, currentSide, i, depth);
                if(currentValue > maxValue) {

                    maxValue = currentValue;
                }
            }
        }

        return maxValue + evaluation;
    }
}

int main(void) {

    printIntro();

    for(;;) {

        char board[2][BOARD_LENGTH] = {{0, 4, 4, 4, 4, 4, 4}, {0, 4, 4, 4, 4, 4, 4}}; 

        bool currentSide = start;
        
        char mode = pickGameMode();

        int depth;
        
        if(mode == 2) {
        
            depth = selectComputerStrength();
        }

        printf("\n\n\n");
        printBoard(board);

        while(gameNotOver(board)) {
            
            if(mode == 2 && !currentSide) {
                
                Sleep(500);
                printf("\nComputer's Move...\n");
                
                if(depth <= 6 && !SHOW_EVAL) {
                    Sleep(2000);
                }

                int move = calculateMove(board, currentSide, depth);

                if(makeMove(board, currentSide, move)) {
                    
                    currentSide = true;
                }
            }else if(makeMove(board, currentSide, getMove(board, currentSide))) {
                
                currentSide = !currentSide;
            }
        }

        printWinner(board, mode == 2 ? depth : -1);

        char playAgain = 0;
        
        while(playAgain != 'y' && playAgain != 'n') {
            
            clear_input();
            printf("Play Again? (y/n)\n");
            scanf("%c", &playAgain);
            playAgain = tolower(playAgain);
        }

        if(playAgain == 'n') {

            return 0;
        }

        for(int i = 0; i < 7; i ++) {

            dropped[0][i] = false;
            dropped[1][i] = false;
        }
        capture = -1;
        start = !start;
    }
    return 0;
}