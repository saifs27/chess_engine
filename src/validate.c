#include "defs.h"

int square_on_board(const int sq) {
    return (FilesBoard[sq] == OFFBOARD) ? 0 : 1;
}

int valid_side(const int side) {
    return (side == WHITE || side == BLACK) ? 0 : 1;
}

int valid_fileRank(const int fileRank) {
    return (fileRank >= 0 && fileRank <= 7) ? 0 : 1;
}

int valid_empty_piece(const int piece) {
    return (piece>= EMPTY && piece <= bK)? 1 : 0;
}

int valid_piece(const int piece) {
    return (piece >= wP && piece <= bK) ? 1 : 0;
}