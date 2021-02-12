#include"game.h"
#include <cstdlib>

namespace snake {
    namespace {


        void normilize_coordinates(int &x, int &y) {
            if (x >= Game::WIDTH) {
                x = 0;
            }
            if (x < 0) {
                x = Game::WIDTH - 1;
            }
            if (y >= Game::HEIGHT) {
                y = 0;
            }
            if (y < 0) {
                y = Game::HEIGHT - 1;
            }
        }
    }//namespace
    bool not_opposite(snake::direction first_dir, snake::direction second_dir) {
        if (first_dir == snake::direction::BOTTOM) {
            return second_dir != snake::direction::TOP;
        }
        if (first_dir == snake::direction::TOP) {
            return second_dir != snake::direction::BOTTOM;
        }
        if (first_dir == snake::direction::LEFT) {
            return second_dir != snake::direction::RIGHT;
        }
        if (first_dir == snake::direction::RIGHT) {
            return second_dir != snake::direction::LEFT;
        }
        return first_dir == snake::direction::NONE || second_dir == snake::direction::NONE;
    }

    void Game::move_head() {
        snake_part& head = getBlock(0);
        if (head.block_direction == direction::LEFT) {
            head.x_coordinate--;
        }
        if (head.block_direction == direction::RIGHT) {
            head.x_coordinate++;
        }
        if (head.block_direction == direction::TOP) {
            head.y_coordinate--;
        }
        if (head.block_direction == direction::BOTTOM) {
            head.y_coordinate++;
        }
        normilize_coordinates(head.x_coordinate, head.y_coordinate);
    }
    void snake_part::move_block(snake_part& other){
        x_coordinate = other.x_coordinate;
        y_coordinate = other.y_coordinate;
    }
    void snake_part::set_direction(direction new_direction) {
        block_direction = new_direction;
    }

    Game::Game() : field(std::vector<std::vector<cell>>(
            snake::Game::HEIGHT, std::vector<cell>(snake::Game::WIDTH, cell::EMPTY))),
                   snake({{snake::direction::NONE,
                                  snake::Game::WIDTH / 2, snake::Game::HEIGHT / 2}}),
                   score(0),
                   snake_size(1),
                   ended(false),
                   have_food_on_board(false) {
        field[Game::HEIGHT / 2][Game::WIDTH / 2] = cell::SNAKE;
        //TODO make constants to better code style
    }


    bool &Game::is_ended() {
        return ended;
    }

    cell Game::get_cell(int x, int y) {
        return field[y][x];
    }

    direction Game::get_direction() {
        return getBlock(0).block_direction;
    }

    void Game::set_cell(int x, int y, cell value) {
        if (x < Game::WIDTH and x >= 0 and y < Game::HEIGHT and y >= 0) {
            field[y][x] = value;
        }
    }

    void Game::create_food() {
        while (true) {
            int x_food = rand() % Game::WIDTH;
            int y_food = rand() % Game::HEIGHT;
            if (get_cell(x_food, y_food) == cell::EMPTY) {
                set_cell(x_food, y_food, cell::FOOD);
                have_food_on_board = true;
                return;
            }
        }
    }

    void Game::increase_snake() {
        snake_size++;
        auto snake_end = snake.back();
        snake.push_back({direction::NONE, snake_end.x_coordinate, snake_end.y_coordinate});
        score++;
    }

    void Game::change_direction(direction another_direction) {
        auto &head = getBlock(0);
        head.set_direction(another_direction);
    }//Do not checks if direction is correct, because of this check in main

    snake_part &Game::getBlock(int i) {
        assert(i < snake_size);
        return snake[i];
    }

    int Game::get_score() const {
        return score;
    }

    int Game::get_snake_size() const {
        return snake_size;
    }

    void Game::make_move() {
        if (snake.back().block_direction != direction::NONE) {
            set_cell(snake.back().x_coordinate, snake.back().y_coordinate, cell::EMPTY);
        }
        for (int i = snake_size - 1; i >= 0; i--) {


            if (i > 0) {
                getBlock(i).move_block(getBlock(i-1));
                set_cell(getBlock(i).x_coordinate, getBlock(i).y_coordinate, cell::SNAKE);
                getBlock(i).set_direction(getBlock(i - 1).block_direction);
            }
        }
        move_head();
        if (get_cell(getBlock(0).x_coordinate, getBlock(0).y_coordinate) == snake::cell::FOOD) {
            increase_snake();
            have_food_on_board = false;
        }
        set_cell(getBlock(0).x_coordinate, getBlock(0).y_coordinate, snake::cell::SNAKE);
    }

    bool Game::no_food() {
        return !have_food_on_board;
    }

    bool Game::check_is_ended() {
        //TODO implement this function
        for (int i = 1; i < snake_size; i++) {
            if (getBlock(i).x_coordinate == getBlock(0).x_coordinate and
                getBlock(i).y_coordinate == getBlock(0).y_coordinate and
                getBlock(i).block_direction != direction::NONE) {
                is_ended() = true;
                break;
            }
        }
        return is_ended();
    }
};