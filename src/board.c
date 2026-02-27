#include "board.h"

inline bool board_get_castlingRight(const Board *board, bool side, bool queen_side) {
    return ((board->castling_rights >> (side * 2 + queen_side)) & 1);
}
inline void board_set_castlingRight(Board *board, bool side, bool queen_side) {
    board->castling_rights |= (uint8_t)1 << (side * 2 + queen_side);
}

