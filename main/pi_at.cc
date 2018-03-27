#include <gtest/gtest.h>

#include "pilib/include/pilib.h"

TEST(pitest, getpi)
{
    ASSERT_FLOAT_EQ(3.1415927f, pi::pi());
}
