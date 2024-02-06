#pragma once

#include <cstdint>
#include <iostream>
#include <bit>
#include <bitset>

#if defined(_MSC_VER)
#include <intrin.h>
#endif

class Bitboard
{
public:
    constexpr Bitboard() = default;
    constexpr Bitboard(uint16_t v);

    static constexpr Bitboard fromSquare(int sq);

    constexpr Bitboard operator<<(int other) const;
    constexpr Bitboard operator>>(int other) const;
    constexpr Bitboard operator&(const Bitboard& other) const;
    constexpr Bitboard operator|(const Bitboard& other) const;
    constexpr Bitboard operator^(const Bitboard& other) const;
    constexpr Bitboard operator~() const;
    constexpr Bitboard& operator<<=(int other);
    constexpr Bitboard& operator>>=(int other);
    constexpr Bitboard& operator&=(const Bitboard& other);
    constexpr Bitboard& operator|=(const Bitboard& other);
    constexpr Bitboard& operator^=(const Bitboard& other);

    constexpr bool operator==(const Bitboard& other) const = default;
    constexpr bool operator!=(const Bitboard& other) const = default;

    constexpr Bitboard north() const;
    constexpr Bitboard south() const;
    constexpr Bitboard west() const;
    constexpr Bitboard east() const;
    constexpr Bitboard northEast() const;
    constexpr Bitboard northWest() const;
    constexpr Bitboard southEast() const;
    constexpr Bitboard southWest() const;

    constexpr uint64_t value() const;

    uint32_t lsb() const;
    Bitboard lsbBB() const;
    uint32_t msb() const;
    uint32_t popcount() const;
    uint32_t poplsb();
    constexpr bool any() const;
    constexpr bool empty() const;
    constexpr bool multiple() const;
    constexpr bool one() const;

    friend std::ostream& operator<<(std::ostream& os, const Bitboard& bitboard);
private:
    uint16_t m_Value;
};

constexpr Bitboard::Bitboard(uint16_t v)
    : m_Value(v)
{

}


constexpr Bitboard IN_BOARD = 0b111111111;

constexpr Bitboard FILE_A = 0b001001001;
constexpr Bitboard FILE_B = 0b010010010;
constexpr Bitboard FILE_C = 0b100100100;

constexpr Bitboard RANK_1 = 0b000000111;
constexpr Bitboard RANK_2 = 0b000111000;
constexpr Bitboard RANK_3 = 0b111000000;

constexpr Bitboard DIAG_A = 0b100010001;
constexpr Bitboard DIAG_B = 0b001010100;

constexpr Bitboard Bitboard::fromSquare(int sq)
{
    return Bitboard(1u << sq);
}

constexpr Bitboard Bitboard::operator<<(int other) const
{
    return Bitboard(m_Value << other);
}

constexpr Bitboard Bitboard::operator>>(int other) const
{
    return Bitboard(m_Value >> other);
}

constexpr Bitboard Bitboard::operator&(const Bitboard& other) const
{
    return Bitboard(m_Value & other.m_Value);
}

constexpr Bitboard Bitboard::operator|(const Bitboard& other) const
{
    return Bitboard(m_Value | other.m_Value);
}

constexpr Bitboard Bitboard::operator^(const Bitboard& other) const
{
    return Bitboard(m_Value ^ other.m_Value);
}

constexpr Bitboard Bitboard::operator~() const
{
    return Bitboard(~m_Value);
}

constexpr Bitboard& Bitboard::operator<<=(int other)
{
    m_Value <<= other;
    return *this;
}

constexpr Bitboard& Bitboard::operator>>=(int other)
{
    m_Value >>= other;
    return *this;
}

constexpr Bitboard& Bitboard::operator&=(const Bitboard& other)
{
    m_Value &= other.m_Value;
    return *this;
}

constexpr Bitboard& Bitboard::operator|=(const Bitboard& other)
{
    m_Value |= other.m_Value;
    return *this;
}

constexpr Bitboard& Bitboard::operator^=(const Bitboard& other)
{
    m_Value ^= other.m_Value;
    return *this;
}

inline uint8_t reverse(uint8_t b)
{
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

inline std::ostream& operator<<(std::ostream& os, const Bitboard& bitboard)
{
    uint16_t v = bitboard.m_Value;
    for (int i = 0; i < 3; i++)
    {
        // too lazy lmao
        uint8_t val = reverse(v >> 6) >> 5;
        os << std::bitset<3>(val) << std::endl;
        v = (v << 3) & IN_BOARD.value();
    }
    return os;
}

constexpr Bitboard Bitboard::north() const
{
    return (m_Value << 3) & IN_BOARD.value();
}

constexpr Bitboard Bitboard::south() const
{
    return m_Value >> 3;
}

constexpr Bitboard Bitboard::west() const
{
    return Bitboard(m_Value >> 1) & ~FILE_C;
}

constexpr Bitboard Bitboard::east() const
{
    return Bitboard(m_Value << 1) & ~FILE_A;
}

constexpr Bitboard Bitboard::northEast() const
{
    return north().east();
}
constexpr Bitboard Bitboard::northWest() const
{
    return north().west();
}
constexpr Bitboard Bitboard::southEast() const
{
    return south().east();
}
constexpr Bitboard Bitboard::southWest() const
{
    return south().west();
}

constexpr uint64_t Bitboard::value() const
{
    return m_Value;
}

inline uint32_t Bitboard::lsb() const
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_ctzll(m_Value);
#elif defined(_MSC_VER)
    unsigned long idx;
    _BitScanForward64(&idx, m_Value);
    return idx;
#else
    return std::countl_zero(m_Value);
#endif
}

inline Bitboard Bitboard::lsbBB() const
{
    return Bitboard(m_Value & (0 - m_Value));
}

inline uint32_t Bitboard::msb() const
{
#if defined(__GNUC__) || defined(__clang__)
    return 63 - __builtin_clzll(m_Value);
#elif defined(_MSC_VER)
    unsigned long idx;
    _BitScanReverse64(&idx, m_Value);
    return idx;
#else
    return 63 - std::countr_zero(m_Value);
#endif
}

inline uint32_t Bitboard::popcount() const
{
    return std::popcount(m_Value);
}

inline uint32_t Bitboard::poplsb()
{
    uint32_t b = lsb();
    m_Value &= m_Value - 1;
    return b;
}

constexpr bool Bitboard::any() const
{
    return m_Value != 0;
}

constexpr bool Bitboard::empty() const
{
    return m_Value == 0;
}
constexpr bool Bitboard::multiple() const
{
    return (m_Value & (m_Value - 1)) != 0;
}

constexpr bool Bitboard::one() const
{
    return !empty() && !multiple();
}
