#include <iostream>
#include <string>
#include "types.h"

std::string print_square(square value) {
    std::string res;

    for (short rank = 8; rank >= 1; rank--) {
        res += '\n';
        for (short file = 1; file <= 8; file++) {
            square ptr = (file - 1) + (rank - 1) * 8;
            bool it = (value & (1ULL << ptr)) != 0;
            res += (it ? 'o' : '.');
        }
    }
    return res;
}

