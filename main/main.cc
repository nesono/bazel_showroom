#include <iostream>

#include "pilib/include/pilib.h"
#include "platformlib/platform.h"

int main(int argc, char** argv)
{
    std::cout << "Today: " << __DATE__ << std::endl;
    std::cout << "Time:  " << __TIME__ << std::endl;
    std::cout << "Hello Ma(i)n" << std::endl;
    pi::printPi();
    platform::printPlatform();
    return 0;
}
