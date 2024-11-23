# C-Final-project
Conways Game of Life with its usual representation and a prefixed example and a bigger layer scan and rules 
Code explanation:  

*FUNCTIONS*

INITIALIZE RANDOM BOARD

void initializeRandomBoard(int** board, int rows, int columns) { 

    srand(static_cast<unsigned>(time(0))); 

    for (int i = 0; i < rows; i++) { 

        for (int j = 0; j < columns; j++) { 

            board[i][j] = rand() % 2; // 0 or 1, representing a dead or alive cell 

        } 

    } 

} 

Purpose: Initializes the board with random values of 0 (dead cell) or 1 (alive cell). 

srand(static_cast<unsigned>(time(0))): Seeds the random number generator. 

rand() % 2: Generates a random value of 0 or 1. 

 
INITIALIZE GLIDER GUN BOARD

void initializeGliderGunBoard(int** board, int rows, int columns) { 

    // Clear board 

    for (int i = 0; i < rows; i++) { 

        for (int j = 0; j < columns; j++) { 

            board[i][j] = 0; 

        } 

    } 

    // Positions of the "Glider Gun" 

    int positions[36][2] = { 

        {5, 1}, {5, 2}, {6, 1}, {6, 2}, 

        {5, 11}, {6, 11}, {7, 11}, {4, 12}, {8, 12}, {3, 13}, {9, 13}, {3, 14}, {9, 14}, 

        {6, 15}, {4, 16}, {8, 16}, {5, 17}, {6, 17}, {7, 17}, {6, 18}, 

        {3, 21}, {4, 21}, {5, 21}, {3, 22}, {4, 22}, {5, 22}, {2, 23}, {6, 23}, 

        {1, 25}, {2, 25}, {6, 25}, {7, 25}, 

        {3, 35}, {4, 35}, {3, 36}, {4, 36} 

    }; 

    for (auto& pos : positions) { 

        board[pos[0]][pos[1]] = 1; 

    } 

} 

Purpose: Initializes the board with a specific pattern called "Glider Gun". 

Clears the board by setting all cells to 0. 

positions: Specifies the positions of alive cells to form the "Glider Gun". 

board[pos[0]][pos[1]] = 1: Places alive cells in the specified positions. 

 

 

 

 
DRAW BOARD

void drawBoard(sf::RenderWindow& window, int** board, int rows, int columns, int cellSize) { 

    for (int i = 0; i < rows; i++) { 

        for (int j = 0; j < columns; j++) { 

            sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize)); // Without subtracting 1 

            cell.setPosition(j * cellSize, i * cellSize); 

            if (board[i][j] == 1) { 

                cell.setFillColor(sf::Color::Green); // Alive cell 

            } else { 

                cell.setFillColor(sf::Color::Black); // Dead cell 

            } 

            window.draw(cell); 

        } 

    } 

} 

Purpose: Draws the board in the window using SFML. 

sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize)): Creates a cell of size cellSize. 

cell.setPosition(j * cellSize, i * cellSize): Positions the cell in the window. 

cell.setFillColor: Sets the color of the cell depending on whether it is alive (green) or dead (black). 

window.draw(cell): Draws the cell in the window. 

COUNT NEIGHBOURS

int countNeighbors(int** board, int row, int column, int rows, int columns, int range) { 

    int livingNeighbors = 0; 

    for (int i = -range; i <= range; i++) { 

        for (int j = -range; j <= range; j++) { 

            if (i == 0 && j == 0) continue; // Skip the current cell 

            int neighborRow = (row + i + rows) % rows;   // Toroidal border 

            int neighborColumn = (column + j + columns) % columns; // Toroidal border 

            livingNeighbors += board[neighborRow][neighborColumn]; 

        } 

    } 

    return livingNeighbors; 

} 

Purpose: Calculates the number of living neighbors around a cell within a specified range. 

for (int i = -range; i <= range; i++): Iterates over the range of cells around the current cell. 

if (i == 0 && j == 0) continue;: Skips the current cell. 

int neighborRow = (row + i + rows) % rows;: Calculates the position of the neighbor considering the toroidal border. 

livingNeighbors += board[neighborRow][neighborColumn];: Adds the state of the neighbor cell (0 or 1) to livingNeighbors. 


APPLY RULES

int applyRules(int neighbors, int currentState) { 

    if (neighbors >= 0 && neighbors <= 33) { 

        return 0; 

    } else if (neighbors >= 34 && neighbors <= 45) { 

        return 1; 

    } else if (neighbors >= 58 && neighbors <= 121) { 

        return 0; 

    } 

    return currentState; // Maintain the current state if no rule is met 

} 

Purpose: Applies the rules to determine the new state of a cell. 

Returns 0 (dead) or 1 (alive) based on the number of living neighbors (neighbors) and specific rules. 

UPDATE BOARD

void updateBoard(int** board, int rows, int columns, int range, bool useExtendedRules) { 

    int** newBoard = new int*[rows]; 

    for (int i = 0; i < rows; i++) { 

        newBoard[i] = new int[columns]; 

    } 

    for (int i = 0; i < rows; i++) { 

        for (int j = 0; j < columns; j++) { 

            int neighbors = countNeighbors(board, i, j, rows, columns, range); 

            if (useExtendedRules) { 

                newBoard[i][j] = applyRules(neighbors, board[i][j]); 

            } else { 

                // Standard Game of Life rules 

                if (board[i][j] == 1) { // Alive cell 

                    newBoard[i][j] = (neighbors == 2 || neighbors == 3) ? 1 : 0; 

           } else { // Dead cell 

                    newBoard[i][j] = (neighbors == 3) ? 1 : 0; 

                } 

            } 

        } 

    } 

    // Copy newBoard to the original board 

    for (int i = 0; i < rows; i++) { 

        for (int j = 0; j < columns; j++) { 

            board[i][j] = newBoard[i][j]; 

        } 

    } 

    for (int i = 0; i < rows; i++) { 

        delete[] newBoard[i]; 

    } 

    delete[] newBoard; 

} 

Purpose: Updates the board based on the game's rules. 

int** newBoard = new int*[rows];: Creates a new board to store the new states of the cells. 

int neighbors = countNeighbors(board, i, j, rows, columns, range);: Calculates the number of living neighbors. 

if (useExtendedRules): Applies the extended rules if necessary. 

board[i][j] = newBoard[i][j];: Copies the new states to the original board. 

delete[] newBoard[i];: Frees the memory of the new board. 
