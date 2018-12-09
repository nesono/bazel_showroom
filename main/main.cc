#include <iostream>

#include "pilib/pilib.h"
#include "platformlib/platform.h"
#include "datalib/datalib.h"

int main(int argc, char** argv)
{
    std::cout << "Today: " << __DATE__ << std::endl;
    std::cout << "Time:  " << __TIME__ << std::endl;
    std::cout << "Hello Ma(i)n" << std::endl;

    pi::printPi();
    platform::printPlatform();
    std::cout << "Consumed data: " << datalib::consume_data() << std::endl;
    return 0;
}
