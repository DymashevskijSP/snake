#include"game.h"
#include"view.h"

namespace {
    //TODO fix constants
    const int delay = 10;
}

void play_game() {
    snake::Game game;
    snake::View view(game);
    while (!game.is_ended()) {
        view.draw_board();
        snake::direction next_direction = snake::direction::NONE;
        for (int i = 0; i< delay; i++) {
            next_direction = view.get_direction();
        }
        game.change_direction(next_direction);
        game.make_move();
        game.check_is_ended();
    }
    view.end_game();
}

int main() {
    play_game();
    return 0;
}