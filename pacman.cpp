#include "iostream"
#include "stdio.h"
#include "termios.h"
#include "unistd.h"
#include "cstdlib"
#include "ctime"

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

void print_map(int matrix[10][10]) {
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
            if(matrix[i][j] == 1) printf(" %c", 35);
            else if(matrix[i][j] == 0) cout << "  ";
            else if(matrix[i][j] == 2) printf(" %c", 64);
            else if(matrix[i][j] == 3) printf("<3");
            else if(matrix[i][j] == 4) printf(" +");
            else if(matrix[i][j] == 5) printf(" *");
        }
        cout << endl;
    }
}

int main() {
    int x = 1, y = 1, oldx, oldy, life = 1, score = 0;
    int botx = 6, boty = 4, oldbotx, oldboty, decision = 0;
    bool bot_over_a_score = false;
    
    int matrix[10][10] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 2, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 5, 5, 5, 5, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 4, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 3, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    };

    srand(time(NULL));

    while(true) {
        system("clear");

        oldx = x; oldy = y;
        oldbotx = botx; oldboty = boty;

        print_map(matrix);
        cout << "Lifes: " << life << endl << "Score: " << score << endl;

        // User part

        int command;
        command = getc();

        system("clear");

        if(command == 'w' || command == 'W') y--;
        else if(command == 'a' || command == 'A') x--;
        else if(command == 's' || command == 'S') y++;
        else if(command == 'd' || command == 'D') x++;
        else if(command == 27) break;

        if(matrix[y][x] == 0) {
            matrix[oldy][oldx] = 0;
            matrix[y][x] = 2;
        }
        else if(matrix[y][x] == 3) {
            matrix[oldy][oldx] = 0;
            matrix[y][x] = 2;
            life++;
        }
        else if(matrix[y][x] == 4) {
            life--;
            x = oldx;
            y = oldy;
        }
        else if(matrix[y][x] == 5) {
            matrix[oldy][oldx] = 0;
            matrix[y][x] = 2;
            score++;
        }
        else {
            x = oldx;
            y = oldy;
        }

        // Bot part (very random!!!)

        decision = 1 + rand() % 4;
        
        if(decision == 1) botx++;
        else if(decision == 2) botx--;
        else if(decision == 3) boty++;
        else boty--;

        if(matrix[boty][botx] == 0) {
            if(bot_over_a_score) matrix[oldboty][oldbotx] = 5;
            else matrix[oldboty][oldbotx] = 0;
            matrix[boty][botx] = 4;
            bot_over_a_score = false;
        }
        else if(matrix[boty][botx] == 2) {
            life--;
            bot_over_a_score = false;
        }
        else if(matrix[boty][botx] == 5) {
            if(bot_over_a_score) matrix[oldboty][oldbotx] = 5;
            else matrix[oldboty][oldbotx] = 0;
            matrix[boty][botx] = 4;
            bot_over_a_score = true;
        }
        else {
            botx = oldbotx;
            boty = oldboty;
            bot_over_a_score = false;
        }


        if(life <= 0) {
            cout << "Game Over!" << endl;
            exit(0);
        }
    }
}