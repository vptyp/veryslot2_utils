# Circular buffer
[![Linux Build](https://github.com/vptyp/veryslot2_utils/actions/workflows/linux_build.yml/badge.svg)](https://github.com/vptyp/veryslot2_utils/actions/workflows/linux_build.yml) [![Coverage Status](https://coveralls.io/repos/github/vptyp/veryslot2_utils/badge.svg?branch=master)](https://coveralls.io/github/vptyp/veryslot2_utils?branch=master) 

A circular buffer is a data structure that uses a single, fixed-size buffer as if it were connected end-to-end. This implementation based on the circular buffer from the boost library. And some other implementations from the internet.

## Usage

```cpp
#include "circular_buffer.h"
int main() {
    veryslot2::circular_buffer<int> buffer(10);
    for (int i = 0; i < 10; i++) {
        buffer.push_back(i);
    }
    for (int i = 0; i < 10; i++) {
        std::cout << buffer[i] << " ";
    }
    std::cout << std::endl;
    return 0;
}
```

## Details

- This implementation using iterators, and can be used with the standard algorithms.
- Is not thread-safe. If you want to use it in a multi-threaded environment, you should use a mutex.
- Can work only with default-constructible elements. Version 2.0 will support non-default-constructible elements.
- Can be safe for overwrite or not. If it is safe, the push_back operation will return -1 if the buffer is full. Only for push_back. Version 2.0 will support insert_back.

## Current stage

- [x] Basic implementation
- [x] Iterators
- [ ] Version 2.0
  - [ ] Non-default-constructible elements
  - [ ] Non-copyable elements
  - [ ] Non-movable elements
  - [ ] Insert_back 
  - [ ] Overwrite
- [x] Tests without google test
  - [x] Tests for iterators
  - [x] Tests for STL-algorithms
  - [ ] Tests for bad input
  - [ ] Tests for overwrite
  - [ ] Tests for non-default-constructible elements
  - [ ] Tests for non-copyable elements
  - [ ] Tests for non-movable elements
  - [x] Tests for memory leaks and other memory problems (valgrind)
- [ ] Tests with google test
  - [ ] Move tests from the previous paragraph to google test
