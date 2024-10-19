#include <iostream>
#include <random>
#include <chrono>
#include <iomanip>

#include "board.h"
#include "movegen.h"
#include "search.h"
#include "bench.h"
#include "datagen.h"
#include "tuner/tune.h"

std::string moveToStr(Move move)
{
    return move.to.toString();
}

Move findMoveFromStr(const Board& board, std::string_view moveStr)
{
    int sq = 9 * (moveStr[1] - '1') + moveStr[0] - 'a';
    MoveList moves;
    genMoves(board, moves);
    auto move = Move{Square(sq)};
    auto it = std::find(moves.begin(), moves.end(), move);
    if (it == moves.end())
        std::cout << "info string illegal move " << moveStr << std::endl;
    return move;
}

template<bool root = true>
uint64_t perft(Board& board, int depth)
{
    if (depth == 0)
        return 1;
    MoveList moves;
    uint64_t nodes = 0;
    if (board.isLost() || board.isDrawn())
        return 0;
    genMoves(board, moves);
    for (Move move : moves)
    {
        board.makeMove(move);
        uint64_t subNodes = perft<false>(board, depth - 1);
        board.unmakeMove();
        if constexpr (root)
            std::cout << moveToStr(move) << ": " << subNodes << std::endl;
        nodes += subNodes;
    }

    return nodes;
}

enum class GameResult
{
    X_WIN, O_WIN, DRAW
};

Move randomMove(const Board& board, std::mt19937& rng)
{
    MoveList moves;
    genMoves(board, moves);
    std::uniform_int_distribution<int> dist(0, moves.size() - 1);
    int idx = dist(rng);
    return moves[idx];
}

GameResult playRandomGame(std::mt19937& rng)
{
    Board board;
    while (true)
    {
        MoveList moves;
        genMoves(board, moves);

        /*if (moves.size() == 0)
        {
            std::cout << board.stringRep() << std::endl;
            std::cout << board.completedBoards() << std::endl;
            std::cout << "WHAT?" << std::endl;
            throw 0;
        }*/
        std::uniform_int_distribution<int> dist(0, moves.size() - 1);
        int idx = dist(rng);

        board.makeMove(moves[idx]);

        if (board.isLost() || board.isDrawn())
            break;
    }
    // std::cout << board.stringRep() << std::endl;
    if (board.isLost())
        return board.sideToMove() == Color::X ? GameResult::O_WIN : GameResult::X_WIN;
    // terrible
    if (board.isDrawn())
        return GameResult::DRAW;
    return GameResult::DRAW;
}

std::pair<std::string, std::array<int, 9>> perftTests[] = {
    {"9/9/9/9/9/9/9/9/9 X 0 0000", {1, 81, 720, 6336, 55080, 473256, 4020960, 33782544, 281067408}},
    {"9/3x5/3oo4/4x2o1/1o7/3x1o3/4o2x1/1x3x3/ox3o1x1 X 0 h6", {1, 6, 44, 320, 2278, 16233, 116635, 849418, 6381392}},
    {"4oo1x1/9/8x/9/3x5/o2x5/1xo6/1o5o1/xo1x1x1o1 X 0 e9", {1, 7, 51, 370, 2827, 21383, 179029, 1487259, 13593237}},
    {"1x7/6ox1/o2o4o/5o3/9/4o4/2xx2o1x/4x4/1ox3x2 X 0 g8", {1, 9, 64, 454, 3185, 23060, 166468, 1260336, 9736622}},
    {"1x3o3/7x1/5o2x/1x3o3/5o1x1/o2o5/3o5/xx7/1x4o2 X 0 f7", {1, 8, 58, 463, 3479, 29053, 241143, 2173280, 19748086}},
    {"7o1/3xx2xx/1x1o2x1x/2oo1o2o/1x2oxoo1/1x3xo2/1xx1oo3/2x2oo1x/o3x3o X 0 f2", {1, 44, 391, 3436, 31662, 289755, 2792347, 26647358, 264767468}},
    {"3o1o3/2xo1x1xx/xox6/6xxo/8o/2x2oxxo/ox2o1ox1/2o5o/o2xox1xo X 0 f4", {1, 4, 28, 239, 2212, 21384, 196693, 1923003, 18155997}},
    {"o1xxox2o/2ox2o2/x3o1o1x/3xx2o1/2x4x1/o2o2x2/oxx1o4/1o2o3o/xxo1x4 X 0 c1", {1, 8, 86, 694, 5205, 40777, 319881, 2664061, 22872400}},
    {"2ox4x/o2xx1x1o/o2x2oo1/1ox2o3/2oo3x1/x1xo3x1/x1xxo2o1/1xo3o2/2o5x X 0 c5", {1, 7, 67, 840, 9609, 115330, 1283277, 14818322, 158683651}},
    {"ooox5/x5ox1/o2xo2o1/1xxx1xoo1/2x2o1ox/o3x3o/x2x5/ooxo1x3/6x2 X 0 g6", {1, 41, 440, 4759, 48816, 496752, 4825482, 47240207, 442983131}},
    {"xox3x2/o2ox1oo1/3x4x/4x2oo/xxx2oo1x/xox1o4/o1o2xoo1/5x3/1o1x5 X 0 b9", {1, 6, 33, 298, 2978, 27462, 251373, 2277374, 20505230}},
    {"1o1x1o2x/o1x2oxox/2xxx1ox1/o3xoxo1/xxx2oooo/o1o1xo1xx/2o2o1x1/x2o2xxx/xxoooo2o X 0 f6", {1, 3, 22, 170, 1292, 7611, 42488, 178604, 683640}},
    {"o3xxo1o/xxxxoxxoo/1x1xx1xx1/1o2oxooo/1o2oxxxo/1oxx2xx1/xo1ooo1ox/o1x2o3/oo7 X 0 h8", {1, 4, 58, 519, 4456, 33205, 232391, 1384237, 7568559}},
    {"1xxxoxxoo/1xx2xo1x/oxox1oo1x/oo1o2o1o/2xx1o1ox/1ooo3oo/o1xx1o3/x2xxoxxx/7ox X 0 g7", {1, 6, 63, 414, 2614, 17476, 108288, 680618, 3769073}},
    {"x1o1ox1xo/xo3ooxx/3o1xoxx/1oxoxx2o/xxxoxooo1/xo3o2x/1x1x1o2o/1oxox1o2/1x4oox X 0 f8", {1, 5, 23, 171, 1094, 7508, 47807, 322940, 2032799}},
    {"1xox1xoo1/x1o2oo2/ooxo1oo2/x2o2xox/1ooxox1ox/1x3x2x/1x1x1xoxx/oxx2x2o/xx1o1ooo1 X 0 d7", {1, 22, 163, 1457, 10431, 82349, 519427, 3451682, 17775153}},
    {"ooxo1ooox/xxoxoxxxo/oxoxxooox/xoxxxox1o/oxo1oo1xo/xo1o3xx/xxxoxxx1o/4oxo1o/2oooxxx1 X 0 e8", {1, 3, 4, 1, 0, 0, 0, 0, 0}},
    {"xoooxooxx/ooxxox1o1/xxo2xxxo/xxox1x2o/ooxxoxox1/1xooxxooo/x2oo1xo1/x1oxoxox1/x1ooox1ox X 0 g9", {1, 3, 6, 5, 0, 0, 0, 0, 0}},
    {"1ooo1xoox/ooxo1o1xo/xoxxx1oxx/x2xx1xox/1x1oxoxxo/1ox1xoox1/oxoxxoo1o/xooooxo1x/xx1ooxxo1 X 0 a3", {1, 9, 35, 123, 327, 695, 1090, 1359, 896}},
    {"oxx2x2o/xx1xxo1ox/1xxxxoo2/xoxoo1o1x/oxooooxx1/oxoxoxoxo/ooxoooox1/ooxxoo1xx/xx1x2oxo X 0 b2", {1, 3, 4, 2, 0, 0, 0, 0, 0}},
    {"1oox1oxox/xo2x1o1o/o2xxxxxo/oxx3xoo/x1oxoooox/ooxxxxxoo/xxxxo1oox/1o1oxxxox/o2ooooxx X 0 h5", {1, 2, 1, 0, 0, 0, 0, 0, 0}},
    {"oxxooxoox/o1o1xxxxo/1xxooxxoo/xoo2x2x/x2o2o1o/oxxoo1xxo/oxoxxoxxo/1x1xoxo1x/xoooxoxo1 X 0 d4", {1, 2, 5, 29, 118, 451, 1452, 4785, 12074}},
    {"x1x1xo1xx/ooooxoo2/o2o1xxxx/xoox1xxoo/1xxooxo1x/xxxxxoxoo/o2ooooxo/xoxx1ooo1/oxoxo2xx X 0 d3", {1, 6, 16, 26, 41, 19, 6, 0, 0}},
    {"ooooxoxx1/x1oxxooxo/x1xxoo1x1/1xxoxox1o/xooo1xoxx/ooxx1xoxx/ooxxxoxoo/o1xx1o2o/2o2xxoo X 0 f7", {1, 9, 26, 71, 140, 284, 357, 338, 194}},
    {"oox2xxox/xx3xoxo/x1o2xo1x/1o1ooox1o/1x2xooox/ox1x2ox1/x2o1oo1x/1ox2xx2/ox3o1o1 O 0 h8", {1, 0, 0, 0, 0, 0, 0, 0}}
};

void genOpeningsImpl(Board& board, int depth)
{
    if (depth == 0)
    {
        std::cout << board.fenStr() << std::endl;
        return;
    }
    MoveList moves;

    if (board.isLost() || board.isDrawn())
        return;

    genMoves(board, moves);
    for (Move move : moves)
    {
        board.makeMove(move);
        genOpeningsImpl(board, depth - 1);
        board.unmakeMove();
    }
}

void genOpenings(int depth)
{
    Board board;
    genOpeningsImpl(board, depth);
}

void runPerftSuite()
{
    Board board;
    uint64_t totalNodes = 0;
    auto t1 = std::chrono::steady_clock::now();
    for (auto& test : perftTests)
    {
        board.setToFen(test.first);
        std::cout << "Testing fen: " << test.first << std::endl;
        for (int depth = 0; depth < 9; depth++)
        {
            uint64_t result = perft<false>(board, depth);
            totalNodes += result;
            bool match = result == test.second[depth];
            if (match)
                std::cout << "\tPassed depth " << depth << std::endl;
            else
                std::cout << "\tFailed depth " << depth << " Expected: " << test.second[depth] << " Got: " << result << std::endl;
        }
    }
    auto t2 = std::chrono::steady_clock::now();
    float time = std::chrono::duration_cast<std::chrono::duration<float>>(t2 - t1).count();
    std::cout << "Total nodes: " << totalNodes << std::endl;
    std::cout << "Total time: " << time << std::endl;
    std::cout << "NPS: " << std::fixed << totalNodes / time << std::endl;
}

int main(int argc, char** argv)
{
    if (argc > 1 && std::string(argv[1]) == "datagen")
    {
        std::cout << "Doing datagen" << std::endl;
        int numThreads = argc > 2 ? std::stoi(argv[2]) : 1;
        std::string filename = argc > 3 ? argv[3] : "positions";
        std::cout << "Threads: " << numThreads << " filename: " << filename << std::endl;
        runDatagen(numThreads, filename);
        return 0;
    }

    std::random_device rd;
    std::mt19937 rng(rd());
    //runPerftSuite();
    std::string command;
    Board currBoard;
    Color player1 = Color::X;

    Search search;

    while (std::getline(std::cin, command))
    {
        std::istringstream ss(command);
        std::string tok;
        ss >> tok;
        if (tok == "ugi")
        {
            std::cout << "id name utttUnnamed" << std::endl;
            std::cout << "id author mcthoaucbb" << std::endl;
            std::cout << "ugiok" << std::endl;
        }
        else if (tok == "isready")
        {
            std::cout << "readyok" << std::endl;
        }
        else if (tok == "uginewgame")
        {
            currBoard.setToFen("9/9/9/9/9/9/9/9/9 X 0 0000");
            player1 = currBoard.sideToMove();
            search.newGame();
        }
        else if (tok == "position")
        {
            ss >> tok;
            if (tok == "startpos")
            {
                currBoard.setToFen("9/9/9/9/9/9/9/9/9 X 0 0000");
                player1 = currBoard.sideToMove();
                if (ss)
                {
                    ss >> tok;
                    if (tok == "moves")
                    {
                        while (ss.tellg() != -1)
                        {
                            ss >> tok;
                            Move move = findMoveFromStr(currBoard, tok);
                            currBoard.makeMove(move);
                        }
                    }
                }
            }
            else if (tok == "fen")
            {
                std::string fen;
                ss >> fen;
                while (ss.tellg() != -1)
                {
                    ss >> tok;
                    if (tok == "moves")
                        break;
                    fen += ' ' + tok;
                }
                currBoard.setToFen(fen);
                player1 = currBoard.sideToMove();
                if (tok == "moves")
                {
                    while (ss.tellg() != -1)
                    {
                        ss >> tok;
                        Move move = findMoveFromStr(currBoard, tok);
                        currBoard.makeMove(move);
                    }
                }
            }
            else
            {
                std::cout << "info string invalid command" << std::endl;
            }
        }
        else if (tok == "query")
        {
            ss >> tok;
            if (tok == "gameover")
            {
                if (currBoard.isLost() || currBoard.isDrawn())
                    std::cout << "response true" << std::endl;
                else
                    std::cout << "response false" << std::endl;
            }
            else if (tok == "p1turn")
            {
                if (currBoard.sideToMove() == player1)
                    std::cout << "response true" << std::endl;
                else
                    std::cout << "response false" << std::endl;
            }
            else if (tok == "result")
            {
                if (currBoard.isLost())
                {
                    if (currBoard.sideToMove() == player1)
                        std::cout << "response p2win" << std::endl;
                    else
                        std::cout << "response p1win" << std::endl;
                }
                else if (currBoard.isDrawn())
                    std::cout << "response draw" << std::endl;
                else
                    std::cout << "response none" << std::endl;
            }
            else
            {
                std::cout << "info string invalid command" << std::endl;
            }
        }
        else if (tok == "go")
        {
            SearchLimits limits = {};
            limits.maxDepth = 127;
            while (ss.tellg() != -1)
            {
                ss >> tok;
                if (tok == "p1time")
                {
                    if (currBoard.sideToMove() == player1)
                    {
                        int time;
                        ss >> time;
                        limits.clock.time = Duration(time);
                        limits.clock.enabled = true;
                    }
                }
                else if (tok == "p2time")
                {
                    if (currBoard.sideToMove() != player1)
                    {
                        int time;
                        ss >> time;
                        limits.clock.time = Duration(time);
                        limits.clock.enabled = true;
                    }
                }
                else if (tok == "p1inc")
                {
                    if (currBoard.sideToMove() == player1)
                    {
                        int time;
                        ss >> time;
                        limits.clock.inc = Duration(time);
                        limits.clock.enabled = true;
                    }
                }
                else if (tok == "p2inc")
                {
                    if (currBoard.sideToMove() != player1)
                    {
                        int time;
                        ss >> time;
                        limits.clock.inc = Duration(time);
                        limits.clock.enabled = true;
                    }
                }
                // L movestogo
                else if (tok == "depth")
                {
                    // todo
                }
                else if (tok == "nodes")
                {
                    // todo
                }
                else if (tok == "mate")
                {
                    // todo
                }
                else if (tok == "movetime")
                {
                    int time;
                    ss >> time;
                    limits.maxTime = Duration(time);
                }
                else if (tok == "infinite")
                {
                    // nothing to be done
                }
            }

            search.setBoard(currBoard);
            auto result = search.runSearch(limits);
            std::cout << "bestmove " << result.bestMove.to.toString() << std::endl;
        }
        else if (tok == "d")
        {
            std::cout << currBoard.stringRep();
            std::cout << "Side to move: " << (currBoard.sideToMove() == Color::X ? 'X' : 'O') << std::endl;
            std::cout << "Zobrist hash: " << std::hex << (currBoard.key().value) << std::dec << std::endl;
        }
        else if (tok == "genopenings")
        {
            int ply;
            ss >> ply;
            genOpenings(ply);
        }
        else if (tok == "bench")
        {
            runBench();
        }
        else if (tok == "perft")
        {
            int depth;
            ss >> depth;
            auto t1 = std::chrono::steady_clock::now();
            int nodes = perft<true>(currBoard, depth);
            auto t2 = std::chrono::steady_clock::now();

            double time = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count();
            std::cout << "Total nodes: " << nodes << std::endl;
            std::cout << "NPS: " << std::fixed << nodes / time << std::endl;
        }
        else if (tok == "tune")
        {
            tuneMain();
        }
        else
        {
            std::cout << "info string invalid command" << std::endl;
        }
    }
    return 0;
}