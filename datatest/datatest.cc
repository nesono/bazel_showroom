#include <gtest/gtest.h>
#include <fstream>

TEST(DataTest, ReadFile)
{
    std::ifstream infile;
    infile.open("datatest/tag");
    ASSERT_TRUE(infile.is_open());
    std::string tagstring;
    infile >> tagstring;
    ASSERT_EQ(tagstring, "1234567890987654321");
}
