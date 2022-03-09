#include "pilib/pilib.h" // adhere to (strip_)include_prefix
#include "constants.h"
#include "tools/buildstamp/build_info_last_change.h"

#include <iostream>

namespace pi
{

void printPi()
{
    std::cout << "buildRevision   " << lastChangeBuildRevision() << std::endl;
    std::cout << "Pi is: " <<  prvt::pi << std::endl;
}

float pi()
{
    return prvt::pi;
}

}
