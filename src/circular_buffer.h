//
// Created by vptyp on 17.02.24.
//

#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H
#include <vector>
#include "cb_iterator.h"
#define CONSTRUCTOR_HIDE_CIRCULARBUFFER
namespace veryslot2 {

template <typename T>
class circular_buffer {
public:
    typedef circular_buffer_iterator<circular_buffer, T> iterator;
    typedef circular_buffer_iterator<const circular_buffer, const T> const_iterator;
    typedef int func_result;
#ifdef CONSTRUCTOR_HIDE_CIRCULARBUFFER
    circular_buffer() = delete;
    circular_buffer(const circular_buffer&);
    circular_buffer(iterator first, iterator last) = delete;
    circular_buffer(const_iterator first, const_iterator last) = delete;
    circular_buffer(circular_buffer&&) noexcept;
    circular_buffer& operator=(const circular_buffer&);
    circular_buffer& operator=(circular_buffer&&) noexcept;
    //circular_buffer(const QVector<T>&);
    explicit circular_buffer(const size_t capacity) : m_capacity(capacity) {
        m_buffer = new T[capacity];
    }
    ~circular_buffer() {
        delete[] m_buffer;
    }
#endif
    /**
     * @brief implement the push-back operation to the circular buffer if the buffer is full,
     * the oldest element will be overwritten.
     * @details this operation is O(1) and will not throw any exception.
     */
    void push_back(const T& value) noexcept{
        m_buffer[m_tail] = value;
        m_tail = (m_tail + 1) % m_capacity;

        if (m_head == m_tail) {
            // buffer is full
            m_head = (m_head + 1) % m_capacity;
        }
    }

    /**
     * @brief implement the pop-front operation to the circular buffer.
     * @details this operation is O(1) and will not throw any exception.
     * @return  0 if done, -1 if buffer is empty.
     */
    func_result pop_front(T& value) noexcept{
        if (m_head == m_tail) {
            // buffer is empty
            return -1;
        }
        value = m_buffer[m_head];
        m_head = (m_head + 1) % m_capacity;
        return 0;
    }

    T& operator[](size_t index) {
        return m_buffer[(m_head + index) % m_capacity];
    }

    T* at(size_t index) {
        return &m_buffer[(m_head + index) % m_capacity];
    }

    /**
    * @brief tail - head gives the number of elements in the buffer and
        if the tail is less than the head then the buffer has wrapped around.
        In that case, we add the capacity to the tail to get the number of elements.
     * @return the number of elements in the buffer.
     */
    size_t size() const{
        return (m_tail + m_capacity - m_head) % m_capacity;
    }

    void clear() {
        m_head = m_tail = 0;
    }
    iterator begin() {
        return iterator(this, 0);
    }

    iterator end() {
        return iterator(this, size());
    }

    const_iterator cbegin() const {
        return const_iterator(this, 0);
    }

    const_iterator cend() const {
        return const_iterator(this, size());
    }

    void resize(size_t new_capacity) {
        T* new_buffer = new T[new_capacity];
        size_t new_size = std::min(new_capacity, m_capacity);
        int idx = new_capacity - size();
        idx = idx < 0 ? 0 - idx : 0;
        for (size_t i = 0; i < new_size; i++) {
            new_buffer[i] = (*this)[idx + i];
        }
        delete[] m_buffer;
        m_buffer = new_buffer;
        m_capacity = new_capacity;
        m_head = 0;
        m_tail = new_size;
    }
private:
    T* m_buffer = nullptr;
    size_t m_capacity = 0;
    size_t m_head = 0;
    size_t m_tail = 0;
};



}

#endif //CIRCULARBUFFER_H
