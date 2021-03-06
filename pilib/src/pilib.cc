#include "pilib/pilib.h" // adhere to (strip_)include_prefix
#include "constants.h"
#include "tools/buildstamp/build_info.h"

#include <iostream>

namespace pi
{

void printPi()
{
    std::cout << "buildRevision   " << buildRevision() << std::endl;
    std::cout << "Pi is: " <<  prvt::pi << std::endl;
}

float pi()
{
    return prvt::pi;
}

}
