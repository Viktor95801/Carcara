#ifndef CARCARA_BOARD_H
#define CARCARA_BOARD_H

#include <stdbool.h>

#include "utils.h"

enum Enum_Square {
  a1, b1, c1, d1, e1, f1, g1, h1,
  a2, b2, c2, d2, e2, f2, g2, h2,
  a3, b3, c3, d3, e3, f3, g3, h3,
  a4, b4, c4, d4, e4, f4, g4, h4,
  a5, b5, c5, d5, e5, f5, g5, h5,
  a6, b6, c6, d6, e6, f6, g6, h6,
  a7, b7, c7, d7, e7, f7, g7, h7,
  a8, b8, c8, d8, e8, f8, g8, h8
};

enum Enum_BBPieces {
    BBPawn,
    BBRook,
    BBHorse,
    BBBishop,
    BBKing, BBQueen,
    
    BBWhite = 0, BBBlack = 1
};
#define SIDE_AMOUNT 2
#define BBPIECE_AMOUNT 6

typedef u64 bboard;

typedef struct {
    struct {
        union {
            bboard pieces[SIDE_AMOUNT][BBPIECE_AMOUNT];
            struct {
                bboard Wpawn;
                bboard Wrook;
                bboard Whorse;
                bboard Wbishop;
                bboard Wking; bboard Wqueen;

                bboard Bpawn;
                bboard Brook;
                bboard Bhorse;
                bboard Bbishop;
                bboard Bking; bboard Bqueen;
            };
        };
        bboard occ;
        bboard empty;
    } bb;

    bool side;
    uint8_t enpass_sqr;
    uint8_t castling_rights;
    uint8_t fifty_clock;
    uint8_t fullmove_clock;
} Board;

inline bool board_get_castlingRight(const Board *board, bool side, bool queen_side);
inline void board_set_castlingRight(Board *board, bool side, bool queen_side);

#define SetBit(index)       ((bboard)1 << (index))
#define GetBit(bb, index) !!((bb) & SetBit(index))

#endif//CARCARA_BOARD_H