#include "game.h"
#include "view.h"

namespace {
    const int delay = 90;
}//namespace

void play_game() {
    snake::Game game;
    snake::View view(game);
    view.start();
    while (!game.is_ended()) {
        if (!game.is_paused()) {
            view.draw_board();
        } else {
            view.draw_pause();
        }
        auto prev_direction = game.get_direction();
        for (int i = 0; i < delay; i++) {
            auto next_direction = view.get_turn();
            if (!game.is_paused()) {
                if (snake::not_opposite(prev_direction, next_direction)) {
                    game.change_direction(next_direction);
                }
            }
        }
        game.make_move();
        game.check_is_ended();
        if (game.no_food()) {
            game.create_food();
        }
    }
    view.end_game();
}

int main() {
    srand(time(NULL));
    play_game();
    return 0;
}