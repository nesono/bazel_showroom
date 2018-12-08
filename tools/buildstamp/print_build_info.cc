#include "build_info.h"

#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
    std::cout << "buildRevision   " << buildRevision() << endl;
    std::cout << "buildStatus     " << buildStatus() << endl;
    std::cout << "buildTimestamp  " << buildTimestamp() << endl;
    std::cout << "buildEmbedLabel " << buildEmbedLabel() << endl;
    std::cout << "buildBranch     " << buildBranch() << endl;

    return 0;
}
