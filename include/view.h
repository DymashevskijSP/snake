#ifndef SNAKE_VIEW_H
#define SNAKE_VIEW_H

namespace snake {
    struct View {
    public:
        Game &game_;

        explicit View(Game &g);

        void draw_board();

        direction get_turn();
        static void draw_border();
        void end_game() const;

        static void draw_pause() ;

        static void draw_cell(cell c);
    };
}
#endif //SNAKE_VIEW_H
