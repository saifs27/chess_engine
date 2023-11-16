#include <stdio.h>
#include "defs.h"

#define MOVE(from,to,capture,promote,fl) ( (from) | ((to) << 7) | ((capture) << 14) | (promote) << 20 | (fl))
#define SQUARE_OFFBOARD(sq) (FilesBoard[(sq)]==OFFBOARD)


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

void add_white_pawn_capture_move(const S_BOARD *pos, const int from , const int to, const int capture, S_MOVELIST *list) {
    // handle promotion
    if (RanksBoard[from] == 7) {
        add_move_capture(pos, MOVE(from, to, capture, wQ, 0), list);
        add_move_capture(pos, MOVE(from, to, capture, wR, 0), list);
        add_move_capture(pos, MOVE(from, to, capture, wN, 0), list);
        add_move_capture(pos, MOVE(from, to, capture, wB, 0), list);
    }
    else {
        add_move_capture(pos, MOVE(from, to, capture, EMPTY, 0), list);
    }
}

void add_white_pawn_move(const S_BOARD *pos, const int from , const int to, S_MOVELIST *list) {
    // handle promotion
    if (RanksBoard[from] == 7) {
        add_move_quiet(pos, MOVE(from, to, EMPTY, wQ, 0), list);
        add_move_quiet(pos, MOVE(from, to, EMPTY, wR, 0), list);
        add_move_quiet(pos, MOVE(from, to, EMPTY, wN, 0), list);
        add_move_quiet(pos, MOVE(from, to, EMPTY, wB, 0), list);
    }
    else {
        add_move_quiet(pos, MOVE(from, to, EMPTY, EMPTY, 0), list);
    }
}

void generate_all_moves(const S_BOARD *pos, S_MOVELIST *list) {
    ASSERT(board_check(pos));

    int piece = EMPTY;
    int side = pos->side;
    int sq = 0; int t_sq = 0;
    int pieceNum = 0;
}
