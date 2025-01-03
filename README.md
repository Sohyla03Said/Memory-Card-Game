# Memory Card Matching Game

This is a simple 2D memory card matching game implemented in C++ using the OpenGL library. Players aim to match all pairs of cards by revealing them one at a time. The game tracks the number of moves and elapsed time, providing a fun and interactive way to test memory skills.

## Features
- **Shapes Theme**: Cards have five unique shapes (Star, Triangle, Oval, Hexagon, and Diamond), each with a distinct color.
- **Game Board**: A 3x4 grid of cards that are shuffled randomly at the start of each game.
- **Gameplay Mechanics**:
  - Single-player gameplay.
  - Tracks the number of moves.
  - Includes a real-time timer.
  - Reveals a win message when all pairs are matched.
  - Prompts the player to play again after a win.
- **Aesthetics**: Black background with visually distinct cards and shapes.

## How to Play
1. Run the program.
2. Click on a card to reveal its shape.
3. Click on a second card to attempt to find a matching pair.
4. If the two selected cards match, they remain revealed. Otherwise, they are hidden again.
5. Match all pairs to win the game!

## Controls
- **Mouse**: Left-click to select cards.
- **Keyboard**:
  - Press `Y` to play again after winning.
  - Press `N` to exit the game after winning.

## Compilation and Execution
### Prerequisites
- C++ compiler (e.g., `g++` or MSVC).
- OpenGL and GLUT libraries installed on your system.

### Compilation Command
```bash
g++ -o memory_game memory_game.cpp -lGL -lGLU -lglut
```

### Execution
```bash
./memory_game
```

## Code Structure
### Enum: `Shape`
Represents the five shapes used on the cards:
- `STAR`, `TRIANGLE`, `OVAL`, `HEXAGON`, `DIAMOND`.

### Struct: `Card`
Holds data for each card, including:
- `Shape shape`: The shape on the card.
- `bool revealed`: Whether the card is currently revealed.
- `bool matched`: Whether the card has been successfully matched.
- `int x, y`: The card's position on the game board.
- `int colorIndex`: The color associated with the card's shape.

### Functions
- **Initialization**:
  - `initCards()`: Randomly shuffles and assigns shapes to the card grid.
- **Drawing**:
  - `drawShape()`: Draws a specific shape on the screen.
  - `drawCard()`: Draws a card, either as hidden or revealed.
- **Gameplay**:
  - `mouse()`: Handles mouse click events to reveal cards and check for matches.
  - `isGameWon()`: Checks if all cards have been matched.
- **Display and Interaction**:
  - `display()`: Draws the game board, cards, and game status (e.g., moves, timer).
  - `keyboard()`: Handles keyboard inputs for playing again or exiting the game.
- **Timer**:
  - `updateTimer()`: Updates the elapsed game time.
- **Reset**:
  - `resetGame()`: Resets the game variables and reshuffles the cards for a new game.

### OpenGL Setup
- **Window Configuration**:
  - 600x600 pixels.
  - 2D orthographic projection.
- **Background**:
  - Black with light gray hidden cards and colored shapes for revealed cards.

## Example Gameplay
- At the start of the game, all cards are hidden.
- Players click on cards to reveal shapes, aiming to match pairs.
- A win message is displayed after all pairs are matched, along with the total time and move count.

## Future Improvements
- Support for dynamic grid sizes (e.g., 4x4 or larger).
- Add sound effects for clicks and matches.
- Implement multiple difficulty levels with varying time limits or additional shapes.
- Support for two-player mode.

## License
This project is open-source and free to use.

## Author
- **Sohyla Said Mohamed**
- **Gmail** sohyla.abdelwahed@ejust.edu.eg
