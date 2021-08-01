#include <stdlib.h>
#include <Tracy.hpp>
#include <stdint.h>
constexpr int depth = 5;

void* operator new(size_t count) {
    auto ptr = malloc(count);
    TracyAllocS(ptr, count, depth);
    return ptr;
}
void operator delete(void* ptr) noexcept {
    TracyFreeS(ptr, depth);
    free(ptr);
}
