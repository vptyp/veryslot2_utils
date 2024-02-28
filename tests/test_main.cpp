#include <gtest/gtest.h>
#include <QVector>
#include <list>
#include "circular_buffer.h"

TEST(Constructor, DefaultConstructor) {
    veryslot2::circular_buffer<int> buffer(100);
    EXPECT_EQ(buffer.size(), 0);
    EXPECT_EQ(buffer.capacity(), 100);

    EXPECT_ANY_THROW(veryslot2::circular_buffer<int> buffer2(0));

}

TEST(Constructor, CopyConstructor) {
    veryslot2::circular_buffer<int> buffer(100);
    for (int i = 0; i < 100; i++) {
        buffer.push_back(i);
    }
    veryslot2::circular_buffer<int> buffer2(buffer);
    for (int i = 0; i < 100; i++) {
        EXPECT_EQ(buffer[i], buffer2[i]);
    }

}

TEST(Constructor, MoveConstructor) {
    veryslot2::circular_buffer<int> buffer(100);
    for (int i = 0; i < 100; i++) {
        buffer.push_back(i);
    }
    veryslot2::circular_buffer<int> buffer2(std::move(buffer));
    for (int i = 0; i < 100; i++) {
        EXPECT_EQ(buffer2[i], i);
    }
    EXPECT_EQ(buffer.size(), 0);
}

TEST(Operator, CopyAssignment) {
    veryslot2::circular_buffer<int> buffer(100);
    for (int i = 0; i < 100; i++) {
        buffer.push_back(i);
    }
    veryslot2::circular_buffer<int> buffer2(100);
    buffer2 = buffer;
    for (int i = 0; i < 100; i++) {
        EXPECT_EQ(buffer[i], buffer2[i]);
    }

    buffer2 = buffer2;
}

TEST(Operator, MoveAssignment) {
    veryslot2::circular_buffer<int> buffer(100);
    for (int i = 0; i < 100; i++) {
        buffer.push_back(i);
    }
    veryslot2::circular_buffer<int> buffer2(100);
    buffer2 = std::move(buffer);
    for (int i = 0; i < 100; i++) {
        EXPECT_EQ(buffer2[i], i);
    }
    EXPECT_EQ(buffer.size(), 0);

    buffer2 = std::move(buffer2);
    EXPECT_EQ(buffer2.size(), 100);
    for (int i = 0; i < 100; i++) {
        EXPECT_EQ(buffer2[i], i);
    }
}

TEST(Methods, PushBack) {
    veryslot2::circular_buffer<int> buffer(100);
    for (int i = 0; i < 100; i++) {
        buffer.push_back(i);
    }
    for (int i = 0; i < 100; i++) {
        EXPECT_EQ(buffer[i], i);
    }
}

TEST(Methods, PopFront) {
    veryslot2::circular_buffer<int> buffer(100);
    for (int i = 0; i < 100; i++) {
        buffer.push_back(i);
    }
    int val;
    for (int i = 0; i < 100; i++) {
        buffer.pop_front(val);
        EXPECT_EQ(val, i);
    }

    EXPECT_EQ(buffer.pop_front(val), -1);
    EXPECT_EQ(buffer.size(), 0);
}

TEST(Methods, EmplaceBack) {
    class A {
    public:
        A() : a(0) {}
        explicit A(int a) : a(a) {}
        int a;
    };

    veryslot2::circular_buffer<A> buffer(100);
    for (int i = 0; i < 100; i++) {
        buffer.emplace_back(i);
    }
    for (int i = 0; i < 100; i++) {
        EXPECT_EQ(buffer[i].a, i);
    }
}

TEST(Methods, InsertBack) {
    veryslot2::circular_buffer<int> buffer(100);
    std::vector<int> test(300);
    for (int i = 0; i < 300; i++) {
        test[i] = i;
    }
    EXPECT_EQ(buffer.insert_back(test.begin(), test.begin()), -1);
    auto skipped = buffer.insert_back(test.begin(), test.end());
    EXPECT_EQ(buffer.size(), 100);
    EXPECT_EQ(skipped, test.size() - buffer.size());
    for (int i = 0; i < buffer.size(); i++) {
        EXPECT_EQ(buffer[i],  i + skipped);
    }

    QVector<int> test2(300);
    for (int i = 0; i < 300; i++) {
        test2[i] = i;
    }
    skipped = buffer.insert_back(test2.begin(), test2.begin() + 200);
    EXPECT_EQ(buffer.size(), 100);
    EXPECT_EQ(skipped, 200 - buffer.size());
    for(int i = 0; i < buffer.size(); i++){
        EXPECT_EQ(buffer[i], i + skipped);
    }

    std::list<int> test3;
    for (int i = 0; i < 300; i++) {
        test3.push_back(i);
    }
    skipped = buffer.insert_back(test3.begin(), test3.end());
    EXPECT_EQ(buffer.size(), 100);
    EXPECT_EQ(skipped, test3.size() - buffer.size());
    for(int i = 0; i < buffer.size(); i++){
        EXPECT_EQ(buffer[i], i + skipped);
    }

    veryslot2::circular_buffer<int> buffer2(buffer);
    EXPECT_EQ(buffer2.size(), 100);
    EXPECT_EQ(buffer2.capacity(), 100);
    for(int i = 0; i < 100; i++){
        EXPECT_EQ(buffer2[i], buffer[i]);
    }
    skipped = buffer.insert_back(test3.begin(), ++test3.begin());
    EXPECT_EQ(buffer.size(), 100);
    EXPECT_EQ(skipped, 0);
    int notEqual = 0;
    for(int i = 0; i < buffer.size(); i++){
        if(buffer[i] != buffer2[i + 1]){
            notEqual++;
        }
    }
    EXPECT_EQ(notEqual, 1);

}

TEST(Methods, Safety){
    veryslot2::circular_buffer<int> buffer(100);
    std::vector<int> test(300);
    buffer.setSafe(true);
    for (int i = 0; i < 300; i++) {
        test[i] = i;
    }
    EXPECT_EQ(buffer.isSafe(), true);
    EXPECT_EQ(buffer.at(0), &buffer[0]);
    auto vec_it = test.begin();
    auto counter = 0;
    while(bool res = buffer.push_back(*vec_it++) == 0){
        ++counter;
    }
    EXPECT_EQ(counter, 100);
    for(int i = 0; i < 100; i++){
        EXPECT_EQ(buffer[i], i);
    }
    counter = 0;
    buffer.clear();
    while(bool res = buffer.push_back(1) == 0){
        ++counter;
    }
    EXPECT_EQ(counter, 100);
    for(int i = 0; i < 100; i++){
        EXPECT_EQ(buffer[i], 1);
    }

}

TEST(Methods, Replace) {
    veryslot2::circular_buffer<int> buffer(100);
    std::vector<int> test(300);
    for (int i = 0; i < 300; i++) {
        test[i] = i;
    }
    buffer.insert_back(test.begin(), test.end());
    std::vector<int> test2(300);
    for (int i = 0; i < 300; i++) {
        test2[i] = i + 300;
    }
    buffer.replace(test2.begin(), test2.end(), 50);
    for (int i = 0; i < buffer.size(); i++) {
        if (i < 50) {
            EXPECT_EQ(buffer[i], i + 550);
        } else {
            EXPECT_EQ(buffer[i], i + 450);
        }
    }

}

TEST(Methods, Empty) {
    veryslot2::circular_buffer<int> buffer(100);
    EXPECT_TRUE(buffer.empty());
    buffer.push_back(1);
    EXPECT_FALSE(buffer.empty());
    int a;
    buffer.pop_front(a);
    EXPECT_TRUE(buffer.empty());
    for(int i = 0; i < 100; i++){
        buffer.push_back(i);
    }
    EXPECT_FALSE(buffer.empty());
    veryslot2::circular_buffer<int> buffer2(std::move(buffer));
    EXPECT_TRUE(buffer.empty());
    for(int i = 0; i < 100; i++){
        buffer2.pop_front(a);
    }
    EXPECT_TRUE(buffer2.empty());
}

TEST(Methods, BeginEnd) {
    veryslot2::circular_buffer<int> buffer(100);
    for(int i = 0; i < 100; i++){
        buffer.push_back(i);
    }
    int i = 0;
    for(int &val : buffer){
        EXPECT_EQ(val, i++);
    }

    buffer.pop_front(i);
    buffer.pop_front(i);
    EXPECT_EQ(buffer.size(), 98);
    EXPECT_EQ(*(buffer.end() - 1), 99);
    EXPECT_EQ(*(buffer.begin()), 2);
    i = 2;

    for(int &val : buffer){
        EXPECT_EQ(val, i++);
    }

    veryslot2::circular_buffer<int> buffer2(100);
    for(i = 200; i < 400; i++){
        buffer2.push_back(i);
    }

    buffer.insert_back(buffer2.begin(), buffer2.begin() + 10);
    i = 0;
    for(int &val : buffer){
        if(i < 90){
            EXPECT_EQ(val, i + 10);
        } else {
            EXPECT_EQ(val, i + 300 - 90);
        }
        i++;
    }
    auto temp(buffer);

    buffer.resize(50);
    for(i = 0; i < 50; i++){
        EXPECT_EQ(buffer[i], temp[i + 50]);
    }

    buffer.resize(150);
    for(i = 0; i < 150; i++){
        if(i < 50){
            EXPECT_EQ(buffer[i], temp[i + 50]);
        } else {
            EXPECT_EQ(buffer[i], 0);
        }
    }

}

