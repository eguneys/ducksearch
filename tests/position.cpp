#include <gtest/gtest.h>
#include "position.h"




int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    InitializeMagicBitboards();
    return RUN_ALL_TESTS();
}