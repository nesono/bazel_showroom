#include "pilib/pilib.h" // adhere to (strip_)include_prefix
#include "constants.h"

#include <iostream>

namespace pi
{

void printPi()
{
    std::cout << "Pi is: " <<  prvt::pi << std::endl;
}

float pi()
{
    return prvt::pi;
}

}
