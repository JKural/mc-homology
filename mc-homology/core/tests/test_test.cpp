#include <core/parser.h>
#include <gtest/gtest.h>

#include "core/polymorphic.h"

using namespace core;

TEST(TestyTest, TestTest) {
    Polymorphic<MinecraftSavefileParser> parser {
        MinecraftSavefileParser_mcSavefileParsers {}
    };
    auto complex = parser->parse(
        "/home/mekkor/Repositories/mc-homology/mc-homology/core/tests/region",
        {0, 0, 0},
        {16, 16, 16}
    );
    auto homology = complex->z2_homology();
    FAIL();
}
