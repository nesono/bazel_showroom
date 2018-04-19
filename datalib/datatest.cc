#include <gtest/gtest.h>
#include <fstream>

#include "datalib.h"

TEST(DataTest, ReadFile)
{
    ASSERT_EQ(datalib::consume_data(), "1234567890987654321");
}
