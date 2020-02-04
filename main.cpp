#include <iostream>
#include "fast-cpp-csv-parser/csv.h"

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: " << std::endl;
        std::cerr << "For tuple lookup: " << argv[0] << " -L [ID]" << std::endl;
        std::cerr << "Tuple lookup example: " << argv[0] << " -L 4" << std::endl;
        std::cerr << "For index creation: " << argv[0] << " -C" << std::endl;
        return 1;
    }
    std::cout << "Correct number of arguments given" << std::endl;
    return 0;
}
