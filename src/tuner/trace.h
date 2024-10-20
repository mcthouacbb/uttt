#pragma once

#include "../types.h"

template<typename T>
struct ColorArray : public std::array<T, 2>
{
    using std::array<T, 2>::operator[];

    T& operator[](Color p)
    {
        return (*this)[static_cast<int>(p)];
    }

    const T& operator[](Color p) const
    {
        return (*this)[static_cast<int>(p)];
    }
};

using TraceElem = ColorArray<int>;

#define TRACE_OFFSET(elem) (offsetof(Trace, elem) / sizeof(TraceElem))
#define TRACE_SIZE(elem) (sizeof(Trace::elem) / sizeof(TraceElem))

#define TRACE_INC(traceElem) currTrace.traceElem[us]++
#define TRACE_ADD(traceElem, amount) currTrace.traceElem[us] += amount

struct Trace
{
    TraceElem psqt[9];

    double egScale;
};