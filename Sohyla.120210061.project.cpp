// Name: Sohyla Said Mohamed

#include <iostream>
#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <vector>
#include <algorithm> // For std::shuffle

using namespace std;

// Enum to represent the shapes
enum Shape {
    STAR,
    TRIANGLE,
    OVAL,
    HEXAGON,
    DIAMOND
};

// Card data structure
struct Card {
    Shape shape;   // Shape on the card
    bool revealed; // Is the card revealed?
    bool matched;  // If the card is part of a match
    int x, y;      // Position of the card on screen
    int colorIndex; // Index to store color of the shape
};

// Game variables
Card cards[2][5];              // 2x5 grid of cards
int firstCardX, firstCardY;    // Coordinates of the first selected card
bool firstCardSelected = false; // If a card has been selected
int moves = 0;                  // Move counter

// Timer variables
int startTime = 0;             // Starting time of the game (milliseconds)
int elapsedTime = 0;           // Elapsed time in seconds
bool gameStarted = false;      // Flag to check if the game has started
bool gameWon = false;          // Flag to check if the game is won
bool showPlayAgainPrompt = false; // Flag to show play again prompt
bool waitingForResponse = false;  // Flag to check if we are waiting for user input
bool winMessageShown = false;     // Flag to check if the win message has been shown
int winMessageTime = 0;           // Time when win message is shown

// Colors for each shape
float colors[5][3] = {
    {1.0f, 0.0f, 0.0f}, // Red for STAR
    {0.0f, 1.0f, 0.0f}, // Green for TRIANGLE
    {0.0f, 0.0f, 1.0f}, // Blue for OVAL
    {1.0f, 1.0f, 0.0f}, // Yellow for HEXAGON
    {1.0f, 0.0f, 1.0f}  // Magenta for DIAMOND
};

// Function to convert integer to string
std::string toString(int num) {
    std::ostringstream ss;
    ss << num;
    return ss.str();
}

// Function to initialize the cards with shapes
void initCards() {
    vector<Shape> shapes = { STAR, TRIANGLE, OVAL, HEXAGON, DIAMOND, DIAMOND, HEXAGON, OVAL, TRIANGLE, STAR };
    int index = 0;

    // Shuffle the shapes randomly
    srand(time(0));
    random_shuffle(shapes.begin(), shapes.end());

    // Initialize the card grid with random shapes
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 5; j++) {
            cards[i][j].shape = shapes[index++];
            cards[i][j].revealed = false;
            cards[i][j].matched = false; // Initially no cards are matched
            cards[i][j].x = j * 100 + 50;  // X position
            cards[i][j].y = i * 100 + 100; // Y position
            cards[i][j].colorIndex = cards[i][j].shape; // Assign color based on shape
        }
    }
}

// Function to draw a star shape
void drawStar(int x, int y, int colorIndex) {
    glColor3fv(colors[colorIndex]); // Set color for the shape
    glBegin(GL_POLYGON);
    for (int i = 0; i < 5; i++) {
        float angle = 2 * 3.14159f * i / 5;
        glVertex2f(x + 30 * cos(angle), y + 30 * sin(angle));
    }
    glEnd();
}

// Function to draw a triangle
void drawTriangle(int x, int y, int colorIndex) {
    glColor3fv(colors[colorIndex]); // Set color for the shape
    glBegin(GL_TRIANGLES);
    glVertex2f(x, y + 30);       // Top vertex
    glVertex2f(x - 26, y - 15);  // Bottom-left vertex
    glVertex2f(x + 26, y - 15);  // Bottom-right vertex
    glEnd();
}

// Function to draw an oval
void drawOval(int x, int y, int colorIndex) {
    glColor3fv(colors[colorIndex]); // Set color for the shape
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float angle = 2 * 3.14159f * i / 360;
        glVertex2f(x + 30 * cos(angle), y + 15 * sin(angle));
    }
    glEnd();
}

// Function to draw a hexagon
void drawHexagon(int x, int y, int colorIndex) {
    glColor3fv(colors[colorIndex]); // Set color for the shape
    glBegin(GL_POLYGON);
    for (int i = 0; i < 6; i++) {
        float angle = 2 * 3.14159f * i / 6;
        glVertex2f(x + 30 * cos(angle), y + 30 * sin(angle));
    }
    glEnd();
}

// Function to draw a diamond
void drawDiamond(int x, int y, int colorIndex) {
    glColor3fv(colors[colorIndex]); // Set color for the shape
    glBegin(GL_QUADS);
    glVertex2f(x, y + 30);   // Top vertex
    glVertex2f(x + 30, y);    // Right vertex
    glVertex2f(x, y - 30);    // Bottom vertex
    glVertex2f(x - 30, y);    // Left vertex
    glEnd();
}

// Function to draw the shape on a card
void drawShape(Shape shape, int x, int y, int colorIndex) {
    switch (shape) {
    case STAR: drawStar(x, y, colorIndex); break;
    case TRIANGLE: drawTriangle(x, y, colorIndex); break;
    case OVAL: drawOval(x, y, colorIndex); break;
    case HEXAGON: drawHexagon(x, y, colorIndex); break;
    case DIAMOND: drawDiamond(x, y, colorIndex); break;
    }
}

// Function to draw a card
void drawCard(Card& card) {
    if (card.revealed || card.matched) {
        // Draw the shape on the revealed or matched card
        drawShape(card.shape, card.x, card.y, card.colorIndex);
    }
    else {
        // Draw the hidden card (gray rectangle)
        glColor3f(0.7f, 0.7f, 0.7f); // Light gray
        glRecti(card.x - 40, card.y - 40, card.x + 40, card.y + 40);
    }
}

// Function to check if a click is on a card
bool isCardClicked(int mouseX, int mouseY, int& row, int& col) {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 5; j++) {
            if (mouseX > cards[i][j].x - 40 && mouseX < cards[i][j].x + 40 &&
                mouseY > cards[i][j].y - 40 && mouseY < cards[i][j].y + 40 &&
                !cards[i][j].matched) {
                row = i;
                col = j;
                return true;
            }
        }
    }
    return false;
}

// Function to handle mouse clicks
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Convert y to OpenGL coordinates
        y = 600 - y;

        int row, col;
        if (isCardClicked(x, y, row, col)) {
            if (!gameStarted) {
                gameStarted = true; // Start the timer when the game begins
                startTime = glutGet(GLUT_ELAPSED_TIME); // Store the start time
            }

            if (!firstCardSelected) {
                // First card selection
                firstCardX = row;
                firstCardY = col;
                cards[row][col].revealed = true;
                firstCardSelected = true;
            }
            else {
                // Second card selection
                if (firstCardX != row || firstCardY != col) {
                    cards[row][col].revealed = true;
                    moves++;

                    // Check if cards match
                    if (cards[firstCardX][firstCardY].shape == cards[row][col].shape) {
                        // Mark the cards as matched
                        cards[firstCardX][firstCardY].matched = true;
                        cards[row][col].matched = true;
                    }
                    else {
                        // Hide the cards again after a short delay
                        glutPostRedisplay(); // Request redraw to update the board
                        firstCardSelected = false;
                        cards[firstCardX][firstCardY].revealed = false;
                        cards[row][col].revealed = false;
                    }
                }
            }
        }
    }
}

// Function to check if the game is won
bool isGameWon() {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 5; j++) {
            if (!cards[i][j].matched) {
                return false;
            }
        }
    }
    return true;
}

// Function to update the timer
void updateTimer(int value) {
    if (!gameWon) {
        // Calculate the elapsed time in seconds
        elapsedTime = (glutGet(GLUT_ELAPSED_TIME) - startTime) / 1000;
        glutPostRedisplay(); // Redraw the screen
        glutTimerFunc(1000, updateTimer, 0); // Call the timer function every second
    }
}

// Function to reset the game
void resetGame() {
    // Reset game variables
    moves = 0;
    gameStarted = false;
    gameWon = false;
    winMessageShown = false;
    winMessageTime = 0;
    showPlayAgainPrompt = false;
    waitingForResponse = false;
    initCards();
    elapsedTime = 0; // Reset elapsed time to 0
}

// Function to display the game board
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 5; j++) {
            drawCard(cards[i][j]);
        }
    }

    // Display the move count
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2i(10, 580);
    string moveText = "Moves: " + toString(moves);
    for (char c : moveText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    // Display the timer
    if (gameStarted && !gameWon) {
        glRasterPos2i(500, 580);
        string timerText = "Time: " + toString(elapsedTime) + "s";
        for (char c : timerText) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    }

    // Check if the game is won and display the win message
    if (isGameWon() && !gameWon && !winMessageShown) {
        gameWon = true;
        winMessageShown = true;
        winMessageTime = glutGet(GLUT_ELAPSED_TIME); // Capture the time when win message is shown
    }

    if (winMessageShown && (glutGet(GLUT_ELAPSED_TIME) - winMessageTime) < 500) {
        glColor3f(1.0f, 1.0f, 0.0f); // Yellow for win message
        glRasterPos2i(250, 300);
        string winMessage = "You Win! Time: " + toString(elapsedTime) + "s, Moves: " + toString(moves);
        for (char c : winMessage) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    }

    // After 5 seconds, prompt for play again
    if (winMessageShown && (glutGet(GLUT_ELAPSED_TIME) - winMessageTime) >= 500) {
        showPlayAgainPrompt = true;
        glutPostRedisplay();
    }

    // Display the play again prompt if necessary
    if (showPlayAgainPrompt) {
        glColor3f(1.0f, 1.0f, 1.0f); // White text
        glRasterPos2i(200, 350);
        string playAgainText = "Play again? (Y/N)";
        for (char c : playAgainText) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    }

    glutSwapBuffers();
}

// Function to handle keyboard input
void keyboard(unsigned char key, int x, int y) {
    if (showPlayAgainPrompt) {
        if (key == 'Y' || key == 'y') {
            resetGame(); // Reset the game
            startTime = glutGet(GLUT_ELAPSED_TIME); // Reset the timer to current time
            moves = 0; // Reset the moves
            gameStarted = false; // Reset the game start flag to start the timer again
            gameWon = false; // Reset gameWon to allow the game to be won again
            winMessageShown = false; // Reset win message flag
            winMessageTime = 0; // Reset win message time
            glutPostRedisplay(); // Restart the game
            glutTimerFunc(1000, updateTimer, 0); // Restart the timer function
        }
        else if (key == 'N' || key == 'n') {
            exit(0); // Exit the game if the player presses N
        }
    }
}

// Function to set up the OpenGL window
void initOpenGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 600.0, 0.0, 600.0);  // Set the coordinate system
}

int main(int argc, char** argv) {
    srand(time(0));
    initCards();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Memory Card Game");

    initOpenGL();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);

    glutTimerFunc(1000, updateTimer, 0); // Start the timer function

    glutMainLoop();
    return 0;
}
