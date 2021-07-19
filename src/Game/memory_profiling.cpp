#include <stdlib.h>
#include <Tracy.hpp>

constexpr int depth = 5;

void* operator new(std::size_t count) {
    auto ptr = malloc(count);
    TracyAllocS(ptr, count, depth);
    return ptr;
}
void operator delete(void* ptr) noexcept {
    TracyFreeS(ptr, depth);
    free(ptr);
}
