#include "tools/buildstamp/build_info_last_change.h"

#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
    std::cout << "BUILD INFORMATION" << std::endl;
    std::cout << "    buildRevision   " << lastChangeBuildRevision() << endl;
    std::cout << "    buildStatus     " << lastChangeBuildStatus()   << endl;
    std::cout << "    buildTimestamp  " << lastChangeBuildTimestamp()   << endl;
    std::cout << "    buildBranch     " << lastChangeBuildEmbedLabel()   << endl;

    return 0;
}
