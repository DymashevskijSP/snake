#include "game.h"
#include <cstdlib>

namespace snake {
    namespace {

        bool random_generate_teleport() {
            return rand() % 1337 == 228;
        }

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
        snake_part &head = get_block(0);
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

    void snake_part::move_block(snake_part &other) {
        x_coordinate = other.x_coordinate;
        y_coordinate = other.y_coordinate;
    }

    void snake_part::set_direction(direction new_direction) {
        block_direction = new_direction;
    }

    Game::Game() : field(std::vector<std::vector<cell>>(
                           snake::Game::HEIGHT, std::vector<cell>(snake::Game::WIDTH, cell::EMPTY))),
                   score(0),
                   snake_size(3),
                   ended(false), paused(false), teleportation_place(0, 0, 0, 0), snake({}) {
        snake.push_back({direction::NONE, SNAKE_START_X_COORDINATE, SNAKE_START_Y_COORDINATE});
        field[SNAKE_START_Y_COORDINATE][SNAKE_START_X_COORDINATE] = cell::SNAKE;
        snake.push_back({direction::NONE, SNAKE_START_X_COORDINATE - 1, SNAKE_START_Y_COORDINATE});
        field[SNAKE_START_Y_COORDINATE][SNAKE_START_X_COORDINATE - 1] = cell::SNAKE;
        snake.push_back({direction::NONE, SNAKE_START_X_COORDINATE - 2, SNAKE_START_Y_COORDINATE});
        field[SNAKE_START_Y_COORDINATE][SNAKE_START_X_COORDINATE - 2] = cell::SNAKE;
        change_direction(direction::RIGHT);
    }


    bool &Game::is_ended() {
        return ended;
    }

    cell Game::get_cell(int x, int y) {
        return field[y][x];
    }

    direction Game::get_direction() {
        return get_block(0).block_direction;
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
                all_food.emplace_back(x_food, y_food);
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
        auto &head = get_block(0);
        head.set_direction(another_direction);
    }//Do not checks if direction is correct, because of this check in main

    snake_part &Game::get_block(int i) {
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
        if (paused) return;
        if (!teleport_activated) {
            if (random_generate_teleport()) {
                create_teleport();
            }
        } else {
            if (get_cell(teleportation_place.x, teleportation_place.y) != cell::TELEPORT) {
                set_cell(teleportation_place.x, teleportation_place.y, cell::TELEPORT);
            }
            if (get_cell(teleportation_place.x2, teleportation_place.y2) != cell::TELEPORT) {
                set_cell(teleportation_place.x2, teleportation_place.y2, cell::TELEPORT);
            }
        }
        std::vector<std::list<food>::iterator> to_remove;
        for (auto it = all_food.begin(); it != all_food.end(); it++) {
            if (get_cell((*it).x, (*it).y) != cell::FOOD or time(nullptr) - (*it).time_created > food::lifetime) {
                set_cell((*it).x, (*it).y, cell::EMPTY);
                to_remove.push_back(it);
            }
        }
        if (teleport_activated) {
            if (teleportation_place.time_created + snake::teleport::lifetime < time(nullptr)) {
                remove_teleport();
            }
        }
        for (auto it : to_remove) {
            all_food.erase(it);
        }
        if (started and !paused) {
            set_cell(snake.back().x_coordinate, snake.back().y_coordinate, cell::EMPTY);
        }
        for (int i = snake_size - 1; i >= 0; i--) {


            if (i > 0) {
                get_block(i).move_block(get_block(i - 1));
                set_cell(get_block(i).x_coordinate, get_block(i).y_coordinate, cell::SNAKE);
                get_block(i).set_direction(get_block(i - 1).block_direction);
            }
        }
        move_head();
        if (get_cell(get_block(0).x_coordinate, get_block(0).y_coordinate) == snake::cell::FOOD) {
            increase_snake();
        }
        if (is_teleport_activated() and get_block(0).x_coordinate == teleportation_place.x and
            get_block(0).y_coordinate == teleportation_place.y) {
            get_block(0).x_coordinate = teleportation_place.x2;
            get_block(0).y_coordinate = teleportation_place.y2;
        } else if (is_teleport_activated() and get_block(0).x_coordinate == teleportation_place.x2 and
                   get_block(0).y_coordinate == teleportation_place.y2) {
            get_block(0).x_coordinate = teleportation_place.x;
            get_block(0).y_coordinate = teleportation_place.y;
        }
        set_cell(get_block(0).x_coordinate, get_block(0).y_coordinate, snake::cell::SNAKE);
    }

    bool Game::no_food() {
        return all_food.empty();
    }

    bool Game::is_paused() const {
        return paused;
    }

    void Game::pause() {
        paused = !paused;
    }

    bool Game::check_is_ended() {
        if (is_ended()) {
            return is_ended();
        }
        for (int i = 1; i < snake_size; i++) {
            if (get_block(i).x_coordinate == get_block(0).x_coordinate and
                get_block(i).y_coordinate == get_block(0).y_coordinate and
                get_block(i).block_direction != direction::NONE) {
                is_ended() = true;
                break;
            }
        }
        return is_ended();
    }

    bool Game::is_teleport_activated() const {
        return teleport_activated;
    }

    void Game::create_teleport() {
        if (!teleport_activated) {
            while (true) {
                int xin = rand() % Game::WIDTH;
                int yin = rand() % Game::HEIGHT;
                int xout = rand() % Game::WIDTH;
                int yout = rand() % Game::HEIGHT;
                if (xin == xout and yin == yout) {
                    continue;
                }
                if (get_cell(xin, yin) == cell::EMPTY and get_cell(xout, yout) == cell::EMPTY) {
                    teleportation_place = teleport(xin, yin, xout, yout);
                    teleport_activated = true;
                    return;
                }
            }
        }
    }

    void Game::remove_teleport() {
        if (teleport_activated) {
            teleport_activated = false;
            if (get_cell(teleportation_place.x, teleportation_place.y) == cell::TELEPORT) {
                set_cell(teleportation_place.x, teleportation_place.y, cell::EMPTY);
            }
            if (get_cell(teleportation_place.x2, teleportation_place.y2) == cell::TELEPORT) {
                set_cell(teleportation_place.x2, teleportation_place.y2, cell::EMPTY);
            }
        }
    }

    food::food(int x_, int y_) : x(x_), y(y_), time_created(time(nullptr)) {}

    teleport::teleport(int x_, int y_, int x2_, int y2_) : x(x_), y(y_), x2(x2_), y2(y2_),
                                                           time_created(time(nullptr)) {}
};// namespace snake