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

void print_map(int matrix[21][19]) {
    for(int i = 0; i < 21; i++) {
        for(int j = 0; j < 19; j++) {
            if(matrix[i][j] == 1) printf("\033[1;34m %c\033[0m", 35);
            else if(matrix[i][j] == 0) cout << "  ";
            else if(matrix[i][j] == 2) printf("\033[1;33m %c\033[0m", 64);
            else if(matrix[i][j] == 3) printf("<3");
            else if(matrix[i][j] == 4) printf(" +");
            else if(matrix[i][j] == 5) printf(" *");
        }
        cout << endl;
    }
}

void movement(int matrix[21][19], int *x, int *y, int *oldx, int *oldy, int *life, int *score) {
    int command;
    command = getc();

    system("clear");

    if(command == 'w' || command == 'W') *y-=1;
    else if(command == 'a' || command == 'A') *x-=1;
    else if(command == 's' || command == 'S') *y+=1;
    else if(command == 'd' || command == 'D') *x+=1;
    else if(command == 27) exit(0);

    if(matrix[*y][*x] == 0) {
        matrix[*oldy][*oldx] = 0;
        matrix[*y][*x] = 2;
    }
    else if(matrix[*y][*x] == 3) {
        matrix[*oldy][*oldx] = 0;
        matrix[*y][*x] = 2;
        *life+=1;
    }
    else if(matrix[*y][*x] == 4) {
        *life-=1;
        *x = *oldx;
        *y = *oldy;
    }
    else if(matrix[*y][*x] == 5) {
        matrix[*oldy][*oldx] = 0;
        matrix[*y][*x] = 2;
        *score+=1;
    }
    else {
        *x = *oldx;
        *y = *oldy;
    }
}

void ghost_movement(int matrix[21][19], int *x, int *y, int *oldx, int *oldy, int *direction, bool *over_score) {
    if(*direction == 1) *x+=1;
    else if(*direction == 2) *x-=1;
    else if(*direction == 3) *y+=1;
    else *y-=1;

    if(matrix[*y][*x] == 0) {
        if(*over_score) matrix[*oldy][*oldx] = 5;
        else matrix[*oldy][*oldx] = 0;
        matrix[*y][*x] = 4;
        *over_score = false;
    }
    else if(matrix[*y][*x] == 2) {
        //life--;
        *over_score = false;
    }
    else if(matrix[*y][*x] == 5) {
        if(*over_score) matrix[*oldy][*oldx] = 5;
        else matrix[*oldy][*oldx] = 0;
        matrix[*y][*x] = 4;
        *over_score = true;
    }
    else {
        *x = *oldx;
        *y = *oldy;
        *direction+=1;
        if(*direction > 4) *direction = 1;
        *over_score = false;
    }
}

int main() {
    player pacman;
    pacman.init(9, 15, 1);

    ghost g1;
    g1.init(9, 9, 1);

    //int x = 9, y = 15, oldx, oldy, life = 1, score = 0;
    //int *x = 6, *y = 4, *oldx, *oldy, *direction = 0;
    bool bot_over_a_score = false;

    int matrix[21][19] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 5, 5, 5, 5, 5, 5, 5, 5, 1, 5, 5, 5, 5, 5, 5, 5, 5, 1,
        1, 5, 1, 1, 5, 1, 1, 1, 5, 1, 5, 1, 1, 1, 5, 1, 1, 5, 1,
        1, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1,
        1, 5, 1, 1, 5, 1, 5, 1, 1, 1, 1, 1, 5, 1, 5, 1, 1, 5, 1,
        1, 5, 5, 5, 5, 1, 5, 5, 5, 1, 5, 5, 5, 1, 5, 5, 5, 5, 1,
        1, 1, 1, 1, 5, 1, 1, 1, 0, 1, 0, 1, 1, 1, 5, 1, 1, 1, 1,
        1, 1, 1, 1, 5, 1, 0, 0, 0, 0, 0, 0, 0, 1, 5, 1, 1, 1, 1,
        1, 1, 1, 1, 5, 1, 0, 0, 0, 0, 0, 0, 0, 1, 5, 1, 1, 1, 1,
        1, 5, 5, 5, 5, 0, 0, 0, 0, 4, 0, 0, 0, 0, 5, 5, 5, 5, 1,
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

    srand(time(NULL));

    while(true) {
        system("clear");

        pacman.update();
        g1.update();

        print_map(matrix);
        
        int x = pacman.get_x(), y = pacman.get_y(), oldx = pacman.get_oldx(), oldy = pacman.get_oldy();
        int life = pacman.get_life(), score = pacman.get_score();
        
        cout << "Lifes: " << life << endl << "Score: " << score << endl;
        
        // User part
        movement(matrix, &x, &y, &oldx, &oldy, &life, &score);

        pacman.save_changes(x, y, oldx, oldy, life, score);

        // Bot part (very random!!!)

        x = g1.get_x(); y = g1.get_y(); oldx = g1.get_oldx(); oldy = g1.get_oldy();
        int direction = g1.get_direction();
        bool over_score = g1.get_over_score();

        ghost_movement(matrix, &x, &y, &oldx, &oldy, &direction, &over_score);

        g1.save_changes(x, y, oldx, oldy, direction, over_score);

        // *direction = 1 + rand() % 4;


        if(life <= 0) {
            cout << "Game Over!" << endl;
            exit(0);
        }
        
        if(score == 158) {
            cout << "Congratulations, you win!" << endl;
            exit(0);
        }
    }
}