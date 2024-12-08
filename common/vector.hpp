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
#include <ranges>

namespace reveal3d::utl {

template<typename T, bool destruct = true>
class vector {
public:
  using value_type      = T;
  using pointer         = T*;
  using const_pointer   = T const*;
  using reference       = T&;
  using const_reference = T const&;
  using iterator        = typename std::span<T>::iterator;
  using const_iterator  = typename std::span<T>::const_iterator;

  vector() = default;

  explicit constexpr vector(u64 const count) { reserve(count); }

  constexpr vector(u64 count, T const& value) {
    reserve(count);
    std::ranges::copy(data, data + count, value);
  }

  constexpr vector(vector const& other) { *this = other; }

  constexpr vector(vector&& other) noexcept : size_(other.size_), data_(other.data_) { other.reset(); }

  ~vector() { reset(); }

  constexpr vector& operator=(vector const& other) {
    data_ = other.data_;
    size_ = other.size_;
    return *this;
  }

  constexpr vector& operator=(vector&& other) noexcept {
    *this       = other;
    other.data_ = {};
    other.size_ = 0;

    return *this;
  }

  constexpr reference operator[](u64 index) { return data_[index]; }

  constexpr const_reference operator[](u64 index) const { return data_[index]; }

  constexpr void push_back(T const& value) { emplace_back(value); }

  constexpr void push_back(T&& value) { emplace_back(std::move(value)); }

  constexpr void emplace() { }

  constexpr reference at(u64 index) { return data_[index]; }

  constexpr void swap() { }

  constexpr pointer data() { return data_; }

  constexpr iterator begin() { return data_.begin(); }

  constexpr iterator end() { return data_.begin() + size_; }

  constexpr u64 capacity() { return data_.size(); }

  [[nodiscard]] constexpr u64 size() const { return size_; }

  [[nodiscard]] constexpr bool empty() const { return size_ == 0; }

  constexpr void clear() {
    clear_range(0, size_);
    size_ = 0;
  }

  constexpr T& back() { return data_[size_ - 1]; }

  template<typename... params>
  constexpr decltype(auto) emplace_back(params&&... value) {
    if (size_ == data_.size()) {
      reserve(size_ + (size_ >> 1) + 1);
    }
    T* const item {new (std::addressof(data_[size_])) T(std::forward<params>(value)...)};
    ++size_;
    return *item;
  }

  constexpr pointer remove(u64 index) { return remove(std::addressof(data_[index])); }

  constexpr pointer remove(pointer const item) {
    if constexpr (destruct) {
      item->~T();
    }
    if (item < std::addressof(data_[--size_])) {
      memcpy(item, item + 1, (std::addressof(data_[size_]) - item) * sizeof(T));
    }
    return item;
  }


  constexpr pointer unordered_remove(pointer const item) {
    if constexpr (destruct) {
      item->~T();
    }
    if (item < std::addressof(data_[--size_])) {
      std::memcpy(item, std::addressof(data_[size_]), sizeof(T));
    }
    return item;
  }

  constexpr pointer unordered_remove(u64 index) { return unordered_remove(std::addressof(data_[index])); }

  constexpr void reserve(u64 new_capacity) {
    if (new_capacity > data_.size()) {
      if (void* new_buff = std::realloc(data_.data(), new_capacity * sizeof(T)); new_buff != nullptr) {
        data_ = std::span(static_cast<T*>(new_buff), static_cast<T*>(new_buff) + new_capacity);
      }
    }
  }

  constexpr void resize(u64 const new_size, T const& value) {
    if (new_size > size_) {
      reserve(new_size);
      while (size_ < new_size) {
        emplace_back(value);
      }
    }
    else if (new_size < size_) {
      if constexpr (destruct) {
        clear_range(new_size, size_);
      }
      size_ = new_size;
    }
  }

  constexpr void shrink_to_fit() {
    if (void* new_buff = std::realloc(data_.data(), size_ * sizeof(T)); new_buff != nullptr) {
      data_ = std::span(static_cast<T*>(new_buff), size_);
    }
  }

private:
  constexpr void clear_range(u64 const first, u64 const last) {
    if (data_.data() != nullptr) {
      for (u64 i = first; i < last; ++i) {
        data_[i].~T();
      }
    }
  }

  constexpr void reset() {
    clear();
    std::free(data_.data());
    size_ = 0;
  }

  u64 size_ {0};
  std::span<T> data_ {};
};

} // namespace reveal3d::utl
