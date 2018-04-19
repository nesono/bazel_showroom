#include "datalib.h"

#include <iostream>
#include <fstream>
#include <stdexcept>

namespace datalib{
namespace {
const std::string datafile("datalib/tag");
}

std::string consume_data()
{
    std::ifstream infile;
    infile.open(datafile);
    if (!infile.is_open()) {
        throw std::runtime_error("couldn't open " + datafile);
    }
    std::string tagstring;
    infile >> tagstring;
    return tagstring;
}
}
