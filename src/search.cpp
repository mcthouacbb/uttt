#include "search.h"
#include "movegen.h"

Search::Search()
{
    m_ShouldStop = false;
}

void Search::setBoard(const Board& board)
{
    m_Board = board;
}

SearchResult Search::runSearch(const SearchLimits& limits)
{
    m_ShouldStop = false;
    nodes = 0;
    m_TimeMan.setLimits(limits);
    m_TimeMan.startSearch();

    int score = 0;
    Move bestMove = Move{};
    for (int depth = 1; depth <= limits.maxDepth; depth++)
    {
        int iterScore = search(depth, 0);
        if (m_ShouldStop)
            break;
        score = iterScore;
        bestMove = m_RootBestMove;
        int64_t time = m_TimeMan.elapsed().count();
        std::cout
            << "info depth " << depth
            << " score cp " << score
            << " nodes " << nodes
            << " time " << time
            << " nps " << nodes * 1000 / std::max(1LL, time)
            << " pv " << bestMove.to.toString()
            << std::endl;
    }
    return SearchResult{score, bestMove};
}

int evaluate(const Board& board)
{
    int eval = board.wonBoards(Color::X).popcount() - board.wonBoards(Color::O).popcount();
    return board.sideToMove() == Color::X ? eval : -eval;
}

int Search::search(int depth, int ply)
{
    // TODO: add better evaluation function
    // TODO: move evaluation into it's own file
    if (depth == 0)
        return evaluate(m_Board);
    if (m_Board.isDrawn())
        return 0;
    if (m_Board.isLost())
        return -SCORE_WIN + ply;

    if (nodes % 512 == 0 && m_TimeMan.shouldStop())
    {
        m_ShouldStop = true;
        return 0;
    }

    bool root = ply == 0;

    MoveList moveList;
    genMoves(m_Board, moveList);

    int bestScore = -SCORE_WIN;

    for (Move move : moveList)
    {
        nodes++;
        m_Board.makeMove(move);
        int score = -search(depth - 1, ply + 1);
        m_Board.unmakeMove(move);

        if (m_ShouldStop)
            return 0;

        if (score > bestScore)
        {
            bestScore = score;
            if (root)
                m_RootBestMove = move;
        }
    }

    return bestScore;
}
