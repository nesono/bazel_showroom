#include "pilib/pilib.h" // adhere to (strip_)include_prefix
#include "constants.h"
#include "tools/buildstamp/build_info.h"

#include <iostream>

namespace pi
{

void printPi()
{
    std::cout << "buildRevision   " << buildRevision() << std::endl;
    std::cout << "buildStatus     " << buildStatus() << std::endl;
    std::cout << "buildTimestamp  " << buildTimestamp() << std::endl;
    std::cout << "buildEmbedLabel " << buildEmbedLabel() << std::endl;
    std::cout << "buildBranch     " << buildBranch() << std::endl << std::endl;

    std::cout << "Pi is: " <<  prvt::pi << std::endl;
}

float pi()
{
    return prvt::pi;
}

}
