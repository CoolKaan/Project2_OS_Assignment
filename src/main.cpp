#include "../include/parser.h"
#include "../include/memory.h"

#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <datafile>\n";
        return 1;
    }

    if (!process_df(argv[1])) {
        return 1;
    }

    print_lists();

    return 0;
}
