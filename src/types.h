#pragma once

#include <cstdint>

struct Move
{
    uint8_t subBoard;
    uint8_t square;
};

enum class Piece
{
    NONE,
    X,
    O
};

enum class Color
{
    X,
    O
};

constexpr Color operator~(const Color& color)
{
    return static_cast<Color>(static_cast<int>(color) ^ 1);
}
