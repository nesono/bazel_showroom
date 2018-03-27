#include <gtest/gtest.h>
#include <unistd.h>

TEST(main, get_cwd)
{
    char cwd[1024];

    ASSERT_NE(nullptr, getcwd(cwd, sizeof(cwd)));
    std::cout << "Current Working Directory" << std::endl;
    std::cout << cwd << std::endl;
}

