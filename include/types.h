#pragma once

#include <string>

using square = unsigned long long;

constexpr square EMPTY = 0x0000000000000000ULL;

constexpr square A_FILE = 0x0101010101010101ULL;
constexpr square B_FILE = 0x0202020202020202ULL;
constexpr square C_FILE = 0x0404040404040404ULL;
constexpr square D_FILE = 0x0808080808080808ULL;
constexpr square E_FILE = 0x1010101010101010ULL;
constexpr square F_FILE = 0x2020202020202020ULL;
constexpr square G_FILE = 0x4040404040404040ULL;
constexpr square H_FILE = 0x8080808080808080ULL;

constexpr square RANK_1 = 0x00000000000000FFULL;
constexpr square RANK_2 = RANK_1 << 8;
constexpr square RANK_3 = RANK_1 << 16;
constexpr square RANK_4 = RANK_1 << 24;
constexpr square RANK_5 = RANK_1 << 32;
constexpr square RANK_6 = RANK_1 << 40;
constexpr square RANK_7 = RANK_1 << 48;
constexpr square RANK_8 = RANK_1 << 56;

constexpr square A1 = 0x0000000000000001ULL;
constexpr square B1 = A1 << 1;
constexpr square C1 = A1 << 2;
constexpr square D1 = A1 << 3;
constexpr square E1 = A1 << 4;
constexpr square F1 = A1 << 5;
constexpr square G1 = A1 << 6;
constexpr square H1 = A1 << 7;
constexpr square A2 = A1 << 8;
constexpr square B2 = A1 << 9;
constexpr square C2 = A1 << 10;
constexpr square D2 = A1 << 11;
constexpr square E2 = A1 << 12;
constexpr square F2 = A1 << 13;
constexpr square G2 = A1 << 14;
constexpr square H2 = A1 << 15;
constexpr square A3 = A1 << 16;
constexpr square B3 = A1 << 17;
constexpr square C3 = A1 << 18;
constexpr square D3 = A1 << 19;
constexpr square E3 = A1 << 20;
constexpr square F3 = A1 << 21;
constexpr square G3 = A1 << 22;
constexpr square H3 = A1 << 23;
constexpr square A4 = A1 << 24;
constexpr square B4 = A1 << 25;
constexpr square C4 = A1 << 26;
constexpr square D4 = A1 << 27;
constexpr square E4 = A1 << 28;
constexpr square F4 = A1 << 29;
constexpr square G4 = A1 << 30;
constexpr square H4 = A1 << 31;
constexpr square A5 = A1 << 32;
constexpr square B5 = A1 << 33;
constexpr square C5 = A1 << 34;
constexpr square D5 = A1 << 35;
constexpr square E5 = A1 << 36;
constexpr square F5 = A1 << 37;
constexpr square G5 = A1 << 38;
constexpr square H5 = A1 << 39;
constexpr square A6 = A1 << 40;
constexpr square B6 = A1 << 41;
constexpr square C6 = A1 << 42;
constexpr square D6 = A1 << 43;
constexpr square E6 = A1 << 44;
constexpr square F6 = A1 << 45;
constexpr square G6 = A1 << 46;
constexpr square H6 = A1 << 47;
constexpr square A7 = A1 << 48;
constexpr square B7 = A1 << 49;
constexpr square C7 = A1 << 50;
constexpr square D7 = A1 << 51;
constexpr square E7 = A1 << 52;
constexpr square F7 = A1 << 53;
constexpr square G7 = A1 << 54;
constexpr square H7 = A1 << 55;
constexpr square A8 = A1 << 56;
constexpr square B8 = A1 << 57;
constexpr square C8 = A1 << 58;
constexpr square D8 = A1 << 59;
constexpr square E8 = A1 << 60;
constexpr square F8 = A1 << 61;
constexpr square G8 = A1 << 62;
constexpr square H8 = A1 << 63;

std::string print_square(square value);