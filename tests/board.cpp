#include <gtest/gtest.h>
#include "board.h"


TEST(BoardTests, FirstTest) {
    EXPECT_EQ(DuckBoard(DuckBoard::kStartposFen).DebugString(), R"(
rnbqkbnr
pppppppp
........
........
........
........
PPPPPPPP
RNBQKBNR (from white's eyes)
)");
}


TEST(BoardTests, GenerateMoves) {
    auto d = DuckBoard(DuckBoard::kStartposFen);
    EXPECT_EQ(d.GenerateMoves().size(), 640u);
}


TEST(BoardTests, PlayMoves) {
    auto d = DuckBoard(DuckBoard::kStartposFen);

    d.ApplyMove(Move(Square(E2).as_board_square(), Square(E4).as_board_square(), Square(E3).as_board_square()));
    EXPECT_EQ(d.DebugString(), R"(
rnbqkbnr
pppppppp
........
........
....P...
....d...
PPPP.PPP
RNBQKBNR (from white's eyes)
)");
}