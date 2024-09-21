#include <gtest/gtest.h>
#include "types.h"


TEST(SquaresTest, Operations) {
    EXPECT_EQ(Square(A1 | E8).as_string(), R"(
....o...
........
........
........
........
........
........
o.......)");
}


TEST(SquaresTest, Constants) {
    EXPECT_EQ(Square(A1).as_string(), R"(
........
........
........
........
........
........
........
o.......)");


    EXPECT_EQ(Square(E4).as_string(), R"(
........
........
........
........
....o...
........
........
........)");

    EXPECT_EQ(Square(RANK_1).as_string(), R"(
........
........
........
........
........
........
........
oooooooo)");


    EXPECT_EQ(Square(A_FILE).as_string(), R"(
o.......
o.......
o.......
o.......
o.......
o.......
o.......
o.......)");




}