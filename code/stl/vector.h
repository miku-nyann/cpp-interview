#pragma once 

#include <cstddef>
#include <memory>

// 1. T is not array
// 2. impl reserve(), resize(), push_back(), pop_back()

template <typename T>
class vector {
public:
    vector() : _data(nullptr), _size(0), _capacity(0) {}
    explicit vector(size_t capacity) {}
    vector(size_t capacity, const T& val) {}

    vector(const vector& that) {}
    vector& operator=(const vector& that) {}
    
    vector(vector&& that) {}
    vector& operator=(vector&& that) {}

    ~vector() {
        clear();
        ::operator delete(_data);
        _capacity = 0;
    }

    void clear() {
        for (size_t i = 0; i < _size; ++i)
            _data[i].~T();
        _size = 0;
    }

    void resize(size_t newSize, const T& val) {
        if (newSize > _size) {
            reserve(newSize);
            for (size_t i = _size; i < newSize; ++i)
                new (&_data[i]) T(val);
        } else if (newSize < _size) {
            for (size_t i = newSize; i < _size; ++i)
                _data[i].~T();
        }
        _size = newSize;
    }

    void reserve(size_t newCapacity) {
        if (newCapacity <= _capacity)
            return;
        _capacity = std::max(newCapacity, _capacity * 2);
        T* oldData = _data;
        _data = static_cast<T*>(::operator new(_capacity * sizeof(T)));
        if (oldData) {
            for (size_t i = 0; i < _size; ++i)
                new (&_data[i]) T(std::move(oldData[i]));
            for (size_t i = 0; i < _size; ++i)
                oldData[i].~T();
            ::operator delete(oldData);
        }
    }

    void push_back(const T& val) {
        emplace_back(val);
    }

    void push_back(T&& val) {
        emplace_back(std::move(val));
    }

    template <class... Args>
    T& emplace_back(Args... args) {
        if (_size >= _capacity) {
            reserve(_size + 1);
        }
        T* t = new (&_data[_size]) T(std::forward<Args>(args)...); 
        ++_size;
        return *t;
    }
    
    T& front() { return *_data; }
    const T& front() const { return *_data; }

    T& back() { return _data[_size - 1]; }
    const T& back() const { return _data[_size - 1]; }

    void pop_back() {
        _data[_size].~T();
        --_size;
    }

private:
    T*      _data;
    size_t  _size;
    size_t  _capacity;
};
