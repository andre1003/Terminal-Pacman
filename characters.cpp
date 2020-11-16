#include "iostream"

using namespace std;

class character {
    private:
        int x, y;
        int oldx, oldy;

    public:
        void init(int x, int y) {
            this->x = x;
            this->y = y;
            this->oldx = x;
            this->oldy = y;
        }

        void update() {
            this->oldx = this->x;
            this->oldy = this->y;
        }

        void top() {
            this->y++;
        }

        void bottom() {
            this->y--;
        }

        void right() {
            this->x++;
        }

        void left() {
            this->x--;
        }

        void set_x(int x) {
            this->x = x;
        }

        void set_y(int y) {
            this->y = y;
        }

        void set_oldx(int x) {
            this->oldx = x;
        }

        void set_oldy(int y) {
            this->oldy = y;
        }

        int get_x() {
            return this->x;
        }

        int get_y() {
            return this->y;
        }

        int get_oldx() {
            return this->oldx;
        }

        int get_oldy() {
            return this->oldy;
        }
};

class player : public character {
    private:
        int life;
        int score;

    public:
        void init(int x, int y, int life) {
            set_x(x);
            set_y(y);
            set_oldx(x);
            set_oldy(y);
            this->life = life;
            this->score = 0;
        }

        void save_changes(int x, int y, int oldx, int oldy, int life, int score) {
            set_x(x);
            set_y(y);
            set_oldx(oldx);
            set_oldy(oldy);
            this->life = life;
            this->score = score;
        }

        void set_life(int life) {
            this->life = life;
        }

        void set_score(int score) {
            this->score = score;
        }

        int get_life() {
            return this->life;
        }

        int get_score() {
            return this->score;
        }
};

class ghost : public character {
    public:
        int direction;
        int steps;
        bool over_score;

    public:
        void init(int x, int y, int direction) {
            set_x(x);
            set_y(y);
            set_oldx(x);
            set_oldy(y);
            this->direction = direction;
            this->over_score = false;
            this->steps = 0;
        }

        void save_changes(int x, int y, int oldx, int oldy, int direction, bool over_score, int steps) {
            set_x(x);
            set_y(y);
            set_oldx(oldx);
            set_oldy(oldy);
            this->direction = direction;
            this->over_score = over_score;
            this->steps = steps;
        }

        int get_direction() {
            return this->direction;
        }

        int get_steps() {
            return this->steps;
        }

        bool get_over_score() {
            return this->over_score;
        }
};