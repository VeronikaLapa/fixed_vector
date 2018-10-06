//
// Created by Nika on 06.10.2018.
//

#ifndef FIXED_VECTOR_H
#define FIXED_VECTOR_H

#include <type_traits>
#include <iostream>
#include <cassert>
#include <iterator>
#include <algorithm>

template<typename T, std::size_t N>
class fixed_vector{

public:
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
public:
    fixed_vector(): _size(0){};
    fixed_vector(fixed_vector<T, N> const& other): fixed_vector(){
        for (size_t i = 0; i < other._size; i++) {
            push_back(other[i]);
        }
    };
    template<typename O, size_t osize>
    fixed_vector(fixed_vector<O, osize> const& other): fixed_vector(){
        for (size_t i = 0; i < std::min(N, other.size()); i++) {
            push_back(other[i]);
        }
    };
    void Swap( fixed_vector<T,N>& other) {
        std::swap(data, other.data);
        std::swap(_size, other._size);
    }

    fixed_vector operator=(const fixed_vector<T,N>& other){
        fixed_vector<T,N> temp(other);
        Swap(temp);
        return *this;
    };

    template<typename O, size_t osize>
    fixed_vector operator=(const fixed_vector<O,osize>& other){
        fixed_vector<T,N> temp(other);
        Swap(temp);
        return *this;
    };
    ~fixed_vector() {
        for(std::size_t pos = 0; pos < _size; ++pos) {
            reinterpret_cast<T*>(data+pos)->~T();
        }
    }
    T& operator[](size_t index){
        assert(index < _size);
        return *reinterpret_cast<T*>(data + index);
    };
    T const& operator[] (size_t index) const {
        assert(index < _size);
        return *reinterpret_cast<const T*>(data + index);
    };
    size_t size() const {
        return  _size;
    }
    size_t capacity() const {
        return N;
    }

    bool empty() const {
        return _size == 0;
    }
    void push_back(T const& val){
        assert(_size <= N);
        new(&data[_size]) T(val);
        ++_size;
    };
    void pop_back(){
        assert(!empty());
        reinterpret_cast<T *> (&data[_size - 1])->~T();
        --_size;
    };

    T& back(){
        return *reinterpret_cast<T*>(data +_size - 1);
    };
    T const& back() const{
        return *reinterpret_cast<const T*> (data + _size - 1);
    };

    T& front() {
        return *reinterpret_cast<T*>(data);
    };
    T const& front() const {
        return *reinterpret_cast<const T*> (data);
    };

    void clear() {
        for(std::size_t pos = 0; pos < _size; ++pos) {
            reinterpret_cast< T*>(data+pos)->~T();
        }
        _size = 0;
    }
    iterator erase(const_iterator pos){
        auto i = static_cast<size_t>(std::distance(cbegin(), pos));
        while (i < _size - 1) {
            operator[](i) = operator[](i+1);
            ++i;
        }
        pop_back();
        return iterator(pos);
    };

    iterator erase(const_iterator beg, const_iterator end){
        auto i1 = static_cast<size_t>(std::distance(cbegin(), beg));
        auto i2 = static_cast<size_t>(std::distance(cbegin(), end));
        while(i2 < _size) {
            operator[](i1) = operator[](i2);
            ++i1;
            ++i2;
        }
        for (size_t i = 0; i < i2 - i1; i++) {
            pop_back();
        }
        return iterator(beg);
    };
    iterator insert(const_iterator pos, T const& el){
        auto i = static_cast<size_t>(std::distance(cbegin(), pos));
        push_back(el);
        if (_size > 0) {
            for (size_t j = _size - 1; j > i; --j) {
                operator[](j) = operator[](j - 1);
            }
            operator[](i) = el;
            return iterator(pos);
        }
    };
private:
    typename std::aligned_storage<sizeof(T), alignof(T)>::type data[N];
    size_t _size;
public:
    iterator begin() {
        return iterator(reinterpret_cast<T*>(data));
    }
    const_iterator begin() const{
        return const_iterator(reinterpret_cast<const T*>(data));
    };
    const_iterator cbegin() {
        return begin();
    }
    iterator end() {
        return begin() + _size;
    }
    const_iterator end() const{
        return begin() + _size;
    };
    const_iterator cend() const{
        return end();
    };
    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }

    reverse_iterator rend() {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    }

    const_reverse_iterator crbegin() const {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator crend() const {
        return const_reverse_iterator(begin());
    }

};
#endif //FIXED_VECTOR_H
