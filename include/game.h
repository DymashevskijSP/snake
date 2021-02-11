#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include<vector>
#include<cassert>

namespace snake {
    enum class direction {
        LEFT, RIGHT, BOTTOM, TOP, NONE
    };
    enum class cell {
        EMPTY, SNAKE, FOOD
    };

    struct snake_part {
        direction block_direction;
        int x_coordinate;
        int y_coordinate;

        void move();

        void set_direction(direction new_direction);
    };

    struct Game {
    public:
        static const int WIDTH = 10;
        static const int HEIGHT = 10;

        Game();

        bool &is_ended();

        void make_move();

        void change_direction(direction another_direction);

        cell get_cell(int x, int y);

        void increase_snake();

        snake_part &getBlock(int i);

        void set_cell(int x, int y, cell value);

        direction get_direction();

        int get_score() const;
        int get_snake_size() const;

    private:
        bool ended;
        int snake_size;
        int score;
        std::vector<std::vector<cell>> field;
        std::vector<snake_part> snake;
    };
}//snake
#endif //SNAKE_GAME_H
