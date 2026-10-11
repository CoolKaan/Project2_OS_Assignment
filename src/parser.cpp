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
        std::cerr << "Cannot open datafile: " << filename << std::endl;
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
            std::string amount;
            if (!(input >> amount) || (input >> extra) ||
                amount.find_first_not_of("0123456789") != std::string::npos) {
                std::cerr << "Invalid allocation at line " << line_number << std::endl;
                return false;
            }

            // Bound during parsing so neither signed input nor integer
            // overflow can wrap into an otherwise valid allocation size.
            std::size_t bytes = 0;
            for (char digit : amount) {
                bytes = bytes * 10 + static_cast<std::size_t>(digit - '0');
                if (bytes > 512) {
                    std::cerr << "Allocation size exceeds 512 at line "
                              << line_number << std::endl;
                    return false;
                }
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
            std::cerr << "Invalid command at line " << line_number << std::endl;
            return false;
        }
    }

    if (file.bad()) {
        std::cerr << "Error reading datafile: " << filename << std::endl;
        return false;
    }
    return true;
}
