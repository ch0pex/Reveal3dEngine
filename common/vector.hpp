/************************************************************************
* Copyright (c) 2024 Alvaro Cabrera Barrio
* This code is licensed under MIT license (see LICENSE.txt for details)
************************************************************************/
/**
 * @file vector.hpp
 * @version 1.0
 * @date 06/03/2024
 * @brief Vector class
 *
 * My own vector class with unordered_remove implementation
 *
 */


#pragma once

#include "primitive_types.hpp"

#include <cassert>
#include <memory>

namespace reveal3d::utl {

template<typename T, bool destruct = true>
class vector {
public:
    vector() = default;
    constexpr vector(u64 count);
    constexpr vector(u64 count, const T& value);
    constexpr vector(const vector& other);
    constexpr vector(vector&& other);
    ~vector();

    constexpr vector&  operator=(const vector& other);
    constexpr vector&  operator=(vector&& other);
    constexpr T&  operator[](u64 index);
    constexpr const T& operator[](u64 index) const;

    constexpr void emplace();
    constexpr void push_back(const T& value);
    constexpr void push_back(T&& value);
    template<typename... params>
    constexpr decltype(auto) emplace_back(params&&... value);
    constexpr T& at(u64 index);
    constexpr void swap();
    constexpr T* const remove(u64 index);
    constexpr T* const remove(T* const item);
    constexpr T* const  unordered_remove(u64 index);
    constexpr T* const unordered_remove(T* const item);
    constexpr void reserve(u64 new_capacity);
    constexpr void resize(u64 new_size, const T& value);
    constexpr T* data();
    constexpr T* begin();
    constexpr T* end();
    constexpr void shrink_to_fit();
    constexpr u64 capacity();
    constexpr u64 size();
    constexpr bool empty();
    constexpr void clear();
    constexpr T& back();
private:
    constexpr void reset();
    constexpr void clear_range(u64 first, u64 last);

    u64 size_ { 0 };
    u64 capacity_ { 0 };
    T*  data_ { nullptr };
};
template<typename T, bool destruct>
constexpr T &vector<T, destruct>::back() {
    assert(size_);
    return data_[size_ - 1];
}


template<typename T, bool destruct>
constexpr vector<T, destruct>::vector(u64 count) {
    reserve(count);
}

template<typename T, bool destruct>
constexpr vector<T, destruct>::vector(u64 count, const T &value) {
    reserve(count);
    memset(data_, value, count);
}

template<typename T, bool destruct>
constexpr vector<T, destruct>::vector(const vector &other) {
    *this = other;
}

template<typename T, bool destruct>
constexpr vector<T, destruct>::vector(vector &&other) :
    data_(other.data_),
    size_(other.size_),
    capacity_(other.capacity_)
{
    other.reset();
}

template<typename T, bool destruct>
vector<T, destruct>::~vector() {
    reset();
}

template<typename T, bool destruct>
constexpr vector<T, destruct>& vector<T, destruct>::operator=(const vector &other) {
    *this = other;
    return *this;
}

template<typename T, bool destruct>
constexpr vector<T, destruct>& vector<T, destruct>::operator=(vector &&other) {
    *this = other;
    other.reset();
    return *this;
}

template<typename T, bool destruct>
constexpr T& vector<T, destruct>::operator[](u64 index) {
    return data_[index];
}

template<typename T, bool destruct>
constexpr const T &vector<T, destruct>::operator[](u64 index) const {
    assert(data_ && index < size_);
    return data_[index];
}

template<typename T, bool destruct>
constexpr void vector<T, destruct>::push_back(const T &value) {
    emplace_back(value);
}

template<typename T, bool destruct>
constexpr void vector<T, destruct>::push_back(T &&value) {
    emplace_back(std::move(value));
}

template<typename T, bool destruct>
constexpr void vector<T, destruct>::emplace() {

}

template<typename T, bool destruct>
template<typename... params>
constexpr decltype(auto) vector<T, destruct>::emplace_back(params &&...value) {
    if (size_ == capacity_) {
        reserve(size_ + (size_ >> 1) + 1);
    }
    assert(data_);
    T *const item { new (std::addressof(data_[size_])) T(std::forward<params>(value)...) };
    ++size_;
    return *item;

}

template<typename T, bool destruct>
constexpr T &vector<T, destruct>::at(u64 index) {
    assert(index < size_);
    return data_[index];
}

template<typename T, bool destruct>
constexpr void vector<T, destruct>::swap() {

}

template<typename T, bool destruct>
constexpr T *const vector<T, destruct>::remove(u64 index) {
    assert(data_ && index < size_);
    return remove(std::addressof(data_[index]));
}

template<typename T, bool destruct>
constexpr T * const vector<T, destruct>::remove(T *const item) {
    assert(data_ && item >= std::addressof(data_[0]) &&
           item < std::addressof(data_[size_]));
   if constexpr (destruct) {
       item->~T();
   }
   if (item < std::addressof(data_[--size_])) {
        memcpy(item, item  + 1, (std::addressof(data_[size_]) - item) * sizeof(T));
   }
   return item;
}

template<typename T, bool destruct>
constexpr T* const vector<T, destruct>::unordered_remove(u64 index) {
    assert(data_ && index < size_);
    return unordered_remove(std::addressof(data_[index]));
}

template<typename T, bool destruct>
constexpr T* const vector<T, destruct>::unordered_remove(T * const item) {
    assert(data_ && item >= std::addressof(data_[0]) &&
           item < std::addressof(data_[size_]));
    if constexpr (destruct) {
        item->~T();
    }
    if (item < std::addressof(data_[--size_])) {
        memcpy(item, std::addressof(data_[size_]), sizeof(T));
    }
    return item;
}

template<typename T, bool destruct>
constexpr void vector<T, destruct>::reserve(u64 new_capacity) {
   if (new_capacity > capacity_)  {
      void* new_buff = realloc(data_, new_capacity * sizeof(T));
      if (new_buff != nullptr) {
          capacity_ = new_capacity;
          data_ = static_cast<T*>(new_buff);
      }
   }
}

template<typename T, bool destruct>
constexpr void vector<T, destruct>::resize(u64 new_size, const T &value) {
    if (new_size > size_) {
        reserve(new_size);
        while(size_ < new_size) {
            emplace_back(value);
        }
    } else if (new_size < size_) {
        if constexpr (destruct) {
            clear_range(new_size, size_);
        }
        size_ = new_size;
    }
}

template<typename T, bool destruct>
constexpr T* vector<T, destruct>::data() {
    return data_;
}

template<typename T, bool destruct>
constexpr T* vector<T, destruct>::begin() {
    return data_;
}

template<typename T, bool destruct>
constexpr T* vector<T, destruct>::end() {
    return &data_[size_];
}

template<typename T, bool destruct>
constexpr void vector<T, destruct>::shrink_to_fit() {
    void* new_buff = realloc(data_, size_ * sizeof(T));
    if (new_buff != nullptr) {
        capacity_ = size_;
        data_ = static_cast<T*>(new_buff);
    }
}

template<typename T, bool destruct>
constexpr u64 vector<T, destruct>::capacity() {
    return capacity_;
}

template<typename T, bool destruct>
constexpr u64 vector<T, destruct>::size() {
    return size_;
}

template<typename T, bool destruct>
constexpr bool vector<T, destruct>::empty() {
    return size_ == 0;
}

template<typename T, bool destruct>
constexpr void vector<T, destruct>::clear() {
    clear_range(0, size_);
    size_ = 0;
}

template<typename T, bool destruct>
constexpr void vector<T, destruct>::clear_range(u64 first, u64 last) {
    if (data_ != nullptr) {
        for(u64 i = first; i < last; ++i) {
            data_[i].~T();
        }
    }
}

template<typename T, bool destruct>
constexpr void vector<T, destruct>::reset() {
    clear();
    free(data_);
    capacity_ = 0;
    size_ = 0;
}

}
