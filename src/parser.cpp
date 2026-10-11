#include "../include/parser.h"
#include "../include/memory.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

bool process_df(const char* filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "File not opened" << filename << std::endl;
        return false;
    }

    std::vector<void*> active;
    std::string line;
    std::size_t line_number = 0;

    while (std::getline(file, line)) {
        ++line_number;
        std::istringstream input(line);
        std::string command, extra;

        if (!(input >> command)) continue;

        if (command == "alloc:") {
            std::size_t bytes;
            if (!(input >> bytes) || (input >> extra)) {
                std::cerr << "Invalid allocation at l" << line_number << std::endl;
                return false;
            }

            void* chunk = alloc(bytes);
            if (!chunk) {
                std::cerr << "Allocation failed (l" << line_number << ")" << std::endl;
                return false;
            }
            active.push_back(chunk);

        } else if (command == "dealloc" && !(input >> extra)) {
            if (!active.empty()) {
                dealloc(active.back());
                active.pop_back();
            }
        } else {
            std::cerr << "Invalid" << line_number << std::endl;
            return false;
        }
    }

    if (file.bad()) {
        std::cerr << "Error!!!" << filename << std::endl;
        return false;
    }
    return true;
}
