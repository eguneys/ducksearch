#include "types.h"

void InitializeMagicBitboards();


class DuckBoard {
   public:

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

   private:

    Square whites_;
    Square blacks_;
    Square rooks_;
    Square bishops_;
    Square pawns_;
    BoardSquare white_king_;
    BoardSquare black_king_;
    BoardSquare duck_;

};