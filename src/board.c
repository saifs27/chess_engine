#include <stdio.h>
#include "defs.h"

int board_check(const S_BOARD *pos){
    // temporary variables
    int t_pceNum[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
    int t_bigPce[2] = {0,0};
    int t_majPce[2] = {0,0};
    int t_minPce[2] = {0,0};
    int t_material[2] = {0,0};

    int sq64, t_piece, t_pce_num, sq120, color, pcount;

    U64 t_pawns[3] = {0ULL, 0ULL, 0ULL};

    t_pawns[WHITE] = pos-> pawns[WHITE];
    t_pawns[BLACK] = pos-> pawns[BLACK];
    t_pawns[BOTH] = pos-> pawns[BOTH];

    // check piece lists
    for (t_piece = wP; t_piece <= bK; t_piece++){
        for (t_pce_num = 0; t_pce_num < pos -> PieceNumber[t_piece]; t_pce_num++){
            sq120 = pos -> PieceList[t_piece][t_pce_num];
            ASSERT(pos -> pieces[sq120] == t_piece);
        }
    }

    // check piece count and other counters
    for (sq64 = 0; sq64 < 64; sq64++){
        sq120 = Sq64ToSq120[sq64];
        t_piece = pos -> pieces[sq120];
        t_pceNum[t_piece]++;
        color = PieceColor[t_piece];
        if (PieceBig[t_piece] == TRUE) t_bigPce[color]++;
        if (PieceMinor[t_piece] == TRUE) t_minPce[color]++;
        if (PieceMajor[t_piece] == TRUE) t_majPce[color]++;

        t_material[color] += PieceValue[t_piece];
    }

    for (t_piece = wP; t_piece <= bK; t_piece++){
        ASSERT(t_pceNum[t_piece] == pos -> PieceNumber[t_piece]);
    }

    // check bitboard count
    pcount = count_bits(t_pawns[WHITE]);
    ASSERT(pcount = pos -> PieceNumber[wP]);
    pcount = count_bits(t_pawns[BLACK]);
    ASSERT(pcount = pos -> PieceNumber[bP]);
    pcount = count_bits(t_pawns[BOTH]);
    ASSERT(pcount = pos -> PieceNumber[wP] + pos -> PieceNumber[wP]);

    // check bitboard squares
    while (t_pawns[WHITE]){
        sq64 = pop_bit(&t_pawns[WHITE]);
        ASSERT(pos-> pieces[Sq64ToSq120[sq64]] == wP);
    }

    while (t_pawns[BLACK]){
        sq64 = pop_bit(&t_pawns[BLACK]);
        ASSERT(pos-> pieces[Sq64ToSq120[sq64]] == bP);
    }

    while (t_pawns[BOTH]){
        sq64 = pop_bit(&t_pawns[BOTH]);
        ASSERT(pos-> pieces[Sq64ToSq120[sq64]] == wP || (pos -> pieces[Sq64ToSq120[sq64]] == bP));
    }

    ASSERT(t_material[WHITE] == pos -> material[WHITE] && t_material[BLACK] == pos -> material[BLACK]);
    ASSERT(t_minPce[WHITE] == pos -> MinorPiece[WHITE] && t_minPce[BLACK] == pos -> MinorPiece[BLACK]);
    ASSERT(t_majPce[WHITE] == pos -> MajorPiece[WHITE] && t_majPce[BLACK] == pos -> MajorPiece[BLACK]);
    ASSERT(t_bigPce[WHITE] == pos -> BigPiece[WHITE] && t_bigPce[BLACK] == pos -> BigPiece[BLACK]);

    ASSERT(pos -> side == WHITE || pos -> side == BLACK);
    ASSERT(generate_position_key(pos) == pos -> positionKey);

    // check en passant
    ASSERT(pos -> enPassant == NO_SQ || (RanksBoard[pos -> enPassant] == RANK_6 && pos -> side == WHITE)
            || (RanksBoard[pos -> enPassant] == RANK_3 && pos -> side == BLACK));

    ASSERT(pos -> pieces[pos -> KingSq[WHITE]] == wK);
    ASSERT(pos -> pieces[pos -> KingSq[BLACK]] == bK);

    return TRUE;
}

void update_list_material(S_BOARD *pos){
    int piece, sq, i, color;

    for ( i = 0; i < BRD_SQ_NUM; i++){
        sq = i;
        piece = pos -> pieces[i];
        if (piece != OFFBOARD && piece != EMPTY){
            color = PieceColor[piece];
            if (PieceBig[piece] == TRUE) pos -> BigPiece[color]++;
            if (PieceMajor[piece] == TRUE) pos -> MajorPiece[color]++;
            if (PieceMinor[piece] == TRUE) pos -> MinorPiece[color]++;

            pos -> material[color] += PieceValue[piece];

            // piece list
            pos -> PieceList[piece][pos -> PieceNumber[piece]] = sq;
            pos -> PieceNumber[piece]++;

            if (piece == wK) pos -> KingSq[WHITE] = sq;
            if (piece == bK) pos -> KingSq[BLACK] = sq;

            if (piece == wP){
                SETBIT(pos -> pawns[WHITE], Sq120ToSq64[sq]);
                SETBIT(pos -> pawns[BOTH], Sq120ToSq64[sq]);
            }
            else if (piece == bP){
                SETBIT(pos -> pawns[BLACK], Sq120ToSq64[sq]);
                SETBIT(pos -> pawns[BOTH], Sq120ToSq64[sq]);
            }
        }
    }
}


int parse_fen(char *fen, S_BOARD *pos){
    ASSERT(fen != NULL);
    ASSERT(pos != NULL);

    int rank = RANK_8; // fen starts at 8th rank
    int file = FILE_A;
    int piece = 0; // hold piece if we find one
    int count = 0; // count number of empty squares
    int i = 0;
    int sq64 = 0;
    int sq120 = 0;

    board_reset(pos);

    while ((rank >= RANK_1) && *fen){
        count = 1;
        switch(*fen){
            case 'p': piece = bP; break;
            case 'r': piece = bR; break;
            case 'n': piece = bN; break;
            case 'b': piece = bB; break;
            case 'k': piece = bK; break;
            case 'q': piece = bQ; break;
            case 'P': piece = wP; break;
            case 'R': piece = wR; break;
            case 'N': piece = wN; break;
            case 'B': piece = wB; break;
            case 'K': piece = wK; break;
            case 'Q': piece = wQ; break;

            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
                piece = EMPTY;
                count = *fen - '0'; // subtract by ASCII value to convert to integer
                break;

            case '/':
            case ' ':
                rank--;
                file = FILE_A;
                fen++;
                continue;

            default:
                printf("FEN error \n");
                return -1;

        }

        // place pieces on appropriate squares
        for (i = 0; i < count; i++){
            sq64 = rank * 8 + file;
            sq120 = Sq64ToSq120[sq64];
            if (piece != EMPTY){
                pos -> pieces[sq120] = piece;
            }
            file++;
        }
        fen++;
    }

    ASSERT(*fen == 'w' || *fen == 'b');
    
    // side to move
    pos -> side = (*fen == 'w') ? WHITE: BLACK;
    fen += 2;

    // castling permissions
    for(i = 0; i < 4; i++){
        if (*fen == ' '){
            break;
        }
        switch(*fen){
            case 'K': pos -> castlePerm |= WhiteKingsideCastle; break;
            case 'Q': pos -> castlePerm |= WhiteQueensideCastle; break;
            case 'k': pos -> castlePerm |= BlackKingsideCastle; break;
            case 'q': pos -> castlePerm |= BlackQueensideCastle; break;
            default: break;
        }
        fen++;
    }
    fen++;

    ASSERT(pos -> castlePerm >= 0 && pos -> castlePerm <= 15);

    if (*fen != '-'){
        file = fen[0] - 'a';
        rank = fen[1] - '1';

        ASSERT(file >= FILE_A && file <= FILE_H);
        ASSERT(rank >= RANK_1 && file <= RANK_8);

        pos -> enPassant = FILE_RANK_TO_SQ120(file, rank);

    }

    pos -> positionKey = generate_position_key(pos);
    update_list_material(pos);

    return 0;
}


void board_reset(S_BOARD *pos) {
    int i = 0;

    for (i  = 0; i < BRD_SQ_NUM; i++){
        pos -> pieces[i] = OFFBOARD;
    }

    for (i  = 0; i < 64; i++){
        pos -> pieces[Sq64ToSq120[i]] = EMPTY;
    }

    for (i = 0; i < 3; i++){
        pos -> BigPiece[i] = 0;
        pos -> MajorPiece[i] = 0; 
        pos -> MinorPiece[i] = 0;
        pos -> pawns[i] = 0ULL;
    }
    
    // reset piece number to 0
    for (i = 0; i < 13; i++){
        pos -> PieceNumber[i] = 0;
    }

    pos -> KingSq[WHITE] = pos -> KingSq[BLACK] = NO_SQ;

    pos -> side = BOTH;
    pos -> enPassant = NO_SQ;
    pos -> fiftyMove = 0;

    pos -> ply = 0;
    pos -> hisPly = 0;

    pos -> castlePerm = 0;
    pos -> positionKey = 0ULL;
    
}

void board_print(const S_BOARD *pos){
    int file, rank, sq, piece;

    printf("\nGame Board:\n\n");

    for (rank = RANK_8; rank >= RANK_1; rank--){
        for (file = FILE_A; file <= FILE_H; file++){
            sq = FILE_RANK_TO_SQ120(file, rank);
            piece = pos -> pieces[sq];
            printf("%3c", PieceChar[piece]);
        }
        printf("\n"); 
    }

    printf("\n   ");

    printf("\n");
    printf("side: %c\n", SideChar[pos->side]);
    printf("enPassant:%d\n", pos->enPassant);
    printf("castle:%c%c%c%c\n",
            pos -> castlePerm & WhiteKingsideCastle ? 'K': '-',
            pos -> castlePerm & WhiteQueensideCastle ? 'Q': '-',
            pos -> castlePerm & BlackKingsideCastle ? 'k': '-',
            pos -> castlePerm & BlackQueensideCastle ? 'q': '-');
    printf("PositionKey:%llX\n", pos ->positionKey); // print hashkey in hexadecimal
}
