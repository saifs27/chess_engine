#include "defs.h"
#include <stdio.h>

U64 generate_position_key(const S_BOARD *pos) {
    int sq = 0;
    U64 finalKey = 0;
    int piece = EMPTY;

    // pieces
    for (sq = 0; sq < BRD_SQ_NUM; sq++){
        piece = pos->pieces[sq];
        if (piece != NO_SQ && piece != EMPTY && piece != OFFBOARD) {
            ASSERT(piece >= wP && piece <= bK);
            finalKey ^= PieceKeys[piece][sq];
        }
    }

    if (pos -> side == WHITE) {
        finalKey ^= SideKey;
    }

    if (pos -> enPassant != NO_SQ) {
        ASSERT(pos -> enPassant >= 0 && pos -> enPassant < BRD_SQ_NUM);
        finalKey ^= PieceKeys[EMPTY][pos -> enPassant];
    }

    ASSERT(pos -> castlePerm >= 0 && pos -> castlePerm < BRD_SQ_NUM);

    finalKey ^= CastleKeys[pos -> castlePerm];

    return finalKey;  
}
