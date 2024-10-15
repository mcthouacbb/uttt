#pragma once

#include <cstdint>
#include <string>

struct Square
{
public:
    Square()
        : m_SubBoard(0), m_SubSquare(0)
    {

    }
    explicit Square(uint8_t square)
    {
        int x = square % 9;
        int y = square / 9;
        int subBoardX = x / 3;
        int subBoardY = y / 3;
        m_SubBoard = subBoardY * 3 + subBoardX;
        int subSquareX = x - subBoardX * 3;
        int subSquareY = y - subBoardY * 3;
        m_SubSquare = subSquareY * 3 + subSquareX;
    }

    Square(uint8_t subBoard, uint8_t subSquare)
        : m_SubBoard(subBoard), m_SubSquare(subSquare)
    {

    }

    uint8_t subBoard() const
    {
        return m_SubBoard;
    }

    uint8_t subSquare() const
    {
        return m_SubSquare;
    }

    uint8_t x()
    {
        return 3 * (m_SubBoard % 3) + m_SubSquare % 3;
    }

    uint8_t y()
    {
        return 3 * (m_SubBoard / 3) + m_SubSquare / 3;
    }

    std::string toString()
    {
        std::string result;
        result += static_cast<char>(x() + 'a');
        result += static_cast<char>(y() + '1');
        return result;
    }

    constexpr bool operator==(const Square& other) const noexcept = default;
    constexpr bool operator!=(const Square& other) const noexcept = default;
private:
    uint8_t m_SubBoard, m_SubSquare;
};

struct Move
{
    Square to;

    constexpr bool operator==(const Move& other) const noexcept = default;
    constexpr bool operator!=(const Move& other) const noexcept = default;
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
