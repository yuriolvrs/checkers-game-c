/******
This is to certify that this project is my own work, based on my
personal efforts in studying and applying the concepts learned.
I have constructed the functions and their respective algorithms
and corresponding code by myself.
The program was run, tested, and debugged by my own efforts.
I further certify that I have not copied in part or whole or 
otherwise plagiarized the work of other students and/or persons.
Olivares, Marc Yuri Gonzaga, DLSU ID# 12264131
******/

/*
Description: This program runs a simplified version of Checkers in C
Programmed by: Olivares, Marc Yuri Gonzaga S20B
Last modified: December 4, 2022
Version: 12.4
Acknowledgements:
https://en.cppreference.com/w/c/numeric/math
https://manual.cs50.io/
https://www.tutorialspoint.com/index.htm
https://www.programiz.com/
https://stackoverflow.com/
https://cboard.cprogramming.com/
https://www.geeksforgeeks.org/
https://beginnersbook.com/
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define DARK "DARK"
#define LIGHT "LIGHT"
#define SPACE '-'
#define TRUE 1

// Function Prototypes
void empty_stdin(void);
char resetPieces(char sPiece[9][9]);
int displayMenu();
int displayFirst();
void displayBoard(char sPiece[9][9]);
void askMove(int *noldX, int *noldY, int *nNewX, int *nNewY, int nCurrentPlayer, char sCurrentColor[]);
int checkMove(int *noldX, int *noldY, int *nNewX, int *nNewY, char sPiece[9][9], int nCurrentPlayer, int otherPlayer, char sCurrentColor[], char sOtherColor[]);
void checkMulti(int *noldX, int *noldY, int *nNewX, int *nNewY, char sPiece[9][9], char sCurrentColor[]);
void updateTurn(int *nCurrentPlayer, char sCurrentColor[], int *nOtherPlayer, char sOtherColor[]);
void updatePieceCount(char sPiece[9][9], int *nDarkCount, int *nLightCount);
void doSurrender(int nCurrentPlayer, int *nP1Score, int *nP2Score, int *nIsPlaying);
void displayDivider();
void checkWin(char sPiece[9][9], int nDarkPlayer, int nLightPlayer, int *nIsPlaying, int *nP1Score, int *nP2Score, int nDarkCount, int nLightCount);
void checkReachEndRow(char sPiece[9][9], int nDarkPlayer, int nLightPlayer, int *nIsPlaying, int *nP1Score, int *nP2Score);
void checkAllCaptured(char sPiece[9][9], int nDarkPlayer, int nLightPlayer, int *nIsPlaying, int *nP1Score, int *nP2Score, int nDarkCount, int nLightCount);
void checkPossibleMoves(char sPiece[9][9], int nDarkPlayer, int nLightPlayer, int *nIsPlaying, int *nP1Score, int *nP2Score, int nDarkCount, int nLightCount);
void displayScores(int nP1Score, int nP2Score);
int displayQuit();

int main() {
    char sPiece[9][9];
    int nMenuChoice;
    int nIsPlaying;
    int nOldX, nOldY, nNewX, nNewY;
    int nDarkPlayer, nLightPlayer, nCurrentPlayer, nOtherPlayer;
    char sCurrentColor[6], sOtherColor[6];
    int nIsLegal;
    int nDarkCount, nLightCount;
    int nP1Score = 0;
    int nP2Score = 0;

    do {
        resetPieces(sPiece);
        nMenuChoice = displayMenu();
        displayDivider();

        switch (nMenuChoice) {
            case 1:
                nIsPlaying = 1;
                nCurrentPlayer = displayFirst();
                if (nCurrentPlayer == 1) {
                    nOtherPlayer = 2;
                } else {
                    nOtherPlayer = 1;
                }
                strcpy(sCurrentColor, DARK);
                strcpy(sOtherColor, LIGHT);
                nDarkPlayer = nCurrentPlayer;
                if (nDarkPlayer == 1) {
                    nLightPlayer = 2;
                } else {
                    nLightPlayer = 1;
                }

                printf("%37s%d\n", "Dark pieces go to Player ", nDarkPlayer);
                printf("%37s%d\n", "Light pieces go to Player ", nLightPlayer);
                printf("%14s%d (%s) makes the first move\n", "Player ", nCurrentPlayer, sCurrentColor);
                displayDivider();

                while (nIsPlaying == 1) {
                    do {
                        displayBoard(sPiece);
                        updatePieceCount(sPiece, &nDarkCount, &nLightCount);
                        displayDivider();
                        printf("%21s%d (%s)'s TURN\n", "PLAYER ", nCurrentPlayer, sCurrentColor);
                        displayDivider();
                        askMove(&nOldX, &nOldY, &nNewX, &nNewY, nCurrentPlayer, sCurrentColor);
                        displayDivider();
                        nIsLegal = checkMove(&nOldX, &nOldY, &nNewX, &nNewY, sPiece, nCurrentPlayer, nOtherPlayer, sCurrentColor, sOtherColor);
                        displayDivider();
                    } while (nIsLegal == 0);

                    if (nIsLegal == 9) {
                        doSurrender(nCurrentPlayer, &nP1Score, &nP2Score, &nIsPlaying);
                    }

                    checkWin(sPiece, nDarkPlayer, nLightPlayer, &nIsPlaying, &nP1Score, &nP2Score, nDarkCount, nLightCount);

                    if (nIsPlaying == 1) {
                        updateTurn(&nCurrentPlayer, sCurrentColor, &nOtherPlayer, sOtherColor);
                    }
                }
                break;
            case 2:
                displayScores(nP1Score, nP2Score);
                break;
            case 3:
                nMenuChoice = displayQuit();
                break;
            default:
                printf("ERROR: invalid input\n");
        }
        displayDivider();
    } while (nMenuChoice != 3);

    return 0;
}

void empty_stdin(void) {
    int c = getchar();
    while (c != '\n' && c != EOF)
        c = getchar();
}

char resetPieces(char sPiece[9][9]) {
    int i = 1;
    int j;
    while (i <= 8) {
        if (i % 2 != 0) { // Odd rows
            for (j = 2; j < 9; j += 2) {
                if (i == 1 || i == 3) sPiece[j][i] = 'D';
                else if (i == 5) sPiece[j][i] = SPACE;
                else if (i == 7) sPiece[j][i] = 'L';
            }
        } else { // Even rows
            for (j = 1; j < 8; j += 2) {
                if (i == 2) sPiece[j][i] = 'D';
                else if (i == 4) sPiece[j][i] = SPACE;
                else if (i == 6 || i == 8) sPiece[j][i] = 'L';
            }
        }
        i++;
    }
    return sPiece[9][9];
}

int displayMenu() {
    int nChoice;
    int nMenuFlag = 0;
    int rtn;
    printf("\n%35s", "WELCOME TO CHECKERS!\n\n");
    printf("%29s", "1. Play\n");
    printf("%32s", "2. View Scores\n");
    printf("%29s", "3. Quit\n");
    while (nMenuFlag == 0) {
        printf("\nPick a menu number option: ");
        rtn = scanf("%d", &nChoice);
        if (rtn == 0) {
            printf("ERROR: invalid input\n");
            empty_stdin();
        } else {
            nMenuFlag = 1;
        }
    }
    printf("\n");
    return nChoice;
}

int displayFirst() {
    srand(time(NULL));
    return rand() % 2 + 1;
}

void displayBoard(char sPiece[9][9]) {
    int i = 1, j;
    printf("\n%35s\n", "1 2 3 4 5 6 7 8  X");
    while (i <= 8) {
        printf("%15s%d", "", i);
        if (i % 2 != 0) {
            for (j = 2; j < 9; j += 2) printf("| |%c", sPiece[j][i]);
        } else {
            for (j = 1; j < 8; j += 2) printf("|%c| ", sPiece[j][i]);
        }
        printf("|\n");
        i++;
    }
    printf("%17s", "Y\n");
}

void askMove(int *noldX, int *noldY, int *nNewX, int *nNewY, int nCurrentPlayer, char sCurrentColor[]) {
    int nMoveFlag = 0, rtn;
    printf("%31s", "MAKE A MOVE!\n");
    printf("\nFormat: OldX OldY NewX NewY\nExample move: 4354\n");
    printf("Input 9999 to surrender\n");
    while (nMoveFlag == 0) {
        printf("\nPlayer %d (%s)'s move: ", nCurrentPlayer, sCurrentColor);
        rtn = scanf("%1d%1d%1d%1d", noldX, noldY, nNewX, nNewY);
        if (rtn != 4) {
            printf("ERROR: invalid integer input\n");
            empty_stdin();
        } else {
            empty_stdin();
            nMoveFlag = 1;
        }
    }
}

int checkMove(int *noldX, int *noldY, int *nNewX, int *nNewY, char sPiece[9][9], int nCurrentPlayer, int notherPlayer, char sCurrentColor[], char sOtherColor[]) {
    char cSurr;
    if (*noldX == 9 && *noldY == 9 && *nNewX == 9 && *nNewY == 9) {
        printf("%22s%d SURRENDER (Y/N): ", "CONFIRM PLAYER ", nCurrentPlayer);
        scanf(" %c", &cSurr);
        if (cSurr == 'Y') return 9;
        else return 0;
    }

    if (sPiece[*nNewX][*nNewY] == SPACE) {
        if (strcmp(sCurrentColor, DARK) == 0) {
            if (sPiece[*noldX][*noldY] == 'D') {
                // Regular Move
                if ((*nNewX == (*noldX - 1) || *nNewX == (*noldX + 1)) && *nNewY == (*noldY + 1)) {
                    printf("Player %d moved the piece in (%d, %d) to (%d, %d)\n", nCurrentPlayer, *noldX, *noldY, *nNewX, *nNewY);
                    sPiece[*noldX][*noldY] = SPACE;
                    sPiece[*nNewX][*nNewY] = 'D';
                    return 1;
                } 
                // Capture Left
                else if ((*nNewX == (*noldX - 2) && *nNewY == (*noldY + 2)) && sPiece[*nNewX + 1][*nNewY - 1] == 'L') {
                    printf("Player %d moved the %s piece in (%d, %d) to (%d, %d)\n", nCurrentPlayer, sCurrentColor, *noldX, *noldY, *nNewX, *nNewY);
                    printf("Player %d captured Player %d's %s piece in (%d, %d)\n", nCurrentPlayer, notherPlayer, sOtherColor, *nNewX + 1, *nNewY - 1);
                    sPiece[*noldX][*noldY] = SPACE;
                    sPiece[*nNewX][*nNewY] = 'D';
                    sPiece[*nNewX + 1][*nNewY - 1] = SPACE;
                    checkMulti(noldX, noldY, nNewX, nNewY, sPiece, sCurrentColor);
                    return 1;
                }
                // Capture Right
                else if ((*nNewX == (*noldX + 2) && *nNewY == (*noldY + 2)) && sPiece[*nNewX - 1][*nNewY - 1] == 'L') {
                    printf("Player %d moved the %s piece in (%d, %d) to (%d, %d)\n", nCurrentPlayer, sCurrentColor, *noldX, *noldY, *nNewX, *nNewY);
                    printf("Player %d captured Player %d's %s piece in (%d, %d)\n", nCurrentPlayer, notherPlayer, sOtherColor, *nNewX - 1, *nNewY - 1);
                    sPiece[*noldX][*noldY] = SPACE;
                    sPiece[*nNewX][*nNewY] = 'D';
                    sPiece[*nNewX - 1][*nNewY - 1] = SPACE;
                    checkMulti(noldX, noldY, nNewX, nNewY, sPiece, sCurrentColor);
                    return 1;
                }
            }
        } else if (strcmp(sCurrentColor, LIGHT) == 0) {
            if (sPiece[*noldX][*noldY] == 'L') {
                if ((*nNewX == (*noldX - 1) || *nNewX == (*noldX + 1)) && *nNewY == (*noldY - 1)) {
                    printf("Player %d moved the piece in (%d, %d) to (%d, %d)\n", nCurrentPlayer, *noldX, *noldY, *nNewX, *nNewY);
                    sPiece[*noldX][*noldY] = SPACE;
                    sPiece[*nNewX][*nNewY] = 'L';
                    return 1;
                }
                else if ((*nNewX == (*noldX - 2) && *nNewY == (*noldY - 2)) && sPiece[*nNewX + 1][*nNewY + 1] == 'D') {
                    printf("Player %d moved the %s piece in (%d, %d) to (%d, %d)\n", nCurrentPlayer, sCurrentColor, *noldX, *noldY, *nNewX, *nNewY);
                    printf("Player %d captured Player %d's %s piece in (%d, %d)\n", nCurrentPlayer, notherPlayer, sOtherColor, *nNewX + 1, *nNewY + 1);
                    sPiece[*noldX][*noldY] = SPACE;
                    sPiece[*nNewX][*nNewY] = 'L';
                    sPiece[*nNewX + 1][*nNewY + 1] = SPACE;
                    checkMulti(noldX, noldY, nNewX, nNewY, sPiece, sCurrentColor);
                    return 1;
                }
                else if ((*nNewX == (*noldX + 2) && *nNewY == (*noldY - 2)) && sPiece[*nNewX - 1][*nNewY + 1] == 'D') {
                    printf("Player %d moved the %s piece in (%d, %d) to (%d, %d)\n", nCurrentPlayer, sCurrentColor, *noldX, *noldY, *nNewX, *nNewY);
                    printf("Player %d captured Player %d's %s piece in (%d, %d)\n", nCurrentPlayer, notherPlayer, sOtherColor, *nNewX - 1, *nNewY + 1);
                    sPiece[*noldX][*noldY] = SPACE;
                    sPiece[*nNewX][*nNewY] = 'L';
                    sPiece[*nNewX - 1][*nNewY + 1] = SPACE;
                    checkMulti(noldX, noldY, nNewX, nNewY, sPiece, sCurrentColor);
                    return 1;
                }
            }
        }
    }
    printf("ERROR: invalid move.\n");
    return 0;
}

void checkMulti(int *nOldX, int *nOldY, int *nNewX, int *nNewY, char sPiece[9][9], char sCurrentColor[]) {
    int nMultiX, nMultiY, nMultiFlag = 1;
    char cMultiCurrent, cMultiOther;

    if (strcmp(sCurrentColor, DARK) == 0) {
        cMultiCurrent = 'D'; cMultiOther = 'L';
        while (nMultiFlag == 1) {
            if ((*nNewX - 2 > 0 && *nNewY + 2 < 9) && sPiece[*nNewX - 2][*nNewY + 2] == SPACE && sPiece[*nNewX - 1][*nNewY + 1] == cMultiOther) {
                nMultiX = *nNewX - 2; nMultiY = *nNewY + 2;
                sPiece[*nNewX][*nNewY] = SPACE;
                sPiece[nMultiX][nMultiY] = cMultiCurrent;
                sPiece[*nNewX - 1][*nNewY + 1] = SPACE;
                printf("\nMulti-capture to (%d, %d) automated.\n", nMultiX, nMultiY);
                *nNewX = nMultiX; *nNewY = nMultiY;
            } else if ((*nNewX + 2 < 9 && *nNewY + 2 < 9) && sPiece[*nNewX + 2][*nNewY + 2] == SPACE && sPiece[*nNewX + 1][*nNewY + 1] == cMultiOther) {
                nMultiX = *nNewX + 2; nMultiY = *nNewY + 2;
                sPiece[*nNewX][*nNewY] = SPACE;
                sPiece[nMultiX][nMultiY] = cMultiCurrent;
                sPiece[*nNewX + 1][*nNewY + 1] = SPACE;
                printf("\nMulti-capture to (%d, %d) automated.\n", nMultiX, nMultiY);
                *nNewX = nMultiX; *nNewY = nMultiY;
            } else nMultiFlag = 0;
        }
    } else {
        cMultiCurrent = 'L'; cMultiOther = 'D';
        while (nMultiFlag == 1) {
            if ((*nNewX - 2 > 0 && *nNewY - 2 > 0) && sPiece[*nNewX - 2][*nNewY - 2] == SPACE && sPiece[*nNewX - 1][*nNewY - 1] == cMultiOther) {
                nMultiX = *nNewX - 2; nMultiY = *nNewY - 2;
                sPiece[*nNewX][*nNewY] = SPACE;
                sPiece[nMultiX][nMultiY] = cMultiCurrent;
                sPiece[*nNewX - 1][*nNewY - 1] = SPACE;
                printf("\nMulti-capture to (%d, %d) automated.\n", nMultiX, nMultiY);
                *nNewX = nMultiX; *nNewY = nMultiY;
            } else if ((*nNewX + 2 < 9 && *nNewY - 2 > 0) && sPiece[*nNewX + 2][*nNewY - 2] == SPACE && sPiece[*nNewX + 1][*nNewY - 1] == cMultiOther) {
                nMultiX = *nNewX + 2; nMultiY = *nNewY - 2;
                sPiece[*nNewX][*nNewY] = SPACE;
                sPiece[nMultiX][nMultiY] = cMultiCurrent;
                sPiece[*nNewX + 1][*nNewY - 1] = SPACE;
                printf("\nMulti-capture to (%d, %d) automated.\n", nMultiX, nMultiY);
                *nNewX = nMultiX; *nNewY = nMultiY;
            } else nMultiFlag = 0;
        }
    }
}

void updateTurn(int *nCurrentPlayer, char sCurrentColor[], int *nOtherPlayer, char sOtherColor[]) {
    if (*nCurrentPlayer == 1) {
        *nCurrentPlayer = 2; *nOtherPlayer = 1;
    } else {
        *nCurrentPlayer = 1; *nOtherPlayer = 2;
    }
    if (strcmp(sCurrentColor, DARK) == 0) {
        strcpy(sCurrentColor, LIGHT); strcpy(sOtherColor, DARK);
    } else {
        strcpy(sCurrentColor, DARK); strcpy(sOtherColor, LIGHT);
    }
}

void updatePieceCount(char sPiece[9][9], int *nDarkCount, int *nLightCount) {
    int i, j;
    *nDarkCount = 0; *nLightCount = 0;
    for (i = 1; i <= 8; i++) {
        for (j = 1; j <= 8; j++) {
            if (sPiece[j][i] == 'D') (*nDarkCount)++;
            else if (sPiece[j][i] == 'L') (*nLightCount)++;
        }
    }
    printf("\nDark Pieces left: %d\nLight Pieces left: %d\n", *nDarkCount, *nLightCount);
}

void doSurrender(int nCurrentPlayer, int *nP1Score, int *nP2Score, int *nIsPlaying) {
    if (nCurrentPlayer == 1) {
        (*nP2Score)++;
        printf("\n%34s\n%32s\n", "Player 1 surrenders", "Player 2 wins!");
    } else {
        (*nP1Score)++;
        printf("\n%34s\n%32s\n", "Player 2 surrenders", "Player 1 wins!");
    }
    displayScores(*nP1Score, *nP2Score);
    *nIsPlaying = 0;
}

void displayDivider() {
    printf("--------------------------------------------------\n");
}

void checkWin(char sPiece[9][9], int nDarkPlayer, int nLightPlayer, int *nIsPlaying, int *nP1Score, int *nP2Score, int nDarkCount, int nLightCount) {
    checkReachEndRow(sPiece, nDarkPlayer, nLightPlayer, nIsPlaying, nP1Score, nP2Score);
    if (*nIsPlaying) checkAllCaptured(sPiece, nDarkPlayer, nLightPlayer, nIsPlaying, nP1Score, nP2Score, nDarkCount, nLightCount);
    if (*nIsPlaying) checkPossibleMoves(sPiece, nDarkPlayer, nLightPlayer, nIsPlaying, nP1Score, nP2Score, nDarkCount, nLightCount);
}

void checkReachEndRow(char sPiece[9][9], int nDarkPlayer, int nLightPlayer, int *nIsPlaying, int *nP1Score, int *nP2Score) {
    int j;
    for (j = 2; j < 9; j += 2) {
        if (sPiece[j][1] == 'L') {
            displayBoard(sPiece);
            printf("\nPlayer %d got a LIGHT piece to reach the opposite end row!\n", nLightPlayer);
            if (nLightPlayer == 1) (*nP1Score)++; else (*nP2Score)++;
            displayScores(*nP1Score, *nP2Score); *nIsPlaying = 0; return;
        }
    }
    for (j = 1; j < 8; j += 2) {
        if (sPiece[j][8] == 'D') {
            displayBoard(sPiece);
            printf("\nPlayer %d got a DARK piece to reach the opposite end row!\n", nDarkPlayer);
            if (nDarkPlayer == 1) (*nP1Score)++; else (*nP2Score)++;
            displayScores(*nP1Score, *nP2Score); *nIsPlaying = 0; return;
        }
    }
}

void checkAllCaptured(char sPiece[9][9], int nDarkPlayer, int nLightPlayer, int *nIsPlaying, int *nP1Score, int *nP2Score, int nDarkCount, int nLightCount) {
    if (nDarkCount == 0) {
        displayBoard(sPiece);
        printf("\nPlayer %d (LIGHT) captured all of Player %d's DARK pieces!\n", nLightPlayer, nDarkPlayer);
        if (nLightPlayer == 1) (*nP1Score)++; else (*nP2Score)++;
        displayScores(*nP1Score, *nP2Score); *nIsPlaying = 0;
    } else if (nLightCount == 0) {
        displayBoard(sPiece);
        printf("\nPlayer %d (DARK) captured all of Player %d's LIGHT pieces!\n", nDarkPlayer, nLightPlayer);
        if (nDarkPlayer == 1) (*nP1Score)++; else (*nP2Score)++;
        displayScores(*nP1Score, *nP2Score); *nIsPlaying = 0;
    }
}

void checkPossibleMoves(char sPiece[9][9], int nDarkPlayer, int nLightPlayer, int *nIsPlaying, int *nP1Score, int *nP2Score, int nDarkCount, int nLightCount) {
    // Simplified check logic based on code provided
    int i, j, nDarkPossible = nDarkCount, nLightPossible = nLightCount;
    for (i = 1; i <= 8; i++) {
        for (j = 1; j <= 8; j++) {
            if (sPiece[j][i] == 'D') {
                if (i+1 > 8 || ((j-1 < 1 || sPiece[j-1][i+1] != SPACE) && (j+1 > 8 || sPiece[j+1][i+1] != SPACE))) nDarkPossible--;
            }
            if (sPiece[j][i] == 'L') {
                if (i-1 < 1 || ((j-1 < 1 || sPiece[j-1][i-1] != SPACE) && (j+1 > 8 || sPiece[j+1][i-1] != SPACE))) nLightPossible--;
            }
        }
    }
    if (nDarkPossible <= 0) {
        printf("\nPlayer %d (DARK) has no more possible moves!\n", nDarkPlayer);
        if (nLightPlayer == 1) (*nP1Score)++; else (*nP2Score)++;
        *nIsPlaying = 0;
    } else if (nLightPossible <= 0) {
        printf("\nPlayer %d (LIGHT) has no more possible moves!\n", nLightPlayer);
        if (nDarkPlayer == 1) (*nP1Score)++; else (*nP2Score)++;
        *nIsPlaying = 0;
    }
}

void displayScores(int nP1Score, int nP2Score) {
    printf("%28s\n", "SCORES");
    printf("%19sPlayer 1: %d\n%19sPlayer 2: %d\n", " ", nP1Score, " ", nP2Score);
    displayDivider();
}

int displayQuit() {
    char cQuit;
    printf("\n%49s\n", "ARE YOU SURE YOU WANT TO QUIT AND RESET SCORES?");
    printf("CONFIRM (Y/N): ");
    scanf(" %c", &cQuit);
    if (cQuit == 'Y') return 3;
    return 0;
}