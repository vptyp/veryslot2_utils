#include <iostream>
#include "circular_buffer.h"
#include <algorithm>
#include <chrono>

int main() {
    QVector<int> m(150);
//    {
    veryslot2::circular_buffer<int> buffer_test(100);

    int counter = 0;
    for (auto &part: m) {
        part = counter++;
    }

    std::cout << buffer_test.insert_back(m.begin(),
                                         m.end()) << std::endl;

    veryslot2::circular_buffer<int> buffer_gtest(buffer_test);
    for (int &val: buffer_test) {
        std::cout << val << " ";
    }

    std::cout << std::endl << buffer_test.size() << std::endl;
    std::cout << buffer_test.insert_back(m.begin() + 20,
                                         m.begin() + 50) << std::endl;

    for (int &val: buffer_test) {
        std::cout << val << " ";
    }

    std::cout << std::endl << buffer_test.size() << std::endl;
    std::cout << buffer_test.insert_back(m.begin() + 10,
                                         m.begin() + 90) << std::endl;

    for (int &val: buffer_test) {
        std::cout << val << " ";
    }

    std::cout << std::endl << buffer_test.size() << std::endl;
//    }
//    veryslot2::circular_buffer<int> buffer_test(100);
    std::vector<int> test(300);
    for(auto& i : test){
        static int t = 0;
        i = t++;
    }

    std::cout << buffer_test.insert_back(test.begin() + 0,
                                         test.begin() + 50) << std::endl;

    for(int & val : buffer_test){
        std::cout << val << " ";
    }

    std::cout << std::endl << buffer_test.size() << std::endl;


    veryslot2::circular_buffer<int> buffer_2(100);
    std::cout << std::endl << buffer_2.insert_back(m.begin(),
                                                   m.begin() + 150) << std::endl;

    for(int & val : buffer_2){
        std::cout << val << " ";
    }

    std::cout << std::endl << buffer_2.size() << std::endl;

    std::sort(buffer_2.rbegin(), buffer_2.rend());

    while(!buffer_2.empty()) {
        int val;
        buffer_2.pop_front(val);
        std::cout << val << " ";
    }

    return 0;
}
