#include <gtest/gtest.h>
#include "board.h"

namespace {
    int Perft(const DuckBoard& board, int max_depth, bool dump = false, int depth = 0) {
        if (depth == max_depth) return 1;
        int total_count = 0;
        auto moves = board.GenerateMoves();

        auto iter = moves.begin();

        for (const auto& move: moves) {
            auto new_board = board;
            new_board.ApplyMove(move);
            
            new_board.Mirror();
            ++iter;
            int count = Perft(new_board, max_depth, dump, depth + 1);
            if (dump && depth == 0) {
                Move m = move;
                if (board.flipped()) m.Mirror();
                std::cerr << m.as_string() << ": " << count << '\n';
            }
            total_count += count;
        }
        EXPECT_EQ(iter, moves.end());
        return total_count;
    }
}


TEST(DuckBoard, StartingPosition) {
   DuckBoard board;
   board.SetFromFen(DuckBoard::kStartposFen);

   EXPECT_EQ(Perft(board, 0), 1);
   EXPECT_EQ(Perft(board, 1), 640);
   EXPECT_EQ(Perft(board, 2), 379440);
   //EXPECT_EQ(Perft(board, 3, true), 349921262);
}


TEST(DuckBoard, PerftDebug) {
    DuckBoard board;
    board.SetFromFen(DuckBoard::kStartposFen);

    board.ApplyMove(Move("e3@e2e4"));
    board.Mirror();
    EXPECT_EQ(Perft(board, 2), 543914);
}

TEST(DuckBoard, FirstTest) {
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


TEST(DuckBoard, GenerateMoves) {
    auto d = DuckBoard(DuckBoard::kStartposFen);
    EXPECT_EQ(d.GenerateMoves().size(), 640u);
}


TEST(DuckBoard, PlayMoves) {
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