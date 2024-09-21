#include <vector>
#include "types.h"

void InitializeMagicBitboards();


class Move {
    public:
    enum class Promotion { None, Queen, Rook, Bishop, Knight };
    Move() = default;

    constexpr Move(BoardSquare from, BoardSquare to, Promotion promotion,
                   BoardSquare duck): 
                   data_(to.as_int() + (from.as_int() << 6) + (duck.as_int() << 12) + (static_cast<uint8_t>(promotion) << 18)) {}

    BoardSquare to() const { return BoardSquare(data_ & kToMask); }
    BoardSquare from() const { return BoardSquare((data_ & kFromMask) >> 6); }
    BoardSquare duck() const { return BoardSquare((data_ & kDuckMask) >> 12); }
    Promotion promotion() const { return Promotion((data_ & kPromoMask) >> 18); }

    void SetTo(BoardSquare to) { data_ = (data_ & ~kToMask) | to.as_int(); }
    void SetFrom(BoardSquare from) { data_ = (data_ & ~kFromMask) | (from.as_int() << 6); }
    void SetDuck(BoardSquare duck) { data_ = (data_ & ~kDuckMask) | (duck.as_int() << 12); }
    void SetPromotion(Promotion promotion) { data_ = (data_ & ~kPromoMask) | (static_cast<uint8_t>(promotion) << 18); }


    std::string as_string() const {
        std::string res = duck().as_string() + '@' + from().as_string() + to().as_string();

        switch (promotion()) {
            case Promotion::None:
            return res;
            case Promotion::Queen:
            return res + 'q';
            case Promotion::Rook:
            return res + 'r';
            case Promotion::Bishop:
            return res + 'b';
            case Promotion::Knight:
            return res + 'n';
        }
    }

    private:
    unsigned int data_ = 0;

    //bits 0..5 to
    //bits 6..11 from
    //bits 12..17 duck
    //bits 17..19 promotion

    enum Masks : int {
        kToMask    = 0b0000000000000000111111,
        kFromMask  = 0b0000000000111111000000,
        kDuckMask  = 0b0000111111000000000000,
        kPromoMask = 0b0111000000000000000000,
    };
};

using MoveList = std::vector<Move>;

class DuckBoard {
   public:

   Square turn_pieces() const { return is_turn_white_ ? whites() : blacks(); }
   BoardSquare turn_king() const { return is_turn_white_ ? white_king_ : black_king_; }
   BoardSquare duck() const { return duck_; }
   Square whites() const { return whites_; }
   Square blacks() const { return blacks_; }
   Square pawns() const { return pawns_; }
   Square bishops() const { return bishops_ - rooks_; }
   Square rooks() const { return rooks_ - bishops_; }
   Square queens() const { return rooks_ & bishops_; }
   Square knights() const {
    return (whites_ | blacks_) - pawns() - white_king_ - black_king_ - rooks_ - bishops_;
   }
   Square kings() const {
    return white_king_.as_square() | black_king_.as_square();
   }

   bool operator==(const DuckBoard& other) const {
    return (whites_ == other.whites_) &&
    (blacks_ == other.blacks_) && (rooks_ == other.rooks_) &&
    (bishops_ == other.bishops_) && (pawns_ == other.pawns_) &&
    (white_king_ == other.white_king_) &&
    (black_king_ == other.black_king_) &&
    (duck_ == other.duck_);

   }


   MoveList GenerateMoves() const;

   private:

    bool is_turn_white_;
    Square whites_;
    Square blacks_;
    Square rooks_;
    Square bishops_;
    Square pawns_;
    BoardSquare white_king_;
    BoardSquare black_king_;
    BoardSquare duck_;

};