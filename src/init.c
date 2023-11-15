#include "defs.h"
#include <stdlib.h>
#include <stdio.h>

// fill 64 bits with random numbers
#define RAND_64 (   (U64)rand() | \
                    (U64)rand() << 15 | \
                    (U64)rand() << 30 | \
                    (U64)rand() << 45 | \
                    ((U64)rand() & 0xf) << 60 )

int Sq120ToSq64[BRD_SQ_NUM];
int Sq64ToSq120[64];

U64 SetMask[64];
U64 ClearMask[64];

U64 PieceKeys[13][120];
U64 SideKey;
U64 CastleKeys[16]; // uses 4 bits to represent castling rights

int FilesBoard[BRD_SQ_NUM];
int RanksBoard[BRD_SQ_NUM];

void init_filesranks_board(){

    int sq = A1;
    int sq64 = 0;

    for (int i = 0; i < BRD_SQ_NUM; i++){
        FilesBoard[i] = OFFBOARD;
        RanksBoard[i] = OFFBOARD; 
    }

    for (int rank = RANK_1; rank <= RANK_8; rank++){
        for (int file = FILE_A; file <= FILE_H; file++){
            sq = FILE_RANK_TO_SQ120(file, rank);
            FilesBoard[sq] = file;
            RanksBoard[sq] = rank;
        }
    }   
}

void init_hashkeys() {
    for (int i = 0; i < 13;  i++) {
        for (int j = 0; j < 120; j++){
            PieceKeys[i][j] = RAND_64;
        }
    }
    SideKey = RAND_64;
    for (int i = 0; i < 16; i++){
        CastleKeys[i] = RAND_64;
    }
}

void init_bitmasks(){
    int i = 0;

    // initialize values to 0
    for (i = 0; i < 64; i++){
        SetMask[i] = 0ULL;
        ClearMask[i] = 0ULL;
    }

    // set SetMask to "bitwise or" of itself and bit shifted by i times
    // set ClearMask to bitwise complement of SetMask
    for (i = 0; i < 64; i++){
        SetMask[i] |= (1ULL << i);
        ClearMask[i] = ~SetMask[i];
    }
}



void init_sq120_to_64(){
    int i = 0;
    int file = FILE_A;
    int rank = RANK_1;
    int sq = A1;
    int sq64 = 0;
    for (i = 0; i < BRD_SQ_NUM; i++)
    {
        // return integer not in array
        Sq120ToSq64[i] = 65;
    }
    
    for (i = 0; i < BRD_SQ_NUM; i++)
    {
        // return integer not in array
        Sq120ToSq64[i] = 65;
    }

    for (rank = RANK_1; rank <= RANK_8; rank++)
    {
        for (file = FILE_A; file <= FILE_H; file++)
        {
            sq = FILE_RANK_TO_SQ120(file, rank);
            Sq64ToSq120[sq64] = sq;
            Sq120ToSq64[sq] = sq64;
            sq64++; 
        }
    }
}

void all_init(){
    init_sq120_to_64();
    init_bitmasks();
    init_hashkeys();
    init_filesranks_board();
}