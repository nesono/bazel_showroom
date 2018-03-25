#include <iostream>

#include "pilib.h"
#include "platform.h"

int main(int argc, char** argv)
{
    std::cout << "Today: " << __DATE__ << std::endl;
    std::cout << "Time:  " << __TIME__ << std::endl;
    std::cout << "Hello Ma(i)n" << std::endl;
    pi::printPi();
    platform::printPlatform();
    return 0;
}
