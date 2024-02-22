//
// Created by vptyp on 17.02.24.
//

#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H
#include <vector>
#include "cb_iterator.h"
#include <QVector>
#include <iostream>

namespace veryslot2 {
    /**
     * @brief A circular buffer is a data structure that uses a single, fixed-size buffer as if it were connected end-to-end.
     * This implementation based on the circular buffer from the boost library. And some other implementations from the internet.
     * @details This implementation using iterators, and can be used with the standard algorithms.
     * @details Is not thread-safe. If you want to use it in a multi-threaded environment, you should use a mutex.
     * @details Can work only with default-constructible elements. Version 2.0 will support non-default-constructible elements.
     * @details Can be overwrite safe or not. If it is safe, the push_back operation will return -1 if the buffer is full.
     * Currently only for push_back. Version 2.0 will support insert_back.
     * @tparam T is the type of the elements in the buffer. Must be default-constructible.
     */
template <typename T>
class circular_buffer{
public:
    typedef circular_buffer_iterator<circular_buffer, T> iterator;
    typedef circular_buffer_iterator<const circular_buffer, const T> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef int func_result;
    friend iterator;
    friend const_iterator;
    friend reverse_iterator;
    circular_buffer() = delete;
    circular_buffer(iterator first, iterator last) = delete;
    circular_buffer(const_iterator first, const_iterator last) = delete;
    circular_buffer(circular_buffer&&) noexcept = default;
    circular_buffer& operator=(circular_buffer&&) noexcept = default;
    circular_buffer(const circular_buffer& other)
    : m_capacity(other.m_capacity), m_head(other.m_head), m_tail(other.m_tail)
    {
        m_buffer = new T[m_capacity];
        std::copy(other.begin(), other.end(), begin());
    }
    circular_buffer& operator=(const circular_buffer& other) {
        if(this == &other) return *this;
        m_capacity = other.m_capacity;
        m_buffer = new T[m_capacity];
        m_head = other.m_head;
        m_tail = other.m_tail;
        std::copy(other.begin(), other.end(), begin());
        return *this;
    }
    //circular_buffer(const QVector<T>&);
    explicit circular_buffer(const size_t capacity) : m_capacity(capacity) {
        m_buffer = new T[capacity]{0};
    }

    ~circular_buffer() {
        delete[] m_buffer;
    }
    /**
     * @brief implement the push-back operation to the circular buffer if the buffer is full,
     * the oldest element will be overwritten.
     * @details this operation is O(1) and will not throw any exception.
     * @return 0 if done, -1 if buffer is full and safe mode is on.
     */
    func_result push_back(const T& value) noexcept{
        if(safe && isFull) return -1;
        m_buffer[m_tail] = value;
        // buffer is full
        m_head = (m_head + isFull) % m_capacity;
        m_tail = (m_tail + 1) % m_capacity;
        isFull = m_tail == m_head;
        return 0;
    }

    func_result push_back(T&& value) noexcept{
        if(safe && isFull) return -1;
        m_buffer[m_tail] = std::move(value);
        // buffer is full
        m_head = (m_head + isFull) % m_capacity;
        m_tail = (m_tail + 1) % m_capacity;
        isFull = m_tail == m_head;
        return 0;
    }

    template <typename InputIterator>
    func_result insert_back(const InputIterator begin, const InputIterator end) noexcept {
        for(auto it = begin; it != end; ++it) {
            if(push_back(*it)) return std::distance(begin, it);
        }
        return 0;
    }

    func_result insert_back(const typename QVector<T>::iterator begin,
                                    const typename QVector<T>::iterator end) noexcept {
        return private_insert_back(begin, end);
    }

    func_result insert_back(const typename std::vector<T>::iterator begin,
                                const typename std::vector<T>::iterator end) noexcept {
        return private_insert_back(begin, end);
    }

    template<typename InputIterator>
    void replace(InputIterator begin, InputIterator end, size_t position)
    {
        for(auto& it = begin; it != end; ++it) {
            (*this)[position + it - begin] = *it;
        }
    }

    [[nodiscard]] bool empty() const {
        return m_head == m_tail && !isFull;
    }

    /**
     * @brief implement the pop-front operation to the circular buffer.
     * @details this operation is O(1) and will not throw any exception.
     * @return  0 if done, -1 if buffer is empty.
     */
    func_result pop_front(T& value) noexcept{
        if (m_head == m_tail && !isFull){
            // buffer is empty
            return -1;
        }
        value = m_buffer[m_head];
        m_head = (m_head + 1) % m_capacity;
        isFull = false;
        return 0;
    }

    T& operator[](size_t index) const{
        return m_buffer[(m_head + index) % size()];
    }

    T* at(size_t index) const{
        return &m_buffer[(m_head + index) % size()];
    }

    /**
     * @brief basically the same as push_back, because nodes are already created.
     * Will be implemented in version 2.0
     * @tparam Args
     * @param args
     */
    template<class... Args>
    void emplace_back(Args... args) {
        push_back(T(args...));
    }

    bool isSafe() const {
        return safe;
    }

    void setSafe(bool safe) {
        this->safe = safe;
    }

    /**
    * @brief tail - head gives the number of elements in the buffer and
        if the tail is less than the head then the buffer has wrapped around.
        In that case, we add the capacity to the tail to get the number of elements.
     * @return the number of elements in the buffer.
     */
    [[nodiscard]] size_t size() const{
        if(isFull) return m_capacity;
        if(m_tail >= m_head)
            return m_tail - m_head;
        return m_capacity - m_head + m_tail;
    }

    void clear() {
        m_head = m_tail = 0;
        isFull = false;
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

    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }

    reverse_iterator rend() {
        return reverse_iterator(begin());
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
    template <typename RandomIterator>
    func_result private_insert_back(const RandomIterator begin, const RandomIterator end) noexcept {
        if (begin == end) return -1;
        T* raw_ptr;
        std::cout << "distance: " << std::distance(begin,end) << std::endl;
        if (std::distance(begin, end) >= m_capacity) {
            auto moved_begin = begin + (std::distance(begin, end) - m_capacity);
            std::copy(moved_begin, end, m_buffer);
            m_tail = m_head = 0;
            isFull = true;
            return end - m_capacity - begin + 1;
        }
        size_t len = std::distance(begin, end);
        size_t from_tail = m_capacity - m_tail;
        if (len > from_tail) {
            std::copy(begin, begin + from_tail, m_buffer + m_tail);
            std::copy(begin + from_tail, end, m_buffer);
            tail_to_head(len - from_tail, len);
            m_tail = len - from_tail;
        } else {
            std::copy(begin, end, m_buffer + m_tail);
            tail_to_head(m_tail + len, len);
            m_tail += len;
        }
        isFull = m_head == m_tail;
        return 0;
    }

    void tail_to_head(size_t to, size_t move) {
        if(isFull){
            m_head = to;
            return;
        }

        if(m_tail < m_head || (!m_tail && !m_head)){
            m_head = m_tail + move > m_head ? to : m_head;
            return;
        }

        m_head = to > m_head ? to : m_head;
    }
private:
    T* m_buffer = nullptr;
    size_t m_capacity = 0;
    size_t m_head = 0;
    size_t m_tail = 0;
    bool safe = false;
    bool isFull = false;
};



}

#endif //CIRCULARBUFFER_H
