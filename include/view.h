#ifndef SNAKE_VIEW_H
#define SNAKE_VIEW_H

namespace snake {
    struct View {
    public:
        Game &game_;

        explicit View(Game &g);

        void draw_board();

        direction get_turn();

        void end_game() const;

        void draw_cell(cell c);
    };
}
#endif //SNAKE_VIEW_H
