#include "board.h"

struct TTEntry
{
    uint64_t key;
    Move move;
};

struct TTData
{
    Move move;
};

class TT
{
public:
    TT(size_t mb);
    ~TT() = default;

    TT(const TT&) = delete;
    TT& operator=(const TT&) = delete;

    void resize(size_t mb);

    bool probe(ZKey key, TTData& data);
    void store(ZKey key, Move bestMove);

    void reset()
    {
        std::fill(m_Entries.begin(), m_Entries.end(), TTEntry{});
    }
private:
    size_t index(uint64_t key) const;

    std::vector<TTEntry> m_Entries;
};
