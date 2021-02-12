#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include<vector>
#include<cassert>

namespace snake {
    enum class direction {
        LEFT, RIGHT, BOTTOM, TOP, NONE
    };
    enum class cell {
        EMPTY, SNAKE, FOOD, TELEPORT
    };
    bool not_opposite(direction, direction);
    struct snake_part {
        direction block_direction;
        int x_coordinate;
        int y_coordinate;

        void move_block(snake_part &other);

        void set_direction(direction new_direction);
    };
    struct teleport{
        //TODO implement all teleport features
    };
    struct food{
        //TODO make food struct to provide lifetime
    };

    struct Game {
    private:
        void move_head();
        snake_part &getBlock(int i);
    public:
        static const int WIDTH = 100;
        static const int HEIGHT = 50;

        Game();

        bool &is_ended();

        void make_move();

        void change_direction(direction another_direction);

        cell get_cell(int x, int y);

        void increase_snake();


        void set_cell(int x, int y, cell value);

        direction get_direction();

        bool check_is_ended();

        void create_food();

        [[nodiscard]] int get_score() const;

        [[nodiscard]] int get_snake_size() const;

        bool no_food();

    private:
        bool ended;
        int snake_size;
        int score;
        bool have_food_on_board;
        std::vector<std::vector<cell>> field;
        std::vector<snake_part> snake;
    };
}//snake
#endif //SNAKE_GAME_H
