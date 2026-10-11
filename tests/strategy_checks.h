#include "memory.h"

#include <cassert>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

// Isolate expected fatal errors so the suite can continue.
static void expect_fatal(int scenario) {
    const pid_t child = fork();
    assert(child >= 0);
    if (child == 0) {
        if (scenario == 0) {
            int unrelated = 0;
            dealloc(&unrelated);
        } else if (scenario == 1) {
            void* p = alloc(20);
            assert(p);
            dealloc(p);
            dealloc(p);
        } else if (scenario == 2) {
            dealloc(nullptr);
        } else {
            char* p = static_cast<char*>(alloc(20));
            assert(p);
            dealloc(p + 1);
        }
        _exit(0);
    }
    int status = 0;
    assert(waitpid(child, &status, 0) == child);
    assert(WIFEXITED(status) && WEXITSTATUS(status) == EXIT_FAILURE);
}

int main() {
    for (int scenario = 0; scenario < 4; ++scenario) {
        expect_fatal(scenario);
    }
    assert(alloc(0) == nullptr && errno == EINVAL);
    assert(alloc(513) == nullptr && errno == EINVAL);
    void* large = alloc(500);
    void* small = alloc(50);
    void* medium = alloc(100);
    assert(large && small && medium);
    std::memset(medium, 0xA5, 100);
    dealloc(large); // Not the last allocation.
    dealloc(small);
    void* chosen = alloc(40);
    assert(chosen == (EXPECT_FIRST_FIT ? large : small));
    for (int i = 0; i < 100; ++i) {
        assert(static_cast<unsigned char*>(medium)[i] == 0xA5);
    }
    // Reuse a larger partition if the exact class has no free chunks.
    void* remaining = alloc(EXPECT_FIRST_FIT ? 40 : 200);
    assert(remaining == (EXPECT_FIRST_FIT ? small : large));
    dealloc(chosen);
    dealloc(remaining);
    dealloc(medium);

    void* a = alloc(512);
    void* b = alloc(512);
    assert(a && b && a != b);
    dealloc(b);
    dealloc(a);
    assert(alloc(512) == b); // FIFO order within an equal-size class.
    assert(alloc(512) == a);
    std::ostringstream output;
    std::streambuf* original = std::cout.rdbuf(output.rdbuf());
    print_lists();
    std::cout.rdbuf(original);
    assert(output.str().find("Allocated list:") != std::string::npos);
    assert(output.str().find("Used: 512") != std::string::npos);
}
