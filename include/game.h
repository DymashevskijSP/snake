#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include <cassert>
#include <ctime>
#include <list>
#include <optional>
#include <vector>

namespace snake {
    enum class direction {
        LEFT,
        RIGHT,
        BOTTOM,
        TOP,
        NONE
    };
    enum class cell {
        EMPTY,
        SNAKE,
        FOOD,
        TELEPORT
    };

    bool not_opposite(direction, direction);

    struct snake_part {
        direction block_direction;
        int x_coordinate;
        int y_coordinate;

        void move_block(snake_part &other);

        void set_direction(direction new_direction);
    };
    //it is possible to create class temporary and inherit food and teleport, but do we really need it in snake game?
    struct teleport {
        int x, y, x2, y2;
        int time_created;
        static const int lifetime = 30;
        teleport(int x_, int y_, int x2_, int y2_);
    };

    struct food {
        static const int lifetime = 10;
        int x, y, time_created;

        food(int x_, int y_);
    };


    struct Game {
    private:
        void move_head();

        snake_part &get_block(int i);

    public:
        static const int WIDTH = 30;
        static const int HEIGHT = 30;
        static const int SNAKE_START_X_COORDINATE = WIDTH / 2;
        static const int SNAKE_START_Y_COORDINATE = HEIGHT /2;
        bool started = false;
        Game();

        bool &is_ended();

        void make_move();

        void change_direction(direction another_direction);

        cell get_cell(int x, int y);

        void increase_snake();


        void set_cell(int x, int y, cell value);

        direction get_direction();

        [[nodiscard]] bool is_paused() const;

        bool check_is_ended();

        void pause();

        bool is_teleport_activated() const;
        void create_teleport();
        void create_food();
        void remove_teleport();
        [[nodiscard]] int get_score() const;

        [[nodiscard]] int get_snake_size() const;

        bool no_food();

    private:
        bool ended;
        int snake_size;
        int score;
        bool teleport_activated;
        teleport teleportation_place;
        bool paused = false;
        std::vector<std::vector<cell>> field;
        std::vector<snake_part> snake;
        std::list<food> all_food;
    };
}// namespace snake
#endif//SNAKE_GAME_H
