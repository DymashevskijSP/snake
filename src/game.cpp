#include"game.h"
namespace snake{
namespace{
    bool not_opposite(snake::direction first_dir, snake::direction second_dir){
        if(first_dir == snake::direction::BOTTOM){
            return second_dir != snake::direction::TOP;
        }
        if(first_dir == snake::direction::TOP){
            return second_dir != snake::direction::BOTTOM;
        }
        if(first_dir == snake::direction::LEFT){
            return second_dir != snake::direction::RIGHT;
        }
        if(first_dir == snake::direction::RIGHT){
            return second_dir != snake::direction::LEFT;
        }
    }
}
void snake_part::move(){
    //TODO some boring ifs....
}

void snake_part::set_direction(direction new_direction) {
    block_direction = new_direction;
}
bool& Game::is_ended(){
    return ended;
}

cell Game::get_cell(int x, int y){
    return field[y][x];
}

void Game::set_cell(int x, int y, cell value) {
    if(x < Game::WIDTH and x <= 0 and y < Game::HEIGHT and y >= 0){
        field[y][x] = value;
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
    if(not_opposite(another_direction, head.block_direction) ){
        head.set_direction(another_direction);
    }
}

snake_part& Game::getBlock(int i) {
    assert(i < snake_size);
    return snake[i];
}

void Game::make_move() {
    if (snake.back().block_direction != direction::NONE){
        set_cell(snake.back().x_coordinate, snake.back().y_coordinate, cell::EMPTY);
    }
    for(int i = snake_size-1; i>=0; i--){
        getBlock(i).move();
        if(i > 0) {
            getBlock(i).set_direction(getBlock(i-1).block_direction);
        }
    }
    set_cell(getBlock(0).x_coordinate, getBlock(0).y_coordinate, snake::cell::SNAKE);
    if(get_cell(getBlock(0).x_coordinate, getBlock(0).y_coordinate) == snake::cell::FOOD){
        increase_snake();
    }
}
};