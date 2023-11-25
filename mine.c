#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 5
#define MINES 5

struct Cell {
    char value;  // '-': unrevealed, ' ': revealed, '*': mine, '0'-'8': adjacent mines
    int isMine;
    int isRevealed;
    struct Cell* next;
};

struct Cell* gridHead = NULL;

struct Cell* createCell(char value, int isMine) {
    struct Cell* newCell = (struct Cell*)malloc(sizeof(struct Cell));
    if (newCell == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    newCell->value = value;
    newCell->isMine = isMine;
    newCell->isRevealed = 0; // Initially, cells are not revealed
    newCell->next = NULL;
    return newCell;
}

void insertCell(struct Cell** head, char value, int isMine) {
    struct Cell* newCell = createCell(value, isMine);
    if (*head == NULL) {
        *head = newCell;
    } else {
        struct Cell* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newCell;
    }
}

void initializeGrid() {
    for (int i = 0; i < SIZE * SIZE; i++) {
        insertCell(&gridHead, '-', 0);
    }
}

void placeMines() {
    int count = 0;
    while (count < MINES) {
        int position = rand() % (SIZE * SIZE);
        struct Cell* current = gridHead;
        for (int i = 0; i < position; i++) {
            current = current->next;
        }
        if (current->value != '*') {
            current->value = '*';
            current->isMine = 1;
            count++;
        }
    }
}

void printGrid() {
    printf("   ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", i);
    }
    printf("\n");
    struct Cell* current = gridHead;
    for (int i = 0; i < SIZE; i++) {
        printf("%d  ", i);
        for (int j = 0; j < SIZE; j++) {
            if (current->isRevealed) {
                printf("%c ", current->value);
            } else {
                printf("- ");
            }
            current = current->next;
        }
        printf("\n");
    }
}

int countAdjacentMines(int row, int col) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int currentRow = row + i;
            int currentCol = col + j;
            if (currentRow >= 0 && currentRow < SIZE && currentCol >= 0 && currentCol < SIZE) {
                struct Cell* current = gridHead;
                for (int k = 0; k < currentRow * SIZE + currentCol; k++) {
                    current = current->next;
                }
                if (current->isMine) {
                    count++;
                }
            }
        }
    }
    return count;
}

void revealGrid(int row, int col) {
    struct Cell* current = gridHead;
    for (int i = 0; i < row * SIZE + col; i++) {
        current = current->next;
    }

    if (!current->isRevealed) {
        current->isRevealed = 1;
        if (current->value == '-') {
            int count = countAdjacentMines(row, col);
            if (count == 0) {
                current->value = ' ';
                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {
                        int currentRow = row + i;
                        int currentCol = col + j;
                        if (currentRow >= 0 && currentRow < SIZE && currentCol >= 0 && currentCol < SIZE) {
                            struct Cell* adjCell = gridHead;
                            for (int k = 0; k < currentRow * SIZE + currentCol; k++) {
                                adjCell = adjCell->next;
                            }
                            revealGrid(currentRow, currentCol);
                        }
                    }
                }
            } else {
                current->value = count + '0'; // Convert count to character
            }
        }
    }
}

int checkGameEnd() {
    struct Cell* current = gridHead;
    int revealedCells = 0;
    while (current != NULL) {
        if (current->isRevealed && !current->isMine) {
            revealedCells++;
        } else if (current->isRevealed && current->isMine) {
            printf("Oops! You hit a mine. Game Over!\n");
            return -1; // Game lost
        }
        current = current->next;
    }
    if (revealedCells == (SIZE * SIZE - MINES)) {
        printf("Congratulations! You've won!\n");
        return 1; // Game won
    }
    return 0; // Game is still in progress
}

int main() {
    srand(time(0));

    initializeGrid();
    placeMines();

    int row, col;
    int gameStatus = 0; // To track game status

    printf("Welcome to Minesweeper!\n");
    printf("Enter coordinates to reveal the cell (row column)\n");
    printf("Caution if you hit the mine you loose the game\n");

    while (gameStatus == 0) {
        printGrid();
        printf("Enter row and column: ");
        scanf("%d %d", &row, &col);

        if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) {
            printf("Invalid input! Please enter valid coordinates.\n");
            continue;
        }

        struct Cell* current = gridHead;
        int cellNumber = row * SIZE + col;
        for (int i = 0; i < cellNumber; i++) {
            current = current->next;
        }

        if (current->isRevealed) {
            printf("Cell already revealed! Choose another cell.\n");
            continue;
        }

        revealGrid(row, col);
        gameStatus = checkGameEnd();
    }

    return 0;
}
