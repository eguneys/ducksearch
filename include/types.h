#pragma once

#include <string>
#include <optional>

using square = unsigned long long;
using i_square = square;

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

constexpr i_square A1 = 0x0000000000000001ULL;
constexpr i_square B1 = A1 << 1;
constexpr i_square C1 = A1 << 2;
constexpr i_square D1 = A1 << 3;
constexpr i_square E1 = A1 << 4;
constexpr i_square F1 = A1 << 5;
constexpr i_square G1 = A1 << 6;
constexpr i_square H1 = A1 << 7;
constexpr i_square A2 = A1 << 8;
constexpr i_square B2 = A1 << 9;
constexpr i_square C2 = A1 << 10;
constexpr i_square D2 = A1 << 11;
constexpr i_square E2 = A1 << 12;
constexpr i_square F2 = A1 << 13;
constexpr i_square G2 = A1 << 14;
constexpr i_square H2 = A1 << 15;
constexpr i_square A3 = A1 << 16;
constexpr i_square B3 = A1 << 17;
constexpr i_square C3 = A1 << 18;
constexpr i_square D3 = A1 << 19;
constexpr i_square E3 = A1 << 20;
constexpr i_square F3 = A1 << 21;
constexpr i_square G3 = A1 << 22;
constexpr i_square H3 = A1 << 23;
constexpr i_square A4 = A1 << 24;
constexpr i_square B4 = A1 << 25;
constexpr i_square C4 = A1 << 26;
constexpr i_square D4 = A1 << 27;
constexpr i_square E4 = A1 << 28;
constexpr i_square F4 = A1 << 29;
constexpr i_square G4 = A1 << 30;
constexpr i_square H4 = A1 << 31;
constexpr i_square A5 = A1 << 32;
constexpr i_square B5 = A1 << 33;
constexpr i_square C5 = A1 << 34;
constexpr i_square D5 = A1 << 35;
constexpr i_square E5 = A1 << 36;
constexpr i_square F5 = A1 << 37;
constexpr i_square G5 = A1 << 38;
constexpr i_square H5 = A1 << 39;
constexpr i_square A6 = A1 << 40;
constexpr i_square B6 = A1 << 41;
constexpr i_square C6 = A1 << 42;
constexpr i_square D6 = A1 << 43;
constexpr i_square E6 = A1 << 44;
constexpr i_square F6 = A1 << 45;
constexpr i_square G6 = A1 << 46;
constexpr i_square H6 = A1 << 47;
constexpr i_square A7 = A1 << 48;
constexpr i_square B7 = A1 << 49;
constexpr i_square C7 = A1 << 50;
constexpr i_square D7 = A1 << 51;
constexpr i_square E7 = A1 << 52;
constexpr i_square F7 = A1 << 53;
constexpr i_square G7 = A1 << 54;
constexpr i_square H7 = A1 << 55;
constexpr i_square A8 = A1 << 56;
constexpr i_square B8 = A1 << 57;
constexpr i_square C8 = A1 << 58;
constexpr i_square D8 = A1 << 59;
constexpr i_square E8 = A1 << 60;
constexpr i_square F8 = A1 << 61;
constexpr i_square G8 = A1 << 62;
constexpr i_square H8 = A1 << 63;


class BoardSquare {
   public:
    BoardSquare(short value) : value_(value) {}

    static std::optional<BoardSquare> make(short row, short col) {
        if (row >= 0 && row < 8 && col >= 0 && col < 8) {
            return BoardSquare(row * 8 + col);
        }
        return std::nullopt;
    }

    int row() const { return value_ / 8; }
    int col() const { return value_ % 8; }

    constexpr bool operator==(const BoardSquare& other) const {
        return value_ == other.value_;
    }

    constexpr bool operator!=(const BoardSquare& other) const {
        return value_ != other.value_;
    }

    constexpr i_square as_square() const { return 1ULL << value_; }

    std::string as_string() const {
        return std::string(1, 'a' + col()) + std::string(1, '1' + row());
    }

    class BoardSquareIterator {
       public:
        BoardSquareIterator(square value) : value_(value) {}

        BoardSquare operator*() const { return BoardSquare(value_); }

        BoardSquareIterator& operator++() {
            ++value_;
            return *this;
        }

        bool operator!=(const BoardSquareIterator& other) const {
            return value_ != other.value_;
        }

       private:
        short value_;
    };

    BoardSquareIterator begin() const { return BoardSquareIterator(value_); }

    BoardSquareIterator end() const { return BoardSquareIterator(64); }

   private:
    short value_;
};

class Square {
   public:
    Square(square value) : value_(value) {}


    bool operator==(const Square& other) const {
        return value_ == other.value_;
    }

    bool operator!=(const Square& other) const {
        return value_ != other.value_;
    }

    friend Square operator|(const Square& a, const Square& b) { return a.value_ | b.value_; }
    friend Square operator&(const Square& a, const Square& b) { return a.value_ & b.value_; }
    friend Square operator-(const Square& a, const Square& b) { return a.value_ & ~b.value_; }
    friend Square operator-(const Square& a, const BoardSquare& b) { return a.value_ & ~b.as_square(); }

    std::string as_string() {
        std::string res;

        for (short rank = 8; rank >= 1; rank--) {
            res += '\n';
            for (short file = 1; file <= 8; file++) {
                square ptr = (file - 1) + (rank - 1) * 8;
                bool it = (value_ & (1ULL << ptr)) != 0;
                res += (it ? 'o' : '.');
            }
        }
        return res;
    }

    class BitIterator {
        public:
        BitIterator(square value, short position): data(value), ptr(position) {}

        bool operator*() const {
            return (data & (1ULL << ptr)) != 0;
        }

        BitIterator& operator++() {
            ++ptr;
            return *this;
        }


        bool operator!=(const BitIterator& other) const {
            return ptr != other.ptr && data != other.data;
        }

        private:
        square data;
        square ptr;
    };

    BitIterator begin() const {
        return BitIterator(value_, 0);
    }

    BitIterator end() const {
        return BitIterator(value_, 64);
    }



   private:
    square value_;
};
