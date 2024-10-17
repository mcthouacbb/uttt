#pragma once

#include "board.h"

enum class TTBound : uint8_t
{
    NONE,
    UPPER,
    LOWER,
    EXACT
};

struct TTEntry
{
    uint64_t key;
    int16_t score;
    Move move;
    uint8_t depth;
    TTBound bound;
};

struct TTData
{
    Move move;
    int score;
    int depth;
    TTBound bound;
};

class TT
{
public:
    TT(size_t mb);
    ~TT() = default;

    TT(const TT&) = delete;
    TT& operator=(const TT&) = delete;

    void resize(size_t mb);

    bool probe(ZKey key, int ply, TTData& data);
    void store(ZKey key, int ply, Move bestMove, int score, int depth, TTBound bound);

    void reset()
    {
        std::fill(m_Entries.begin(), m_Entries.end(), TTEntry{});
    }
private:
    size_t index(uint64_t key) const;

    std::vector<TTEntry> m_Entries;
};
