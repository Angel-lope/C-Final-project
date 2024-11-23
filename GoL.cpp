#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>
//initialize the board with either 
void initializeRandomBoard(int** board, int rows, int columns) {
    srand(static_cast<unsigned>(time(0)));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            board[i][j] = rand() % 2; // 0 or 1, 0 for dead, 1 for alive
        }
    }
}

// Initialize the board with the "Glider Gun"
void initializeGliderGunBoard(int** board, int rows, int columns) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            board[i][j] = 0;
        }
    }
    // Points generated to in purpose create the 'glider gun'
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

void drawBoard(sf::RenderWindow& window, int** board, int rows, int columns, int cellSize) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize)); 
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

int countNeighbors(int** board, int row, int column, int rows, int columns, int range) {
    int livingNeighbors = 0;

    for (int i = -range; i <= range; i++) {
        for (int j = -range; j <= range; j++) {
            if (i == 0 && j == 0) continue; // It skpis the cell itself
            int neighborRow = (row + i + rows) % rows;   // Cells around the cell its scanning
            int neighborColumn = (column + j + columns) % columns; 
            livingNeighbors += board[neighborRow][neighborColumn];
        }
    }

    return livingNeighbors;
}

// Rules for the 3 option, where it scans 5 layers around each cell
int applyRules(int neighbors, int currentState) {
    if (neighbors >= 0 && neighbors <= 33) {
        return 0;
    } else if (neighbors >= 34 && neighbors <= 45) {
        return 1;
    } else if (neighbors >= 58 && neighbors <= 121) {
        return 0;
    }
    return currentState; // If no rule is met it continues with the same state
}

// Update the board based on the rules that you want to apply
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
                // Normal rules
                if (board[i][j] == 1) { 
                    newBoard[i][j] = (neighbors == 2 || neighbors == 3) ? 1 : 0;
                } else { 
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


int main() {
    int option;
    int rows, columns, cellSize, range;
    bool useExtendedRules;

    std::cout << "What would you like to see? \n";
    std::cout << "For random simulation press 1\n";
    std::cout << "For glider gun simulation press 2\n";
    std::cout << "For simulation with extended cell scan range press 3\n";
    std::cin >> option;

    if (option == 1 || option == 2) {
        rows = 50;
        columns = 50;
        cellSize = 15;
        range = 1;
        useExtendedRules = false;
    } else if (option == 3) {
        rows = 520;
        columns = 700;
        cellSize = 2;
        range = 5;
        useExtendedRules = true;
    } else {
        std::cout << "Invalid option. Exiting program.\n";
        return 0;
    }

    int** board = new int*[rows];
    for (int i = 0; i < rows; i++) {
        board[i] = new int[columns];
    }

    if (option == 1) {
        initializeRandomBoard(board, rows, columns);
    } else if (option == 2) {
        initializeGliderGunBoard(board, rows, columns);
    } else if (option == 3) {
        initializeRandomBoard(board, rows, columns); // Random initialization
    }

    sf::RenderWindow window(sf::VideoMode(columns * cellSize, rows * cellSize), "Game of Life");
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        updateBoard(board, rows, columns, range, useExtendedRules);
        window.clear(sf::Color::White);
        drawBoard(window, board, rows, columns, cellSize);
        window.display();
        if (option == 1 || option == 2) {
            sf::sleep(sf::milliseconds(100)); // Speed control
        } else {
            sf::sleep(sf::milliseconds(0));
        }
    }

    for (int i = 0; i < rows; i++) {
        delete[] board[i];
    }
    delete[] board;

    return 0;
}
