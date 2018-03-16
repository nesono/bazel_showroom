#include "pilib.h"
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
