#include"game.h"
#include"view.h"

namespace {
    //TODO fix constants
    const int delay = 100;
}

void play_game() {
    snake::Game game;
    snake::View view(game);
    while (!game.is_ended()) {
        view.draw_board();
        for (int i = 0; i < delay; i++) {
            game.change_direction(view.get_turn());
        }
        game.make_move();
        game.check_is_ended();
        if(game.no_food()){
            game.create_food();
        }
    }
    view.end_game();
}

int main() {
    play_game();
    return 0;
}