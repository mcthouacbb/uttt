#include "tt.h"

#if defined(_MSC_VER) && !defined(__clang__)
#include <mmintrin.h>
void prefetchAddr(const void* addr)
{
    return _mm_prefetch(static_cast<const char*>(addr), _MM_HINT_T0);
}

uint64_t mulhi64(uint64_t a, uint64_t b)
{
    return __umulh(a, b);
}
#elif defined(_GNU_C) || defined(__clang__)
void prefetchAddr(const void* addr)
{
    return __builtin_prefetch(addr);
}

uint64_t mulhi64(uint64_t a, uint64_t b)
{
    unsigned __int128 result = static_cast<unsigned __int128>(a) * static_cast<unsigned __int128>(b);
    return result >> 64;
}
#else
void prefetchAddr(const void* addr)
{
    // fallback
}

// taken from stockfish https://github.com/official-stockfish/Stockfish/blob/master/src/misc.h#L158
uint64_t mulhi64(uint64_t a, uint64_t b)
{
    uint64_t aL = static_cast<uint32_t>(a), aH = a >> 32;
    uint64_t bL = static_cast<uint32_t>(b), bH = b >> 32;
    uint64_t c1 = (aL * bL) >> 32;
    uint64_t c2 = aH * bL + c1;
    uint64_t c3 = aL * bH + static_cast<uint32_t>(c2);
    return aH * bH + (c2 >> 32) + (c3 >> 32);
}
#endif

inline int retrieveScore(int score, int ply)
{
    if (isWinScore(score))
    {
        score -= score < 0 ? -ply : ply;
    }
    return score;
}

inline int storeScore(int score, int ply)
{
    if (isWinScore(score))
    {
        score += score < 0 ? -ply : ply;
    }
    return score;
}

TT::TT(size_t mb)
{
    resize(mb);
}

size_t TT::index(uint64_t key) const
{
    return static_cast<size_t>(mulhi64(key, m_Entries.size()));
}

void TT::resize(size_t mb)
{
    size_t numEntries = mb * 1024 * 1024 / sizeof(TTEntry);
    m_Entries.resize(numEntries);
    reset();
}

bool TT::probe(ZKey key, int ply, TTData& data)
{
    auto& entry = m_Entries[index(key.value)];
    if (entry.key != key.value)
        return false;
    data.move = entry.move;
    data.score = retrieveScore(entry.score, ply);
    data.depth = entry.depth;
    data.bound = entry.bound;
    return true;
}

void TT::store(ZKey key, int ply, Move bestMove, int score, int depth, TTBound bound)
{
    auto& entry = m_Entries[index(key.value)];
    entry.key = key.value;
    entry.move = bestMove;
    entry.score = storeScore(score, ply);
    entry.depth = depth;
    entry.bound = bound;
}
