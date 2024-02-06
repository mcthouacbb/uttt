#include <iostream>

#include "bitboard.h"
#include "attacks.h"

int main()
{
    for (int i = 0; i < 512; i++)
    {
        Bitboard bb(i);
        std::cout << bb;
        bool won = attacks::boardIsWon(bb);
        if (won)
            std::cout << "Win" << std::endl;
        else
            std::cout << "Not winning" << std::endl;
        std::cout << std::endl;
    }
    return 0;
}