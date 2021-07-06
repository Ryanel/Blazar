#include <memory>
#include <stdlib.h>
#include <iostream>
void* operator new(size_t size) {
    //std::cout << "Alloc of " << size << std::endl;
    return malloc(size);
}