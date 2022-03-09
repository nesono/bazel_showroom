#include "tools/buildstamp/build_info_built.h"

#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
    std::cout << "BUILD INFORMATION" << std::endl;
    std::cout << " buildEmbedLabel   " << buildEmbedLabel()  << std::endl;
    std::cout << " buildHost         " << buildHost()        << std::endl;
    std::cout << " buildUser         " << buildUser()        << std::endl;
    std::cout << " buildDateStamp    " << buildDateStamp()   << std::endl;
    std::cout << " buildTimeStamp    " << buildTimeStamp()   << std::endl;
    std::cout << " buildScmRevision  " << buildScmRevision() << std::endl;
    std::cout << " buildScmBranch    " << buildScmBranch()   << std::endl;
    std::cout << " buildScmStatus    " << buildScmStatus()   << std::endl;
    return 0;
}
