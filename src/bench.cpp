#include "search.h"

const char* benchFens[] = {
    "9/9/9/9/9/9/9/9/9 X 0 0000",
    "9/9/9/9/9/7x1/9/5o3/xo2x4 O 0 h4",
    "x8/1o7/9/6o2/5x3/9/3x5/9/9 O 0 a9",
    "9/6x2/9/2x6/o8/x8/9/o8/9 O 0 c6",
    "9/3x5/3oo4/4x2o1/1o7/3x1o3/4o2x1/1x3x3/ox3o1x1 X 0 h6",
    "4oo1x1/9/8x/9/3x5/o2x5/1xo6/1o5o1/xo1x1x1o1 X 0 e9",
    "1x7/6ox1/o2o4o/5o3/9/4o4/2xx2o1x/4x4/1ox3x2 X 0 g8",
    "1x3o3/7x1/5o2x/1x3o3/5o1x1/o2o5/3o5/xx7/1x4o2 X 0 f7",
    "7o1/3xx2xx/1x1o2x1x/2oo1o2o/1x2oxoo1/1x3xo2/1xx1oo3/2x2oo1x/o3x3o X 0 f2",
    "3o1o3/2xo1x1xx/xox6/6xxo/8o/2x2oxxo/ox2o1ox1/2o5o/o2xox1xo X 0 f4",
    "o1xxox2o/2ox2o2/x3o1o1x/3xx2o1/2x4x1/o2o2x2/oxx1o4/1o2o3o/xxo1x4 X 0 c1",
    "2ox4x/o2xx1x1o/o2x2oo1/1ox2o3/2oo3x1/x1xo3x1/x1xxo2o1/1xo3o2/2o5x X 0 c5",
    "ooox5/x5ox1/o2xo2o1/1xxx1xoo1/2x2o1ox/o3x3o/x2x5/ooxo1x3/6x2 X 0 g6",
    "xox3x2/o2ox1oo1/3x4x/4x2oo/xxx2oo1x/xox1o4/o1o2xoo1/5x3/1o1x5 X 0 b9",
    "1o1x1o2x/o1x2oxox/2xxx1ox1/o3xoxo1/xxx2oooo/o1o1xo1xx/2o2o1x1/x2o2xxx/xxoooo2o X 0 f6",
    "o3xxo1o/xxxxoxxoo/1x1xx1xx1/1o2oxooo/1o2oxxxo/1oxx2xx1/xo1ooo1ox/o1x2o3/oo7 X 0 h8",
    "1xxxoxxoo/1xx2xo1x/oxox1oo1x/oo1o2o1o/2xx1o1ox/1ooo3oo/o1xx1o3/x2xxoxxx/7ox X 0 g7",
    "x1o1ox1xo/xo3ooxx/3o1xoxx/1oxoxx2o/xxxoxooo1/xo3o2x/1x1x1o2o/1oxox1o2/1x4oox X 0 f8",
    "1xox1xoo1/x1o2oo2/ooxo1oo2/x2o2xox/1ooxox1ox/1x3x2x/1x1x1xoxx/oxx2x2o/xx1o1ooo1 X 0 d7",
    "ooxo1ooox/xxoxoxxxo/oxoxxooox/xoxxxox1o/oxo1oo1xo/xo1o3xx/xxxoxxx1o/4oxo1o/2oooxxx1 X 0 e8",
    "xoooxooxx/ooxxox1o1/xxo2xxxo/xxox1x2o/ooxxoxox1/1xooxxooo/x2oo1xo1/x1oxoxox1/x1ooox1ox X 0 g9"
    "1ooo1xoox/ooxo1o1xo/xoxxx1oxx/x2xx1xox/1x1oxoxxo/1ox1xoox1/oxoxxoo1o/xooooxo1x/xx1ooxxo1 X 0 a3",
    "oxx2x2o/xx1xxo1ox/1xxxxoo2/xoxoo1o1x/oxooooxx1/oxoxoxoxo/ooxoooox1/ooxxoo1xx/xx1x2oxo X 0 b2",
    "1oox1oxox/xo2x1o1o/o2xxxxxo/oxx3xoo/x1oxoooox/ooxxxxxoo/xxxxo1oox/1o1oxxxox/o2ooooxx X 0 h5",
    "oxxooxoox/o1o1xxxxo/1xxooxxoo/xoo2x2x/x2o2o1o/oxxoo1xxo/oxoxxoxxo/1x1xoxo1x/xoooxoxo1 X 0 d4",
    "x1x1xo1xx/ooooxoo2/o2o1xxxx/xoox1xxoo/1xxooxo1x/xxxxxoxoo/o2ooooxo/xoxx1ooo1/oxoxo2xx X 0 d3",
    "ooooxoxx1/x1oxxooxo/x1xxoo1x1/1xxoxox1o/xooo1xoxx/ooxx1xoxx/ooxxxoxoo/o1xx1o2o/2o2xxoo X 0 f7",
    "oox2xxox/xx3xoxo/x1o2xo1x/1o1ooox1o/1x2xooox/ox1x2ox1/x2o1oo1x/1ox2xx2/ox3o1o1 O 0 h8",
};


void runBench()
{
    constexpr int BENCH_DEPTH = 10;

    SearchLimits limits = {};
    limits.maxDepth = BENCH_DEPTH;
    Search search;
    Board board;
    uint64_t nodes = 0;
    auto t1 = std::chrono::steady_clock::now();
    for (auto fen : benchFens)
    {
        board.setToFen(fen);
        search.setBoard(board);
        nodes += search.runBenchSearch(limits);
    }
    auto t2 = std::chrono::steady_clock::now();
    int nps = nodes / std::chrono::duration_cast<std::chrono::duration<float>>(t2 - t1).count();
    std::cout << nodes << " nodes " << nps << " nps" << std::endl;
}
