#pragma once

#include <cstdint>
#include <array>
#include "types.h"
#include "util/prng.h"

namespace zobrist
{

struct Keys
{
    uint64_t oToMove;
    std::array<std::array<uint64_t, 81>, 2> pieceSquares;
};

constexpr Keys generateZobristKeys()
{
    Keys keys = {};

    PRNG prng;
    prng.seed(8367428251681ull);

    for (int color = 0; color < 2; color++)
        for (int square = 0; square < 81; square++)
                keys.pieceSquares[color][square] = prng.next64();

    keys.oToMove = prng.next64();
    return keys;
}

constexpr Keys keys = generateZobristKeys();

}

struct ZKey
{
    uint64_t value = 0;

    void flipSideToMove();
    void addPiece(Color color, Square square);

    bool operator==(const ZKey& other) const = default;
    bool operator!=(const ZKey& other) const = default;
};

inline void ZKey::flipSideToMove()
{
    value ^= zobrist::keys.oToMove;
}

inline void ZKey::addPiece(Color color, Square square)
{
    value ^= zobrist::keys.pieceSquares[static_cast<int>(color)][9 * square.y() + square.x()];
}
