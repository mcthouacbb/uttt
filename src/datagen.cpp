#include "datagen.h"
#include "board.h"
#include "movegen.h"
#include "search.h"
#include <fstream>
#include <thread>
#include <mutex>
#include <random>

void playNRandomMoves(Board& board, int n, std::mt19937& rng)
{
    for (int i = 0; i < n; i++)
    {
        MoveList moves;
        genMoves(board, moves);
        std::uniform_int_distribution<int> dist(0, moves.size() - 1);
        Move move = moves[dist(rng)];
        board.makeMove(move);
    }
}

enum class GameResult
{
    NONE,
    X_WIN,
    DRAW,
    X_LOSS
};

struct Position
{
    std::string fen;
    int score;
    Move bestMove;
};

void datagenWorker(int id, std::string outFilename, uint32_t seed, std::mutex& mutex)
{
    outFilename += std::to_string(id) + ".txt";
    std::ofstream outFile(outFilename, std::ios::app);

    Search search;
    std::mt19937 mt19937(seed);
    uint64_t positionsWritten = 0;
    uint64_t positionsThisBatch = 0;
    auto startTime = std::chrono::steady_clock::now();
    auto batchStartTime = startTime;
    while (true)
    {
        std::vector<Position> positions;
        search.newGame();
        Board board;
        playNRandomMoves(board, 6, mt19937);

        GameResult wdlResult;
        while (true)
        {
            SearchLimits limits = {};
            // 40k soft node limit
            limits.softNodes = 40000;
            limits.nodes = 1000000;
            limits.maxDepth = 127;

            search.setBoard(board);
            SearchResult result = search.runSearch(limits, false);
            positions.push_back({board.fenStr(), result.score, result.bestMove});

            board.makeMove(result.bestMove);
            if (board.isLost())
            {
                if (board.sideToMove() == Color::X)
                    wdlResult = GameResult::X_LOSS;
                else
                    wdlResult = GameResult::X_WIN;
                break;
            }
            else if (board.isDrawn())
            {
                wdlResult = GameResult::DRAW;
                break;
            }
        }

        for (auto& pos : positions)
        {
            outFile << pos.fen << " | " << pos.score << " | " << pos.bestMove.to.toString() << " | ";
            if (wdlResult == GameResult::X_WIN)
                outFile << "1.0";
            else if (wdlResult == GameResult::DRAW)
                outFile << "0.5";
            else
                outFile << "0.0";
            outFile << std::endl;
        }

        positionsWritten += positions.size();
        positionsThisBatch += positions.size();

        if (positionsThisBatch > 4096)
        {
            auto now = std::chrono::steady_clock::now();
            auto batchTime = std::chrono::duration_cast<std::chrono::duration<float>>(now - batchStartTime).count();
            auto totalTime = std::chrono::duration_cast<std::chrono::duration<float>>(now - startTime).count();
            mutex.lock();
            std::cout << "[Thread " << id << "] Written " << positionsThisBatch << " poitions in " << batchTime << " seconds" << std::endl;
            std::cout << "\tTotal positions: " << positionsWritten << " Pos/s: " << positionsWritten / totalTime << std::endl;
            mutex.unlock();

            positionsThisBatch = 0;
            batchStartTime = std::chrono::steady_clock::now();
        }
    }
}

void runDatagen(int numThreads, std::string filename)
{
    std::mutex mutex;
    // just 1 thread for now
    std::vector<std::jthread> threads;
    for (int i = 0; i < numThreads; i++)
    {
        threads.push_back(std::jthread([i, &mutex, &filename]()
        {
            datagenWorker(i, filename, 384 + i * 24244, mutex);
        }));
    }
}
