#include "eval_fn.h"
#include "../board.h"
#include "trace.h"
#include "../evaluate.h"
#include "../eval_constants.h"

#include <sstream>
#include <iomanip>

Trace getTrace(const Board& board)
{
    int score = eval::evalTrace(board);
    return eval::currTrace;
}

EvalFn::EvalFn(std::vector<Coefficient>& coefficients)
    : m_Coefficients(coefficients)
{

}

void EvalFn::reset()
{
    m_TraceIdx = 0;
}

std::tuple<size_t, size_t, double> EvalFn::getCoefficients(const Board& board)
{
    reset();
    size_t pos = m_Coefficients.size();
    Trace trace = getTrace(board);
    addCoefficientArray(trace.psqt);

    return {pos, m_Coefficients.size(), trace.egScale};
}

template<typename T>
void addEvalParam(EvalParams& params, const T& t, ParamType type)
{
    params.push_back({type, static_cast<double>(t), 0.0});
}

template<typename T>
void addEvalParamArray(EvalParams& params, const T& t, ParamType type)
{
    for (auto param : t)
        addEvalParam(params, param, type);
}

template<typename T>
void addEvalParamArray2D(EvalParams& params, const T& t, ParamType type)
{
    for (auto& array : t)
        addEvalParamArray(params, array, type);
}

template<typename T>
void addEvalParamArray3D(EvalParams& params, const T& t, ParamType type)
{
    for (auto& array : t)
        addEvalParamArray2D(params, array, type);
}

EvalParams EvalFn::getInitialParams()
{
    EvalParams params;
    addEvalParamArray(params, eval::psqt, ParamType::NORMAL);

    return params;
}

EvalParams EvalFn::getMaterialParams()
{
    return getInitialParams();
    /*EvalParams params = getInitialParams();
    for (auto& param : params)
        param.mg = param.eg = 0;

    for (int i = 0; i < 6; i++)
        for (int j = (i == 0 ? 8 : 0); j < (i == 0 ? 56 : 64); j++)
        {
            params[i * 64 + j].mg += MATERIAL[i].mg();
            params[i * 64 + j].eg += MATERIAL[i].eg();
        }
    return params;*/
}

EvalParams EvalFn::getKParams()
{
    return getInitialParams();
    /*constexpr PackedScore K_MATERIAL[6] = {
        {67, 98}, {311, 409}, {330, 419}, {426, 746}, {860, 1403}, {0, 0}
    };

    EvalParams params = getInitialParams();
    for (auto& param : params)
        param.mg = param.eg = 0;

    for (int i = 0; i < 6; i++)
        for (int j = (i == 0 ? 8 : 0); j < (i == 0 ? 56 : 64); j++)
        {
            params[i * 64 + j].mg += K_MATERIAL[i].mg();
            params[i * 64 + j].eg += K_MATERIAL[i].eg();
        }
    return params;*/
}

struct PrintState
{
    const EvalParams& params;
    uint32_t index;
    std::ostringstream ss;
};

template<int ALIGN_SIZE>
void printSingle(PrintState& state)
{
    const EvalParam& param = state.params[state.index++];
    /*if constexpr (ALIGN_SIZE == 0)
        state.ss << "S(" << param.mg << ", " << param.eg << ')';
    else
        state.ss << "S(" << std::setw(ALIGN_SIZE) << static_cast<int>(param.mg) << ", " << std::setw(ALIGN_SIZE) << static_cast<int>(param.eg) << ')';*/
    if constexpr (ALIGN_SIZE == 0)
        state.ss << param.mg;
    else
        state.ss << std::setw(ALIGN_SIZE) << static_cast<int>(param.mg);
}

template<int ALIGN_SIZE>
void printPSQTs(PrintState& state)
{
    state.ss << "constexpr PackedScore PSQT[9] = {\n";
    for (int y = 0; y < 3; y++)
    {
        state.ss << "\t";
        for (int x = 0; x < 3; x++)
        {
            printSingle<ALIGN_SIZE>(state);
            state.ss << ", ";
        }
        state.ss << "\n";
    }
    state.ss << "};\n";
}

/*void printMaterial(PrintState& state)
{
    state.ss << "constexpr PackedScore MATERIAL[6] = {";
    for (int j = 0; j < 5; j++)
    {
        printSingle<4>(state);
        state.ss << ", ";
    }
    state.ss << "S(0, 0)};\n";
}*/

template<int ALIGN_SIZE>
void printArray(PrintState& state, int len)
{
    state.ss << '{';
    for (int i = 0; i < len; i++)
    {
        printSingle<ALIGN_SIZE>(state);
        if (i != len - 1)
            state.ss << ", ";
    }
    state.ss << '}';
}

template<int ALIGN_SIZE>
void printArray2D(PrintState& state, int outerLen, int innerLen, bool indent = false)
{
    state.ss << "{\n";
    for (int i = 0; i < outerLen; i++)
    {
        state.ss << '\t';
        if (indent)
            state.ss << '\t';
        printArray<ALIGN_SIZE>(state, innerLen);
        if (i != outerLen - 1)
            state.ss << ',';
        state.ss << '\n';
    }
    if (indent)
        state.ss << '\t';
    state.ss << "}";
}

template<int ALIGN_SIZE>
void printArray3D(PrintState& state, int len1, int len2, int len3)
{
    state.ss << "{\n";
    for (int i = 0; i < len1; i++)
    {
        state.ss << '\t';
        printArray2D<ALIGN_SIZE>(state, len2, len3, true);
        if (i != len1 - 1)
            state.ss << ',';
        state.ss << '\n';
    }
    state.ss << "}";
}

template<int ALIGN_SIZE>
void printRestParams(PrintState& state)
{
}

void EvalFn::printEvalParams(const EvalParams& params, std::ostream& os)
{
    PrintState state{params, 0};
    printPSQTs<0>(state);
    state.ss << '\n';
    //printRestParams<0>(state);
    os << state.ss.str() << std::endl;
}

std::array<int, 2> avgValue(EvalParams& params, int offset, int size)
{
    std::array<int, 2> avg = {};
    for (int i = offset; i < offset + size; i++)
    {
        avg[0] += static_cast<int>(params[i].mg);
        avg[1] += static_cast<int>(params[i].eg);
    }
    avg[0] /= size;
    avg[1] /= size;
    return avg;
}

void rebalance(std::array<int, 2> factor, std::array<int, 2>& materialValue, EvalParams& params, int offset, int size)
{
    materialValue[0] += factor[0];
    materialValue[1] += factor[1];

    for (int i = offset; i < offset + size; i++)
    {
        params[i].mg -= static_cast<double>(factor[0]);
        params[i].eg -= static_cast<double>(factor[1]);
    }
}

EvalParams extractMaterial(const EvalParams& params)
{
    return params;
    /*EvalParams rebalanced = params;
    for (auto& elem : rebalanced)
    {
        elem.mg = std::round(elem.mg);
        elem.eg = std::round(elem.eg);
    }

    std::array<std::array<int, 2>, 6> material = {};

    // psqts
    for (int pce = 0; pce < 6; pce++)
    {
        if (pce == 0)
        {
            auto avg = avgValue(rebalanced, TRACE_OFFSET(psqt[0]) + 24, TRACE_SIZE(psqt[0]) - 32);
            rebalance(avg, material[pce], rebalanced, TRACE_OFFSET(psqt[0]) + 8, TRACE_SIZE(psqt[0]) - 16);
        }
        else
        {
            auto avg = avgValue(rebalanced, TRACE_OFFSET(psqt[pce]), TRACE_SIZE(psqt[pce]));
            rebalance(avg, material[pce], rebalanced, TRACE_OFFSET(psqt[pce]), TRACE_SIZE(psqt[pce]));
        }
    }

    // mobility
    rebalance(avgValue(rebalanced, TRACE_OFFSET(mobility[0]), 9), material[1], rebalanced, TRACE_OFFSET(mobility[0]), 9);
    rebalance(avgValue(rebalanced, TRACE_OFFSET(mobility[1]), 14), material[2], rebalanced, TRACE_OFFSET(mobility[1]), 14);
    rebalance(avgValue(rebalanced, TRACE_OFFSET(mobility[2]), 15), material[3], rebalanced, TRACE_OFFSET(mobility[2]), 15);
    rebalance(avgValue(rebalanced, TRACE_OFFSET(mobility[3]), 28), material[4], rebalanced, TRACE_OFFSET(mobility[3]), 28);

    // king attacks
    rebalance(avgValue(rebalanced, TRACE_OFFSET(kingAttacks), TRACE_SIZE(kingAttacks)), material[5], rebalanced, TRACE_OFFSET(kingAttacks), TRACE_SIZE(kingAttacks));

    // bishop pawns
    rebalance(avgValue(rebalanced, TRACE_OFFSET(bishopPawns), TRACE_SIZE(bishopPawns)), material[2], rebalanced, TRACE_OFFSET(bishopPawns), TRACE_SIZE(bishopPawns));

    EvalParams extracted;
    for (int i = 0; i < 5; i++)
    {
        extracted.push_back(EvalParam{ParamType::NORMAL, static_cast<double>(material[i][0]), static_cast<double>(material[i][1])});
    }
    extracted.insert(extracted.end(), rebalanced.begin(), rebalanced.end());
    return extracted;*/
}

void EvalFn::printEvalParamsExtracted(const EvalParams& params, std::ostream& os)
{
    PrintState state{extractMaterial(params), 0};
    //printMaterial(state);
    //state.ss << '\n';
    printPSQTs<4>(state);
    state.ss << '\n';
    //printRestParams<4>(state);
    os << state.ss.str() << std::endl;
}