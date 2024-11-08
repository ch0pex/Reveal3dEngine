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

  constexpr vector(u64 count) { reserve(count); }

  constexpr vector(u64 count, const T &value) { reserve(count); memset(data_, value, count); }

  constexpr vector(const vector &other) { *this = other; }

  constexpr vector(vector &&other) noexcept :
      data_(other.data_),
      size_(other.size_),
      capacity_(other.capacity_)
  {
    other.reset();
  }

  ~vector() { reset(); }

  constexpr vector<T, destruct>& operator=(const vector &other) { *this = other; return *this; }

  constexpr vector<T, destruct>& operator=(vector &&other) {
    *this = other;
    other.reset();
    return *this;
  }

  constexpr T& operator[](u64 index) { return data_[index]; }

  constexpr const T &operator[](u64 index) const { assert(data_ && index < size_); return data_[index]; }

  constexpr void push_back(const T &value) { emplace_back(value); }

  constexpr void push_back(T &&value) { emplace_back(std::move(value)); }

  constexpr void emplace() { }

  constexpr T &at(u64 index) { assert(index < size_); return data_[index]; }

  constexpr void swap() { }

  constexpr T* data() { return data_; }

  constexpr T* begin() { return data_; }

  constexpr T* end() { return &data_[size_]; }

  constexpr u64 capacity() { return capacity_; }

  constexpr u64 size() { return size_; }

  constexpr bool empty() { return size_ == 0; }

  constexpr void clear() { clear_range(0, size_); size_ = 0; }

  constexpr T &back() { assert(size_); return data_[size_ - 1]; }

  template<typename... params>
  constexpr decltype(auto) emplace_back(params &&...value) {
    if (size_ == capacity_) {
      reserve(size_ + (size_ >> 1) + 1);
    }
    assert(data_);
    T *const item { new (std::addressof(data_[size_])) T(std::forward<params>(value)...) };
    ++size_;
    return *item;

  }

  constexpr T *const remove(u64 index) {
    assert(data_ && index < size_);
    return remove(std::addressof(data_[index]));
  }

  constexpr T * const remove(T *const item) {
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


  constexpr T* const unordered_remove(T * const item) {
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

  constexpr T* const unordered_remove(u64 index) {
    assert(data_ && index < size_);
    return unordered_remove(std::addressof(data_[index]));
  }

  constexpr void reserve(u64 new_capacity) {
    if (new_capacity > capacity_)  {
      void* new_buff = realloc(data_, new_capacity * sizeof(T));
      if (new_buff != nullptr) {
        capacity_ = new_capacity;
        data_ = static_cast<T*>(new_buff);
      }
    }
  }

  constexpr void resize(u64 new_size, const T &value) {
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

  constexpr void shrink_to_fit() {
    void* new_buff = realloc(data_, size_ * sizeof(T));
    if (new_buff != nullptr) {
      capacity_ = size_;
      data_ = static_cast<T*>(new_buff);
    }
  }

private:
  constexpr void clear_range(u64 first, u64 last) {
    if (data_ != nullptr) {
      for(u64 i = first; i < last; ++i) {
        data_[i].~T();
      }
    }
  }

  constexpr void reset() {
    clear();
    free(data_);
    capacity_ = 0;
    size_ = 0;
  }

    u64 size_ { 0 };
    u64 capacity_ { 0 };
    T*  data_ { nullptr };
};

}