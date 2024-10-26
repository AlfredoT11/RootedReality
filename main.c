#include <stdlib.h>
#include <stdio.h>

#include "raylib.h"

int TICKS_TO_GROW = 20;
int CURRENT_TICKS = 0;

typedef struct BranchNode {
    int x;               // X value
    int y;               // Y value
    struct BranchNode* next;   // Pointer to the next node
};
typedef struct BranchNode BranchNode;

typedef struct BrancheBranchNode{
    BranchNode *branch;
    struct BrancheBranchNode *next;
};
typedef struct BrancheBranchNode BrancheBranchNode;



struct PlantBranch{
    BranchNode branchStart;
    bool isGrowing;
};
typedef struct PlantBranch PlantBranch;

struct Plant {
    
};
typedef struct Plant Plant;

void processCells(int **grid, int rows, int columns);
bool hasSpaceForSand(int** grid, int row, int column, int rows);
void setCell(int** grid, int row, int column, int value);
void processPlant(int **grid, int rows, int columns, Plant plant);

// Branch management
void growBranch(PlantBranch *plantBranch);
void drawBranch(int** grid, int rows, int columns, BranchNode* head);

// Function to generate random values for x and y
void generateRandomValues(int* x, int* y) {
    *x = rand() % 100; // Random value between 0 and 99
    *y = rand() % 100; // Random value between 0 and 99
}

// Function to create a new BranchNode
BranchNode* createBranchNode(int x, int y) {
    BranchNode* newNode = (BranchNode*)malloc(sizeof(BranchNode));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }
    newNode->x = x;
    newNode->y = y;
    newNode->next = NULL;
    return newNode;
}

BranchNode* getLastBranchNode(BranchNode* head) {
    if (head == NULL) {
        return NULL; // Return NULL if the list is empty
    }

    BranchNode* current = head;
    while (current->next != NULL) {
        current = current->next; // Move to the next node
    }
    return current; // Return the last node
}

// Function to add a BranchNode to the end of the linked list
void addBranchNode(BranchNode** head, int x, int y) {
    
    if(CURRENT_TICKS % TICKS_TO_GROW != 0){
        return;
    }
    
    // Generate random direction for the new node based on the last node
    int newX, newY;
    
    if (*head == NULL) {
        // If the list is empty, create the first node with given values
        *head = createBranchNode(x, y);
        return;
    }
    
    // Get the last node in the list
    BranchNode* lastNode = getLastBranchNode(*head);

    // Generate new x coordinate based on a random direction
    int xDirection = rand() % 3;
    if (xDirection == 0) {
        newX = lastNode->x - 1;
    } else if (xDirection == 1) {
        newX = lastNode->x;
    } else {
        newX = lastNode->x + 1;
    }
    
    // Generate new y coordinate, either the same or one unit up
    newY = (rand() % 2) ? lastNode->y : lastNode->y - 1;

    // Create a new node with the generated coordinates
    BranchNode* newNode = createBranchNode(newX, newY);

    // Attach the new node at the end of the list
    lastNode->next = newNode;
}

void drawBranch(int** grid, int rows, int columns, BranchNode* head) {
    BranchNode* current = head;
    while (current != NULL) {
        // printf("BranchNode: (x: %d, y: %d)\n", current->x, current->y);
        DrawRectangle(current->x * 10, current->y * 10, 10, 10, GREEN);
        current = current->next; // Move to the next node
    }
}




//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    
    srand((unsigned int)time(NULL));
    //--------------------------------------------------------------------------------------
    
    
    // Setting up initial cellular automata
    int rows = 40;
    int columns = 50;
    int** grid = (int**)malloc(rows * sizeof(int*));
    for(int i = 0; i < rows; i++){
        grid[i] = (int*)malloc(columns * sizeof(int));
    }
    
    /*
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            grid[i][j] = rand() % 2;
        }
    }
    */
    
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            grid[i][j] = 0;
        }
    }
    
    
    /*
    grid[1][2] = true;
    grid[2][3] = true;
    grid[3][1] = true;
    grid[3][2] = true;
    grid[3][3] = true;
    */
    
    
    /*
    grid[2][2] = true;
    grid[2][3] = true;
    grid[3][2] = true;
    grid[3][3] = true;
    */
    
    BranchNode* branchNodeList = createBranchNode(columns/2, rows-1); // Head for the first linked list


    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        int newSandPosition = 10 + rand() % (20 - 10 + 1);
        // int newSandPosition = columns/2;
        
        setCell(grid, 0, newSandPosition, 1);
        
        //updateGrid(grid, rows, columns);
        processCells(grid, rows, columns);
        
        // Generating values for branch
        addBranchNode(&branchNodeList, 0, 0);
        
        CURRENT_TICKS++;
        if(CURRENT_TICKS > 1000){
            CURRENT_TICKS = 0;
        }
        
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            //DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
          
           
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < columns; j++) {
                    if(grid[i][j] == 1){
                        DrawRectangle(j * 10, i * 10, 10, 10, BROWN);
                    }else if(grid[i][j] == 0){
                        DrawRectangle(j * 10, i * 10, 10, 10, WHITE);
                    }
                }
            }
            
            //DrawRectangle(newSandPosition * 10, 0, 10, 10, BROWN);
            drawBranch(grid, rows, columns, branchNodeList);
                        
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
// Code related to cellular automata

// Function to count the number of live neighbors of a cell
int countLiveNeighbors(bool** grid, int rows, int columns, int row, int col) {
    
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) {
                continue; // Skip the cell itself
            }
            int newRow = row + i;
            int newCol = col + j;
            // Check if the neighbor is within bounds
            if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < columns) {
                count += (int)grid[newRow][newCol];
            }
        }
    }
    return count;
}


// Function to update the grid to the next generation
void updateGrid(bool **grid, int rows, int columns) {
    
    // Assigning memory to new grid
    bool **newGrid = (bool**)malloc(rows * sizeof(bool*));
    for(int i = 0; i < rows; i++){
        newGrid[i] = (bool*)malloc(columns * sizeof(bool));
    }
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            newGrid[i][j] = false;
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            int liveNeighbors = countLiveNeighbors(grid, rows, columns, i, j);
            if (grid[i][j]) {
                // Apply the rules for live cells
                if (liveNeighbors < 2 || liveNeighbors > 3) {
                    newGrid[i][j] = false; // Cell dies
                } else {
                    newGrid[i][j] = true; // Cell lives
                }
            } else {
                // Apply the rule for dead cells
                if (liveNeighbors == 3) {
                    newGrid[i][j] = true; // Cell becomes alive
                }
            }
        }
    }

    // Copy newGrid to grid
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            grid[i][j] = newGrid[i][j];
        }
    }
    
    for(int i = 0; i < rows; i++){
        free(newGrid[i]);
    }
    free(newGrid);
}

/*
void printGridWithLabels(bool** grid, int rows, int cols) {
    // Print the column numbers
    printf("   "); // Initial padding for row numbers
    for (int j = 0; j < cols; j++) {
        printf("%2d ", j);
    }
    printf("\n");

    // Print the grid with row numbers
    for (int i = 0; i < rows; i++) {
        // Print the row number
        printf("%2d ", i);
        // Print the grid cells
        for (int j = 0; j < cols; j++) {
            printf("%c  ", grid[i][j] ? '#' : '.');
        }
        printf("\n");
    }
    printf("\n");
}
*/


void processSand(int **grid, int **newGrid, int rows, int columns, int row, int column){

    bool down = hasSpaceForSand(grid, row + 1, column, rows);
    bool left = hasSpaceForSand(grid, row + 1, column - 1, rows);
    bool right = hasSpaceForSand(grid, row + 1, column + 1, rows);
    
    if(down){
        setCell(newGrid, row + 1, column, 1);
    }else if (left && right) {
		bool isEven = rand() % 2;
		if(isEven){
            setCell(newGrid, row + 1, column - 1, 1);
        }else{
            setCell(newGrid, row + 1, column + 1, 1);
        }        
	}else if(left){
        setCell(newGrid, row + 1, column - 1, 1);
    }else if(right){
        setCell(newGrid, row + 1, column + 1, 1);
    }else{
        setCell(newGrid, row, column, 1);
    }
}

void setCell(int** grid, int row, int column, int value){
    grid[row][column] = value;
}

bool hasSpaceForSand(int** grid, int row, int column, int rows){
    if(row < rows && grid[row][column] == 0){
        return true;
    }
    return false;
}

void processCells(int **grid, int rows, int columns){
    
    // Assigning memory to new grid
    int **newGrid = (int**)malloc(rows * sizeof(int*));
    for(int i = 0; i < rows; i++){
        newGrid[i] = (int*)malloc(columns * sizeof(int));
    }
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            newGrid[i][j] = 0;
        }
    }
    
    for(int i = rows - 1; i >= 0; i--){
        for(int j = 0; j < columns; j++){
            if(grid[i][j] == 1){
                processSand(grid, newGrid, rows, columns, i, j);
            }
        }
    }
    
    // Copy newGrid to grid
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            grid[i][j] = newGrid[i][j];
        }
    }
    
    for(int i = 0; i < rows; i++){
        free(newGrid[i]);
    }
    free(newGrid);
    
}

/*
void growBranch(PlantBranch *plantBranch){

    if(plantBranch->isGrowing){
        BranchNode aux = plantBranch->
    }

}
*/



//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------


