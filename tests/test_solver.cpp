#include <gtest/gtest.h>
#include "solver.h"

TEST(Solver, SolvesEquation)
{
    float x1 = 0;
    float x2 = 0;

    solve(1, -5, 6, x1, x2);

    EXPECT_FLOAT_EQ(x1, 2);
    EXPECT_FLOAT_EQ(x2, 3);
}

TEST(Solver, DoubleRoot)
{
    float x1 = 0;
    float x2 = 0;

    solve(1, -2, 1, x1, x2);

    EXPECT_FLOAT_EQ(x1, 1);
    EXPECT_FLOAT_EQ(x2, 1);
}
