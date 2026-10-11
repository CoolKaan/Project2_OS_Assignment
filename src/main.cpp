#include "../include/parser.h"
#include "../include/memory.h"

#include <iostream>
#include <new>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <datafile>\n";
        return 1;
    }

    try {
        if (!process_df(argv[1])) {
            return 1;
        }
        print_lists();
    } catch (const std::bad_alloc&) {
        std::cerr << "Out of memory while processing the datafile.\n";
        return 1;
    }

    return 0;
}
