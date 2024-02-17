#include <iostream>
#include "circular_buffer.h"
#include <algorithm>

int main() {
    veryslot2::circular_buffer<int> buffer(10);
    for (int i = 0; i < 100; i++) {
        if(i == 50) {
            buffer.resize(100);
        }
        buffer.push_back(std::rand() % 100);

        if( i % 10 == 0) {
            std::cout << "value 5 found: ";
            std::sort(buffer.begin(), buffer.end());
            std::cout << *std::lower_bound(buffer.begin(), buffer.end(), 20);
            std::cout << std::endl;
            std::cout << "sorted array: ";
        }
        for(int & it : buffer) {
            std::cout << it << " ";
        }
        if(i % 25 == 0) {
            buffer.clear();
        }
        std::cout << std::endl;
    }
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
