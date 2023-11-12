#include <stdio.h>
#include "defs.h"


void add_move_quiet(const S_BOARD *pos, int move, S_MOVELIST *list) {
    list->moves[list->count].move = move;
    list->moves[list->count].score = 0;
    list->count++;

}

void add_move_capture(const S_BOARD *pos, int move, S_MOVELIST *list) {
    list->moves[list->count].move = move;
    list->moves[list->count].score = 0;
    list->count++;

}

void add_move_enPassant(const S_BOARD *pos, int move, S_MOVELIST *list) {
    list->moves[list->count].move = move;
    list->moves[list->count].score = 0;
    list->count++;

}

void generate_all_moves(const S_BOARD *pos, S_MOVELIST *list) {
    list->count = 0;
}
