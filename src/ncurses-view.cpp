#include "game.h"
#include"view.h"
#include <ncurses.h>

namespace snake {
    namespace {
        const short FIELD_COLOR = COLOR_BLACK;
        const short FOOD_COLOR = COLOR_BLUE;
        const short SNAKE_COLOR = COLOR_RED;
        const short TEXT_COLOR = COLOR_WHITE;
        const short TELEPORT_COLOR = COLOR_GREEN;
        const int ping = 1;
    }

    View::View(Game &game) : game_(game) {
        initscr();
        raw();
        noecho();
        keypad(stdscr, TRUE);
        start_color();
        init_pair(1, TEXT_COLOR, FIELD_COLOR);
        init_pair(2, FOOD_COLOR, FOOD_COLOR);
        init_pair(3, SNAKE_COLOR, SNAKE_COLOR);
        init_pair(4, TELEPORT_COLOR, TELEPORT_COLOR);
        curs_set(0);
        timeout(ping);
    }

    void View::draw_cell(cell c) {
        //TODO make colours

        if (c == cell::EMPTY) {
            attron(COLOR_PAIR(1));
        }
        if (c == cell::FOOD) {
            attron(COLOR_PAIR(2));
        }
        if (c == cell::SNAKE) {
            attron(COLOR_PAIR(3));
        }
        if (c == cell::TELEPORT) {
            attron(COLOR_PAIR(4));
        }

        addch(' ');
        addch(' ');
        attron(COLOR_PAIR(0));

    }

    void View::draw_board() {
        clear();
        move(0, 0);
        addstr("Current score: ");
        printw("%d", game_.get_score());
        addch('\n');
        for (int y = 0; y < Game::HEIGHT; y++) {
            for (int x = 0; x < Game::WIDTH; x++) {
                draw_cell(game_.get_cell(x, y));
            }
            addch('\n');
        }
    }

    direction View::get_turn() {
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
            case( 'w'):
                return direction::TOP;
            case('a'):
                return direction::LEFT;
            case('d'):
                return direction::RIGHT;
            case('s'):
                return direction::BOTTOM;
            case ERR:
                return game_.get_direction();
            case 'q':
                game_.is_ended() = true;
                return direction::NONE;
            case 'p':
                game_.pause();
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
        game_.is_ended() = true;
        move(Game::HEIGHT, Game::WIDTH);
        addstr("Press any key to continue");
        nodelay(stdscr, FALSE);
        getch();
        clear();
        endwin();
    }

    void View::draw_pause() const {
        clear();
        move(snake::Game::WIDTH, snake::Game::HEIGHT);
        addstr("Game is paused");
    }
}