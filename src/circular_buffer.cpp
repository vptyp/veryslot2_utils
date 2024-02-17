//
// Created by vptyp on 17.02.24.
//

#include "circular_buffer.h"
#include <string.h>
namespace veryslot2 {
template<typename T>
circular_buffer<T>::circular_buffer(const circular_buffer & other) {
    m_capacity = other.m_capacity;
    m_buffer = new T[m_capacity];
    m_head = other.m_head;
    m_tail = other.m_tail;
    memcpy(m_buffer, other.m_buffer, m_capacity * sizeof(T));
}

template<typename T>
circular_buffer<T>::circular_buffer(circular_buffer && other) noexcept {
    m_capacity = other.m_capacity;
    m_buffer = other.m_buffer;
    m_head = other.m_head;
    m_tail = other.m_tail;
    other.m_capacity = 0;
    other.m_buffer = nullptr;
    other.m_head = 0;
    other.m_tail = 0;
}

template<typename T>
circular_buffer<T> & circular_buffer<T>::operator=(const circular_buffer & other) {
    m_capacity = other.m_capacity;
    m_buffer = new T[m_capacity];
    m_head = other.m_head;
    m_tail = other.m_tail;
    memcpy(m_buffer, other.m_buffer, m_capacity * sizeof(T));
    return *this;
}

template<typename T>
circular_buffer<T> & circular_buffer<T>::operator=(circular_buffer && other) noexcept {
    m_capacity = other.m_capacity;
    m_buffer = other.m_buffer;
    m_head = other.m_head;
    m_tail = other.m_tail;
    other.m_capacity = 0;
    other.m_buffer = nullptr;
    other.m_head = 0;
    other.m_tail = 0;
    return *this;
}
}