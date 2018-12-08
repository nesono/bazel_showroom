#include <iostream>

#include "pilib/pilib.h"
#include "platformlib/platform.h"
#include "datalib/datalib.h"
#include "tools/buildstamp/build_info.h"

int main(int argc, char** argv)
{
    std::cout << "Today: " << __DATE__ << std::endl;
    std::cout << "Time:  " << __TIME__ << std::endl;
    std::cout << "Hello Ma(i)n" << std::endl;

    std::cout << "buildRevision   " << buildRevision() << std::endl;
    std::cout << "buildStatus     " << buildStatus() << std::endl;
    std::cout << "buildTimestamp  " << buildTimestamp() << std::endl;
    std::cout << "buildEmbedLabel " << buildEmbedLabel() << std::endl;
    std::cout << "buildBranch     " << buildBranch() << std::endl << std::endl;

    pi::printPi();
    platform::printPlatform();
    std::cout << "Consumed data: " << datalib::consume_data() << std::endl;
    return 0;
}
