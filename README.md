# Turn-Based-Game-Simulation
This C++ project is a turn-based game simulation where two teams, Team 1 and Team 2, compete by taking turns to fire missiles at each other's positions on a map. The game is set up with the following components:

Map Initialization:

The map size is defined by the user input arguments for the number of rows and columns.
Each team is allocated a number of positions on the map, defined by the user input arguments for the size of Team 1 and Team 2.
Positions for each team are randomly assigned on the map.
Gameplay:

The game alternates turns between Team 1 and Team 2.
During a turn, the current team provides coordinates to fire a missile.
The game checks if the missile hit or missed the target.
If a missile hits a team's position, it is marked on the map.
Game State:

The game continues until all positions of one team are hit.
The game determines the winner based on which team has more positions still standing.
File Handling:

The map state is read from and written to a binary file (map.dat).
Code Explanation
Main Function:

Handles user input for team sizes and map dimensions.
Initializes the game map with random positions for both teams.
Controls the game loop until a winner is determined.
Displays the final state of the map and announces the winner.
Helper Functions:

strToint: Converts a string to an integer.
intTostr: Converts an integer to a string.
printMap: Prints the current state of the game map.
writeFile: Writes the game map to a binary file.
checkGame: Checks the game state to determine if the game has ended and which team is the winner.
checkPos: Checks if a given position is already occupied.
getScore: Reads the game map from the file and checks the current game state.
Game Logic:

game: Handles the logic for a single turn, including reading input, checking if the move is valid, and updating the map.


Usage
Compile the program using a C++ compiler:
g++ -o game game.cpp -lpthread

Run the program with the required arguments:
./game <Team1Size> <Team2Size> <MapRowLength> <MapColumnLength>

For example:
./game 5 5 10 10

