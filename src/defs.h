#ifndef DEFS_H
#define DEFS_H

#include <stdlib.h>

#define DEBUG

#ifndef DEBUG
#define ASSERT(n)
#else
#define ASSERT(n) \
if (!(n)) { \
printf("%s - Failed ", #n); \
printf("On %s ", __DATE__); \
printf("At %s ", __TIME__); \
printf("In File %s ", __FILE__); \
printf("At Line %s\n", __LINE__); \
exit(1);}
#endif

typedef unsigned long long U64;

#define NAME "Engine"
#define BRD_SQ_NUM 120

#define MAXGAMEMOVES 2048

#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

// Board structure 
enum {EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK};
enum {FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE};
enum {RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_NONE};

enum {WHITE, BLACK, BOTH};

// 120 square board (to check if move is legal)
enum{
    A1 = 21, B1, C1, D1, E1, F1, G1, H1,
    A2 = 31, B2, C2, D2, E2, F2, G2, H2,
    A3 = 41, B3, C3, D3, E3, F3, G3, H3,
    A4 = 51, B4, C4, D4, E4, F4, G4, H4,
    A5 = 61, B5, C5, D5, E5, F5, G5, H5,
    A6 = 71, B6, C6, D6, E6, F6, G6, H6,
    A7 = 81, B7, C7, D7, E7, F7, G7, H7,
    A8 = 91, B8, C8, D8, E8, F8, G8, H8, NO_SQ, OFFBOARD
    };

enum {FALSE, TRUE};

// Castling Permisions:
enum {WhiteKingsideCastle = 1, WhiteQueensideCastle = 2, BlackKingsideCastle = 4, BlackQueensideCastle = 8};

typedef struct{
    int move;
    int score;
} S_MOVE;

// Undo move structure
typedef struct {
    int move;
    int castlePerm;
    int enPassant;
    int fiftyMove;
    U64 positionKey; //(or hash key) unique number representing position

} S_UNDO;

//Board structure
typedef struct{
    int pieces[BRD_SQ_NUM];
    U64 pawns[3];

    int KingSq[2];

    int side;
    int enPassant; 
    int fiftyMove; 

    int ply; // number of half-moves
    int hisPly;

    U64 positionKey; 

    int castlePerm;

    int PieceNumber[13]; 
    int BigPiece[2]; // all pieces (no pawns) 
    int MajorPiece[2]; // rooks and queens
    int MinorPiece[2]; // knights and bishops
    int material[2];

    S_UNDO history[MAXGAMEMOVES];

    // piece list (no pawns)
    int PieceList[13][10]; 

} S_BOARD;

/* GAME MOVE */
#define FROMSQ(m) ((m) & 0x7F)
#define TOSQ(m) (((m)>>7) & 0x7F)
#define CAPTURED(m) (((m)>>14) & 0xF)
#define PROMOTED(m) (((m)>>20) & 0xF)

#define MFLAGEP 0x40000
#define MFLAGPS 0x80000
#define MFLAGCA 0x1000000

#define MFLAGCAP 0x7C000
#define MFLAGPROM 0xF00000
      

/* MACROS */
#define FILE_RANK_TO_SQ120(f, r) ((21 + (f)) + ((r) * 10)) 
#define SQ64(sq120) (Sq120ToSq64[(sq120)]) 
#define CLEARBIT(bb, sq) ((bb) &= ClearMask[(sq)])
#define SETBIT(bb, sq) ((bb) |= SetMask[(sq)])

/* GLOBALS */
extern int Sq120ToSq64[BRD_SQ_NUM]; 
extern int Sq64ToSq120[64];
extern U64 SetMask[64];
extern U64 ClearMask[64];
extern U64 PieceKeys[13][120];
extern U64 SideKey;
extern U64 CastleKeys[16]; // uses 4 bits to represent castling rights
extern char PieceChar[];
extern char SideChar[];
extern char RankChar[];
extern char FileChar[];

extern int PieceBig[13];
extern int PieceMajor[13];
extern int PieceMinor[13];
extern int PieceValue[13]; 
extern int PieceColor[13]; 

extern int FilesBoard[BRD_SQ_NUM];
extern int RanksBoard[BRD_SQ_NUM];

extern int IsKnight[13]; 
extern int IsKing[13];
extern int IsRookQueen[13];
extern int IsBishopQueen[13];



/* FUNCTIONS */

// init.c 
extern void all_init();

// bitboards.c
extern void print_bitboard(U64 bb); 
extern int pop_bit(U64 *bb); 
extern int count_bits(U64 b); 

// hashkeys.c
extern U64 generate_position_key(const S_BOARD *pos);

// board.c
extern void reset_board(S_BOARD *pos);
extern int parse_fen(char *fen, S_BOARD *pos);
extern void print_board(const S_BOARD *pos);
extern void update_list_material(S_BOARD *pos);
extern int check_board(const S_BOARD *pos);

// attack.c
extern int square_attacked(const int sq, const int side, const S_BOARD *pos);

// io.c
extern char* print_square(const int sq);
extern char* print_move(const int move);

#endif