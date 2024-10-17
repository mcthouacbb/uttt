#include "search.h"
#include "movegen.h"
#include "evaluate.h"
#include "move_picker.h"

Search::Search()
    : m_TT(64)
{
    m_ShouldStop = false;
}

void Search::setBoard(const Board& board)
{
    m_Board = board;
}

uint64_t Search::runBenchSearch(const SearchLimits& limits)
{
    runSearch(limits, false);
    return m_Nodes;
}

SearchResult Search::runSearch(const SearchLimits& limits, bool report)
{
    m_ShouldStop = false;
    m_Nodes = 0;
    m_TimeMan.setLimits(limits);
    m_TimeMan.startSearch();
    m_History.fill({});

    int score = 0;
    Move bestMove = Move{};
    for (int depth = 1; depth <= limits.maxDepth; depth++)
    {
        int iterScore = search(-SCORE_WIN, SCORE_WIN, depth, 0);
        if (m_ShouldStop)
            break;
        score = iterScore;
        bestMove = m_RootBestMove;
        if (report)
        {
            int64_t time = m_TimeMan.elapsed().count();
            std::cout
                << "info depth " << depth
                << " score cp " << score
                << " nodes " << m_Nodes
                << " time " << time
                << " nps " << m_Nodes * 1000 / std::max(1LL, time)
                << " pv " << bestMove.to.toString()
                << std::endl;
        }
    }
    return SearchResult{score, bestMove};
}

int Search::search(int alpha, int beta, int depth, int ply)
{
    int staticEval = eval::evaluate(m_Board);
    if (depth == 0)
        return staticEval;
    if (m_Board.isDrawn())
        return 0;
    if (m_Board.isLost())
        return -SCORE_WIN + ply;

    if (m_Nodes % 512 == 0 && m_TimeMan.shouldStop())
    {
        m_ShouldStop = true;
        return 0;
    }

    bool root = ply == 0;

    if (!root && depth <= 4 && staticEval - 15 * depth >= beta)
        return staticEval;

    TTData ttData = {};
    bool ttHit = m_TT.probe(m_Board.key(), ply, ttData);

    if (!root && ttHit && ttData.depth >= depth && (
        ttData.bound == TTBound::EXACT ||
        ttData.bound == TTBound::LOWER && ttData.score >= beta ||
        ttData.bound == TTBound::UPPER && ttData.score <= alpha
    ))
        return ttData.score;

    MovePicker movePicker(m_Board, ttData.move, m_History);

    int bestScore = -SCORE_WIN;
    Move bestMove = NULL_MOVE;

    Move move;
    MoveList movesTried;

    TTBound bound = TTBound::UPPER;

    while ((move = movePicker.pickNext()) != NULL_MOVE)
    {
        m_Nodes++;
        m_Board.makeMove(move);
        int score = -search(-beta, -alpha, depth - 1, ply + 1);
        m_Board.unmakeMove();

        if (m_ShouldStop)
            return 0;

        if (score > bestScore)
        {
            bestScore = score;
            if (score > alpha)
            {
                alpha = score;
                bestMove = move;
                bound = TTBound::EXACT;
                if (root)
                    m_RootBestMove = move;
            }

            if (score >= beta)
            {
                for (Move triedMove : movesTried)
                {
                    m_History[static_cast<int>(m_Board.sideToMove())][9 * triedMove.to.y() + triedMove.to.x()] -= depth * depth;
                }
                m_History[static_cast<int>(m_Board.sideToMove())][9 * move.to.y() + move.to.x()] += depth * depth;
                bound = TTBound::LOWER;
                break;
            }
        }

        movesTried.push_back(move);
    }

    m_TT.store(m_Board.key(), ply, bestMove, bestScore, depth, bound);

    return bestScore;
}
