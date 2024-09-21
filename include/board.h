#include <vector>
#include <optional>

#include "types.h"
#include "hashcat.h"

void InitializeMagicBitboards();


class Move {
    public:
    enum class Promotion { None, Queen, Rook, Bishop, Knight };
    Move() = default;

    constexpr Move(BoardSquare from, BoardSquare to, BoardSquare duck, Promotion promotion): 
                   data_(to.as_int() + (from.as_int() << 6) + (duck.as_int() << 12) + (static_cast<uint8_t>(promotion) << 18)) {}
    constexpr Move(BoardSquare from, BoardSquare to, BoardSquare duck): 
                   data_(to.as_int() + (from.as_int() << 6) + (duck.as_int() << 12)) {}

    BoardSquare to() const { return BoardSquare(data_ & kToMask); }
    BoardSquare from() const { return BoardSquare((data_ & kFromMask) >> 6); }
    BoardSquare duck() const { return BoardSquare((data_ & kDuckMask) >> 12); }
    Promotion promotion() const { return Promotion((data_ & kPromoMask) >> 18); }

    void SetTo(BoardSquare to) { data_ = (data_ & ~kToMask) | to.as_int(); }
    void SetFrom(BoardSquare from) { data_ = (data_ & ~kFromMask) | (from.as_int() << 6); }
    void SetDuck(BoardSquare duck) { data_ = (data_ & ~kDuckMask) | (duck.as_int() << 12); }
    void SetPromotion(Promotion promotion) { data_ = (data_ & ~kPromoMask) | (static_cast<uint8_t>(promotion) << 18); }


    void Mirror() { data_ ^= 0b111000111000111000; }

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

   DuckBoard() = default;
   DuckBoard(const DuckBoard&) = default;
   DuckBoard(const std::string& fen) { SetFromFen(fen); }

   DuckBoard& operator=(const DuckBoard&) = default;

   static const char* kStartposFen;

   void Clear();
   void Mirror();

   void SetFromFen(std::string fen, int* rule50_ply = nullptr, int* moves = nullptr);

   Square duck_after_move(BoardSquare source, BoardSquare destination) const {
      return ~(((us_ | them_ | duck_) - source) | destination);
   }

   Square occupied() const {
    return us_ | them_ | duck_;
   }

   bool has_us_king() const {
    return us_.get(us_king_);
   }

   bool has_them_king() const {
    return them_.get(them_king_);
   }

   std::optional<BoardSquare> them_king() const {
    if (has_them_king()) {
        return them_king_;
    } else {
        return std::nullopt;
    }
   }
   std::optional<BoardSquare> us_king() const {
    if (has_us_king()) {
        return us_king_;
    } else {
        return std::nullopt;
    }
   }
   std::optional<BoardSquare> duck() const {
       if (has_duck_) {
           return duck_;
       }
       return std::nullopt;
   }

   Square kings() const {
    return (us_ | them_) & (us_king_.as_square() | them_king_.as_square());
   }
   Square ours() const { return us_; }
   Square theirs() const { return them_; }
   Square pawns() const { return pawns_; }
   Square bishops() const { return bishops_ - rooks_; }
   Square rooks() const { return rooks_ - bishops_; }
   Square queens() const { return rooks_ & bishops_; }
   Square knights() const {
    auto res = (us_ | them_) - pawns() - rooks_ - bishops_;
    if (us_king().has_value()) {
        res = res - us_king().value();
    }
    if (them_king().has_value()) {
        res = res - them_king().value();
    }
    return res;
   }

   bool flipped() const { return flipped_; }

   bool operator==(const DuckBoard& other) const {
    return (us_ == other.us_) &&
    (them_ == other.them_) && (rooks_ == other.rooks_) &&
    (bishops_ == other.bishops_) && (pawns_ == other.pawns_) &&
    (has_us_king() == other.has_us_king() && has_us_king() ? us_king_ == other.us_king_ : true) &&
    (has_them_king() == other.has_them_king() && has_them_king() ? them_king_ == other.them_king_ : true) &&
    (has_duck_ == other.has_duck_ && has_duck_ ? duck_ == other.duck_ : true) &&
    (flipped_ == other.flipped_);

   }

   MoveList GenerateMoves() const;

   bool ApplyMove(Move move);

   std::string DebugString() const;

   uint64_t Hash() const {
    auto us_king = has_us_king() ? us_king_ : -1;
    auto them_king = has_them_king() ? them_king_ : -1;
    auto duck = has_duck_ ? duck_ : -1;

    return HashCat({us_.as_int(), them_.as_int(), rooks_.as_int(),
                    bishops_.as_int(), pawns_.as_int(),
                    (static_cast<uint32_t>(us_king.as_int()) << 24) |
                        (static_cast<uint32_t>(them_king.as_int()) << 16) |
                        (static_cast<uint32_t>(duck.as_int()) << 8) |
                        (static_cast<uint32_t>(flipped_))});
   }

   private:

    // is blacks turn
    bool flipped_;
    Square us_;
    Square them_;
    Square rooks_;
    Square bishops_;
    Square pawns_;
    BoardSquare us_king_;
    BoardSquare them_king_;
    BoardSquare duck_;
    bool has_duck_;
};