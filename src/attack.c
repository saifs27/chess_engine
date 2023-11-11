#include "defs.h"

const int KnightDirection[8] = {-8, -19, -21, -12, 8, 19, 21, 12}; // knight
const int RookDirection[8] = {-1, -10, 1, 10}; // rook
const int BishopDirection[8] = {-9, -11, 11, 9}; // bishop
const int KingDirection[8] = {-1, -10, 1, 10, -9, -11, 11, 9}; // king

int square_attacked(const int sq, const int side, const S_BOARD *pos){
    int pce, i, t_sq, dir;
    // pawns
    if (side == WHITE){
        if (pos -> pieces[sq-11] == wP || pos -> pieces[sq - 9] == wP){
            return TRUE;
        }
    }
    else{
        if (pos -> pieces[sq + 11] == bP || pos -> pieces[sq+9] == bP){
            return TRUE;
        }
    }

    // knights
    for (i = 0; i < 8; i++){
        pce = pos -> pieces[sq + KnightDirection[i]];
        if (IsKnight[pce] && PieceColor[pce] == side){
            return TRUE;
        }
    }

    // rooks, queens
    for (i = 0; i < 4; i++){
        dir = RookDirection[i];
        t_sq = sq + dir; 
        pce = pos -> pieces[t_sq];
        while (pce != OFFBOARD){
            if (pce != EMPTY){
                if(IsRookQueen[pce] && PieceColor[pce] == side){
                    return TRUE;
                }
                break;
            }
            t_sq += dir;
            pce = pos -> pieces[t_sq];
        }
    } 

    // bishops, queens
    for (i = 0; i < 4; i++){
        dir = BishopDirection[i];
        t_sq = sq + dir; 
        pce = pos -> pieces[t_sq];
        while (pce != OFFBOARD){
            if (pce != EMPTY){
                if(IsBishopQueen[pce] && PieceColor[pce] == side){
                    return TRUE;
                }
                break;
            }
            t_sq += dir;
            pce = pos -> pieces[t_sq];
        }
    }

    // kings
    for (i = 0; i < 8; i++){
        pce = pos -> pieces[sq + KingDirection[i]];
        if (IsKing[pce] && PieceColor[pce] == side){
            return TRUE;
        }
    } 

    return FALSE;
}