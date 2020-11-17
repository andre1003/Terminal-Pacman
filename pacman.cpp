#include "iostream"
#include "stdio.h"
#include "termios.h"
#include "unistd.h"
#include "cstdlib"
#include "ctime"
#include "characters.cpp"

using namespace std;

// Remake of getch function (I don't know what I did here, but it works!)
int getc() {
    static struct termios oldt, newt;

    /*tcgetattr gets the parameters of the current terminal
    STDIN_FILENO will tell tcgetattr that it should write the settings
    of stdin to oldt*/
    tcgetattr(STDIN_FILENO, &oldt);
    /*now the settings will be copied*/
    newt = oldt;

    /*ICANON normally takes care that one line at a time will be processed
    that means it will return if it sees a "\n" or an EOF or an EOL*/
    newt.c_lflag &= ~(ICANON);

    /*Those new settings will be set to STDIN
    TCSANOW tells tcsetattr to change attributes immediately. */
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);

    int c = getchar();

    /*restore the old settings*/
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);

    return c;
}

// Function for printing the map, considering every aspect
void print_map(int matrix[21][19]) {
    for(int i = 0; i < 21; i++) {
        for(int j = 0; j < 19; j++) {
            if(matrix[i][j] == 1) printf("\033[1;34m %c\033[0m", 35);      // Wall
            else if(matrix[i][j] == 0) cout << "  ";                       // Free block
            else if(matrix[i][j] == 2) printf("\033[1;33m %c\033[0m", 64); // Pacman
            else if(matrix[i][j] == 3) printf("<3");                       // Life
            else if(matrix[i][j] == 4) printf("\033[1;31m +\033[0m");      // Ghost
            else if(matrix[i][j] == 5) printf(" *");                       // Score
        }
        cout << endl;
    }
}

// Function for performing player movement
void player_movement(int matrix[21][19], int *x, int *y, int *oldx, int *oldy, int *life, int *score) {
    int command;
    command = getc(); // Get keyboard input

    system("clear");

    // Based on WASD letters
    if(command == 'w' || command == 'W') *y-=1;      // Up
    else if(command == 'a' || command == 'A') *x-=1; // Left
    else if(command == 's' || command == 'S') *y+=1; // Down
    else if(command == 'd' || command == 'D') *x+=1; // Right
    else if(command == 27) exit(0);                  // (Esc) Exit game  

    if(matrix[*y][*x] == 0) { // Free block
        matrix[*oldy][*oldx] = 0;
        matrix[*y][*x] = 2;
    }
    else if(matrix[*y][*x] == 3) { // Life
        matrix[*oldy][*oldx] = 0;
        matrix[*y][*x] = 2;
        *life+=1;
    }
    else if(matrix[*y][*x] == 4) { // Ghost
        *life-=1;
        *x = *oldx;
        *y = *oldy;
    }
    else if(matrix[*y][*x] == 5) { // Score
        matrix[*oldy][*oldx] = 0;
        matrix[*y][*x] = 2;
        *score+=1;
    }
    else { // Wall
        *x = *oldx;
        *y = *oldy;
    }
}

// Function for ghost movements
void ghost_movement(int matrix[21][19], int *x, int *y, int *oldx, int *oldy, int *direction, bool *over_score, int *steps) {
    // Directions
    if(*direction == 1) *x+=1;      // Right
    else if(*direction == 2) *x-=1; // Left
    else if(*direction == 3) *y+=1; // Down
    else *y-=1;                     // Up

    *steps+=1; // Step foward
    if(*steps >= (3 + rand() % 8)) { // If ghost walks more than 3 steps (at least), change direction
        *x = *oldx;
        *y = *oldy;
        
        // Change direction
        int aux = *direction;
        do {
            *direction = 1 + rand() % 4;
        } while(*direction == aux);

        *steps = 0;
    }

    if(matrix[*y][*x] == 0) { // Free block
        if(*over_score) matrix[*oldy][*oldx] = 5;
        else matrix[*oldy][*oldx] = 0;
        matrix[*y][*x] = 4;
        *over_score = false;
    }
    else if(matrix[*y][*x] == 2) { // Pacman
        *x = *oldx;
        *y = *oldy;
        *over_score = false;
    }
    else if(matrix[*y][*x] == 5) { // Score (doesn't destroy it)
        if(*over_score) matrix[*oldy][*oldx] = 5;
        else matrix[*oldy][*oldx] = 0;
        matrix[*y][*x] = 4;
        *over_score = true;
    }
    else { // Wall
        *x = *oldx;
        *y = *oldy;

        int aux = *direction;
        do {
            *direction = 1 + rand() % 4;
        } while(*direction == aux);

        *steps = 0;
    }
}

void clear() {
    #ifdef WINDOWS
        system("cls");
    #else
        system("clear");
    #endif
}

void menu() {
    clear();

    cout << "============ TERMINAL PACMAN ============\n\n" << "Commands:\nW - Up\nS - Down\nA - Left\nD - Right\nEsc - Exit\n\nPress any key to continue...";
    getchar();
}


// Main
int main() {
    // Initializing player
    player pacman;
    pacman.init(9, 15, 1);

    // Initializing ghosts
    ghost g1, g2, g3;
    g1.init(8, 9, 3);
    g2.init(9, 9, 4);
    g3.init(10, 9, 3);

    // Map
    int matrix[21][19] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 5, 5, 5, 5, 5, 5, 5, 5, 1, 5, 5, 5, 5, 5, 5, 5, 3, 1,
        1, 5, 1, 1, 5, 1, 1, 1, 5, 1, 5, 1, 1, 1, 5, 1, 1, 5, 1,
        1, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1,
        1, 5, 1, 1, 5, 1, 5, 1, 1, 1, 1, 1, 5, 1, 5, 1, 1, 5, 1,
        1, 5, 5, 5, 5, 1, 5, 5, 5, 1, 5, 5, 5, 1, 5, 5, 5, 5, 1,
        1, 1, 1, 1, 5, 1, 1, 1, 0, 1, 0, 1, 1, 1, 5, 1, 1, 1, 1,
        1, 1, 1, 1, 5, 1, 0, 0, 0, 0, 0, 0, 0, 1, 5, 1, 1, 1, 1,
        1, 1, 1, 1, 5, 1, 0, 0, 0, 0, 0, 0, 0, 1, 5, 1, 1, 1, 1,
        1, 5, 5, 5, 5, 0, 0, 0, 4, 4, 4, 0, 0, 0, 5, 5, 5, 5, 1,
        1, 1, 1, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 1, 1, 1, 1,
        1, 1, 1, 1, 5, 1, 0, 0, 0, 0, 0, 0, 0, 1, 5, 1, 1, 1, 1,
        1, 1, 1, 1, 5, 1, 0, 1, 1, 1, 1, 1, 0, 1, 5, 1, 1, 1, 1,
        1, 5, 5, 5, 5, 5, 5, 5, 5, 1, 5, 5, 5, 5, 5, 5, 5, 5, 1,
        1, 5, 1, 1, 5, 1, 1, 1, 5, 1, 5, 1, 1, 1, 5, 1, 1, 5, 1,
        1, 5, 5, 1, 5, 5, 5, 5, 5, 2, 5, 5, 5, 5, 5, 1, 5, 5, 1,
        1, 1, 5, 1, 5, 5, 5, 1, 1, 1, 1, 1, 5, 5, 5, 1, 5, 1, 1,
        1, 5, 5, 5, 5, 1, 5, 5, 5, 1, 5, 5, 5, 1, 5, 5, 5, 5, 1,
        1, 5, 1, 1, 1, 1, 1, 1, 5, 1, 5, 1, 1, 1, 1, 1, 1, 5, 1,
        1, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };

    // Setting srand
    srand(time(NULL));

    // Show menu
    menu();

    // Loop
    while(true) {
        clear();

        // Update

        pacman.update();
        g1.update();
        g2.update();
        g3.update();

        // Print map

        print_map(matrix);

        // User part
        
        int x = pacman.get_x(), y = pacman.get_y(), oldx = pacman.get_oldx(), oldy = pacman.get_oldy();
        int life = pacman.get_life(), score = pacman.get_score();
        
        cout << "Lifes: " << life << endl << "Score: " << score << endl;
        
        player_movement(matrix, &x, &y, &oldx, &oldy, &life, &score);

        pacman.save_changes(x, y, oldx, oldy, life, score);

        // Bot part (very random!!!)
        // Ghost 1

        x = g1.get_x(); y = g1.get_y(); oldx = g1.get_oldx(); oldy = g1.get_oldy();
        int direction = g1.get_direction(), steps = g1.get_steps();
        bool over_score = g1.get_over_score();

        ghost_movement(matrix, &x, &y, &oldx, &oldy, &direction, &over_score, &steps);

        g1.save_changes(x, y, oldx, oldy, direction, over_score, steps);

        // Ghost 2

        x = g2.get_x(); y = g2.get_y(); oldx = g2.get_oldx(); oldy = g2.get_oldy();
        direction = g2.get_direction(); over_score = g2.get_over_score(); steps = g2.get_steps();

        ghost_movement(matrix, &x, &y, &oldx, &oldy, &direction, &over_score, &steps);

        g2.save_changes(x, y, oldx, oldy, direction, over_score, steps);

        // Ghost 3

        x = g3.get_x(); y = g3.get_y(); oldx = g3.get_oldx(); oldy = g3.get_oldy();
        direction = g3.get_direction(); over_score = g3.get_over_score(); steps = g3.get_steps();

        ghost_movement(matrix, &x, &y, &oldx, &oldy, &direction, &over_score, &steps);

        g3.save_changes(x, y, oldx, oldy, direction, over_score, steps);

        // Life and Score

        if(life <= 0) {
            clear();
            cout << "Game Over!" << endl;
            exit(0);
        }
        
        if(score == 157) {
            cout << "Congratulations, you win!" << endl;
            exit(0);
        }
    }
}
