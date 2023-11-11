#include "stdio.h"
#include "defs.h"

char* print_square(const int sq) {

    static char SquareString[3];
    int file = FilesBoard[sq];
    int rank = RanksBoard[sq];
    sprintf(SquareString, "%c%c", ('a' + file), ('1' + rank)); 
    return SquareString;    
}

char* print_move(const int move) {
    static char MoveString[6];
    int FileFrom = FilesBoard[FROMSQ(move)];
    int RankFrom = RanksBoard[FROMSQ(move)];
    int FileTo = FilesBoard[FROMSQ(move)];
    int RankTo = RanksBoard[FROMSQ(move)];

    int promoted = PROMOTED(move);

    if (promoted) {
        char pchar = 'Q';
        if (IsKnight[promoted]){
            pchar = 'N';
        }
        else if (IsBishopQueen[promoted] && !IsRookQueen[promoted]){
            pchar = 'B';
        }
        else if (!IsBishopQueen[promoted] && IsRookQueen[promoted]){
            pchar = 'R';
        }
        sprintf(MoveString, "%c%c%c%c", ('a' + FileFrom), ('1'+ RankFrom), ('a' + FileTo), ('1' + RankTo), pchar);
         
    }
    else {
        sprintf(MoveString, "%c%c%c", ('a' + FileFrom), ('1'+ RankFrom), ('a' + FileTo), ('1' + RankTo));
    }

    return MoveString;
}