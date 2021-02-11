#include "game.h"
#include"view.h"
#include <ncurses.h>

namespace snake {
    namespace {
        const short FIELD_COLOR = COLOR_BLACK;
        const short FOOD_COLOR = COLOR_BLUE;
        const short SNAKE_COLOR = COLOR_RED;
        const int ping = 1;
    }

    View::View(Game &game) : game_(game) {
        WINDOW * wnd = initscr();
        raw();
        noecho();
        keypad(stdscr, TRUE);
        init_pair(1, FIELD_COLOR, FIELD_COLOR);
        init_pair(2, FOOD_COLOR, FOOD_COLOR);
        init_pair(3, SNAKE_COLOR, SNAKE_COLOR);
        timeout(ping);
    }

    void View::draw_cell(cell c) {
        //TODO make colours
        if (c == cell::EMPTY) {
            addch(' ');
            return;
        }
        if (c == cell::FOOD) {
            addch('X');
            return;
        }
        if (c == cell::SNAKE) {
            addch('O');
            return;
        }
        assert(0);
    }

    void View::draw_board() {
        clear();
        move(0, 0);
        addstr("Current score: ");
        printf("%d", game_.get_score());
        addch('\n');
        for (int y = 0; y < Game::HEIGHT; y++) {
            for (int x = 0; x < Game::WIDTH; x++) {
                draw_cell(game_.get_cell(x, y));
            }
            addch('\n');
        }
    }

    direction View::get_direction() {
        //TODO make asynchronous input
        int ch = getch();
        switch (ch) {
            case (KEY_UP):
                return direction::TOP;
            case (KEY_DOWN):
                return direction::BOTTOM;
            case (KEY_LEFT):
                return direction::LEFT;
            case (KEY_RIGHT):
                return direction::RIGHT;
            case ERR:
                return game_.get_direction();
            default:
                return game_.get_direction();
        }
        assert(0);
    }

    void View::end_game() const {
        clear();
        if (game_.get_snake_size() != Game::WIDTH * Game::HEIGHT) {
            addstr("You lost\n");
            addstr("Your score:");
            printw("%d", game_.get_score());

        } else {
            addstr("You won! congratulations!");
        }
        move(Game::HEIGHT, Game::WIDTH);
        addstr("Press any key to continue");
        getch();
        endwin();
    }
}