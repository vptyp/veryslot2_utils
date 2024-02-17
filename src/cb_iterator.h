//
// Created by vptyp on 17.02.24.
//

#ifndef CIRCULARBUFFERITERATOR_H
#define CIRCULARBUFFERITERATOR_H
#include <iterator>
namespace veryslot2 {

/**
 * @brief
 * @tparam ValueType type of the value in the circular buffer.
 * @tparam BufferType type of the circular buffer.
 */
template <class BufferType, typename ValueType>
class circular_buffer_iterator : public std::iterator<std::random_access_iterator_tag, ValueType> {
    friend BufferType;
private:
    circular_buffer_iterator(BufferType* ptr, const size_t position)
        : m_ptr(ptr), m_position(position) {}
public:
    circular_buffer_iterator(const circular_buffer_iterator& other)
        : m_ptr(other.m_ptr), m_position(other.m_position) {}
    circular_buffer_iterator& operator=(const circular_buffer_iterator& other) {
        if(this == &other)
            return *this;

        m_ptr = other.m_ptr;
        m_position = other.m_position;
        return *this;
    }
    bool operator==(const circular_buffer_iterator& other) const {
        return m_ptr == other.m_ptr && m_position == other.m_position;
    }
    bool operator!=(const circular_buffer_iterator& other) const {
        return !(*this == other);
    }
    bool operator<(const circular_buffer_iterator& other) const {
        return m_position < other.m_position;
    }
    bool operator>(const circular_buffer_iterator& other) const {
        return other < *this;
    }
    bool operator<=(const circular_buffer_iterator& other) const {
        return !(other < *this);
    }
    bool operator>=(const circular_buffer_iterator& other) const {
        return !(*this < other);
    }
    circular_buffer_iterator& operator++() {
        m_position++;
        return *this;
    }
    circular_buffer_iterator operator++(int) {
        circular_buffer_iterator tmp(*this);
        operator++();
        return tmp;
    }
    circular_buffer_iterator& operator--() {
        m_position--;
        return *this;
    }
    circular_buffer_iterator operator--(int) {
        circular_buffer_iterator tmp(*this);
        operator--();
        return tmp;
    }
    circular_buffer_iterator& operator+=(const size_t offset) {
        m_position += offset;
        return *this;
    }
    circular_buffer_iterator& operator-=(const size_t offset) {
        m_position -= offset;
        return *this;
    }
    circular_buffer_iterator operator+(const size_t offset) const {
        circular_buffer_iterator tmp(*this);
        return tmp += offset;
    }
    circular_buffer_iterator operator-(const size_t offset) const {
        circular_buffer_iterator tmp(*this);
        return tmp -= offset;
    }
    size_t operator-(const circular_buffer_iterator& other) const {
        return m_position - other.m_position;
    }
    ValueType& operator*() const {
        return (*m_ptr)[m_position];
    }
    ValueType* operator->() const {
        return &operator*();
    }
    ValueType& operator[](const size_t offset) const {
        return (*m_ptr)[m_position + offset];
    }

private:
    BufferType* m_ptr;
    size_t m_position;
};
}

#endif //CIRCULARBUFFERITERATOR_H
