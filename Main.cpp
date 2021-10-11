#pragma once

#include <iterator>
#include <stdexcept>
#include <iostream>
#include <utility>
#include <initializer_list>
#include <algorithm>
#include <type_traits>

template<class T>
class MyVector {
private:
  size_t capacity_ = 0;
  T* start_ = nullptr;
  T* now_ = nullptr;
  T* end_ = nullptr;

  template <class U, class ...Args, typename =
    std::enable_if_t<!std::is_same<U, MyVector&>::value == true>>
    void MassPushBack_(U&& first, Args && ...args) {
    this->PushBack_(std::forward<T>(first));
    this->MassPushBack_(args...);
  };

  template <class U, typename =
    std::enable_if_t<!std::is_same<U, MyVector&>::value == true>>
    void MassPushBack_(U&& first) {
    this->PushBack_(std::forward<T>(first));
  };


  void PushBack_(T&& elem);

public:
  explicit MyVector();
  explicit MyVector(int num);

  MyVector(const MyVector<T>& vec) noexcept;
  MyVector(MyVector<T>&& vec) noexcept;

  template <class U, class ...Args, typename =
    std::enable_if_t<!std::is_same<U, MyVector&>::value == true>>
    MyVector(U&& first, Args&& ...args) {
    capacity_ = (sizeof(first)) / sizeof(T) + sizeof...(args);
    end_ = now_ = start_ = new T[capacity_];
    end_ += capacity_;
    this->MassPushBack_(first, args...);
  };

  ~MyVector();

  MyVector<T>& operator=(const MyVector<T>& vec);
  MyVector<T>& operator=(MyVector<T>&& vec) noexcept;

  T& operator[](const size_t index);
  const T& operator[](const size_t index) const;

  template<class U>
  void PushBack(U&& elem);
  void Swap(MyVector<T>& vec);
  void PopBack();
  void Clear();
  void Reserve(size_t new_cap);
  size_t Size() const noexcept;
  size_t Capacity() const noexcept;

  void print() noexcept;

  template<class T>
  friend void swap(MyVector<T>& vec, MyVector<T>& vecs) noexcept;
};

typedef MyVector<int> IntVector;

template<class T>
MyVector<T>::MyVector() {
  end_ = now_ = start_ = new T[1];
  capacity_ = 1;
  end_++;
}

template<class T>
MyVector<T>::MyVector(int num) {
  capacity_ = num;
  start_ = new T[capacity_];
  end_ = start_ + capacity_ - 1;
  now_ = start_ + num;
  for (size_t i = 0; i < num; ++i) {
    start_[i] = T();
  }
}

template<class T>
MyVector<T>::MyVector(const MyVector<T>& vec) noexcept :
  capacity_(vec.capacity_),
  start_(new T[capacity_]),
  now_(start_ + (vec.now_ - vec.start_)),
  end_(start_ + capacity_) {
  std::copy(vec.start_, vec.start_ + capacity_, start_);
}

template<class T>
MyVector<T>::MyVector(MyVector<T>&& vec) noexcept :
  capacity_(vec.capacity_),
  start_(vec.start_),
  now_(vec.now_),
  end_(vec.end_) {
  vec.start_ = vec.now_ = vec.end_ = nullptr;
  vec.capacity_ = 0;
}

template<class T>
MyVector<T>::~MyVector() {
  delete[] start_;
}

template<class T>
void MyVector<T>::Swap(MyVector<T>& vec) {
  swap(*this, vec);
}

template<class T>
void MyVector<T>::PopBack() {
  if (now_ != start_) now_--;
}

template<class T>
void MyVector<T>::Clear() {
  for (T* tmp = start_; tmp < now_; ++tmp) {
    *tmp = T();
  }
  now_ = start_;
}

template<class T>
void MyVector<T>::Reserve(size_t new_cap) {
  size_t old_cap = capacity_;
  if (old_cap > new_cap) {
    return;
  }
  capacity_ = new_cap;
  T* tmp = new T[capacity_];
  std::copy(start_, start_ + (old_cap), tmp);
  end_ = tmp + capacity_;
  now_ = tmp + (now_ - start_);
  delete[] start_;
  start_ = tmp;
}

template<class T>
size_t MyVector<T>::Size() const noexcept {
  return now_ - start_;
}

template<class T>
size_t MyVector<T>::Capacity() const noexcept {
  return capacity_;
}

template<class T>
void MyVector<T>::print() noexcept {
  for (T* tmp = start_; tmp < now_; ++tmp) {
    std::cout << *tmp << " ";
  }
  std::cout << "\n";
  std::cout << capacity_ << "\n";
}

template<class T>
MyVector<T>& MyVector<T>::operator=(const MyVector<T>& vec) {
  MyVector<T> tmp(vec);
  swap(*this, tmp);
  return *this;
}

template<class T>
MyVector<T>& MyVector<T>::operator=(MyVector<T>&& vec) noexcept {
  swap(*this, vec);
  return *this;
}

template<class T>
T& MyVector<T>::operator[](const size_t index) {
  return *(start_ + index);
}

template<class T>
const T& MyVector<T>::operator[](const size_t index) const {
  return *(start_ + index);
}

template<class T>
void MyVector<T>::PushBack_(T&& elem) {
  if (now_ == end_) {
    capacity_ *= 2;
    T* tmp = new T[capacity_];
    std::copy(start_, start_ + (capacity_ / 2), tmp);
    end_ = tmp + capacity_ - 1;
    now_ = tmp + (now_ - start_);
    delete[] start_;
    start_ = tmp;
  }
  *now_ = elem;
  now_++;
}

template<class T>
template<class U>
void MyVector<T>::PushBack(U&& elem) {
  this->PushBack_(std::forward<T>(elem));
}

template<class T>
void swap(MyVector<T>& vec, MyVector<T>& vecs) noexcept {
  std::swap(vec.capacity_, vecs.capacity_);
  std::swap(vec.start_, vecs.start_);
  std::swap(vec.now_, vecs.now_);
  std::swap(vec.end_, vecs.end_);
}


int main() {
  IntVector a { 1,2,3};
  std::cout << a.Size() << "\n";
  a.PopBack();
  std::cout << a.Size() << "\n";
  a.Clear();
  std::cout << a.Size();
  return 0;
}
