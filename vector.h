#pragma once
#include "libraries.h"
namespace DataStructures {
    template<class T>class vector {
        size_t _size;
        size_t _capacity;
        T* _Array;

        void _delete_array_and_elements()
        {
            for (int i = 0; i < _size; i++)
                _Array[i].~T();
            ::operator delete (_Array);
        }
    public:
        T* begin() { return _Array; }
        const T* begin() const{ return _Array; }

        T* end()  { return _Array + _size; }
       const T* end()const { return _Array + _size; }

        const T& operator[](size_t i) const {
            assert(i < _size && "Index out of range");
            return _Array[i];
        }

        T& operator[](size_t i) {
            assert(i < _size && "Index out of range");
            return _Array[i];
        }

        T& at(size_t i) {
            if (i >= _size) {
                throw std::out_of_range("vector::at: index out of range");
            }
            return _Array[i];
        }

        const T& at(size_t i) const {
            if (i >= _size) {
                throw std::out_of_range("vector::at: index out of range");
            }
            return _Array[i];
        }

        vector() : _size(0), _capacity(0), _Array(nullptr){}

        vector(size_t initialSize)
            :_size((initialSize > 0) ? initialSize : 0),
            _capacity((initialSize > 0) ? initialSize : 1),
            _Array((T*)::operator new(_capacity * sizeof(T))){
            for (size_t i = 0; i < _size; i++) new (&_Array[i]) T();
        }

        vector(std::initializer_list<T> init_list)
            : _size(init_list.size()),
            _capacity(_size),
            _Array((T*)::operator new(_capacity * sizeof(T)))
        {std::uninitialized_copy(init_list.begin(), init_list.end(), _Array);}

        vector(const vector<T>& other)
            :_size(other._size),
            _capacity(other._capacity),
            _Array((T*)::operator new(_capacity * sizeof(T)))
        {
            std::uninitialized_copy(other.begin(), other.end(), _Array);
        }

        vector(vector&& other) noexcept
            : _size(other._size),
            _capacity(other._capacity),
            _Array(other._Array)
        
        {
            other._Array = nullptr;
            other._size = 0;
            other._capacity = 0;
        }

        vector<T>& operator=(vector<T> other) noexcept {
            std::swap(_size, other._size);
            std::swap(_capacity, other._capacity);
            std::swap(_Array, other._Array);
            return *this;
        }

        ~vector() { _delete_array_and_elements(); }

        void reserve(size_t newCapacity) {
            if (newCapacity <= _capacity) return;

            T* temp = ((T*)::operator new(newCapacity * sizeof(T)));
            
            try {
                std::uninitialized_move(begin(), end(), temp);
            }
            catch (...){
                ::operator delete (temp);
                throw;
            }

            _delete_array_and_elements();
            _Array = temp;
            _capacity = newCapacity;
        }

        void shrink_to_fit() {
            if (_size == _capacity)return;

            if (_size == 0) {
                _delete_array_and_elements();
                _capacity = 0;
                _Array = nullptr;
                return;
            }

            T* temp = ((T*)::operator new(_size * sizeof(T)));
            try {std::uninitialized_move(begin(), end(), temp);}
            catch (...) { ::operator delete(temp); throw; }
            
            _delete_array_and_elements();
            _Array = temp;
            _capacity = _size;
        }

        void push_back(const T& element) {
            if (_size >= _capacity)
                reserve((_capacity == 0) ? 1 : (_capacity * 2));
            
            new (&_Array[_size]) T(element);
            _size++;
        }

        template<typename... Args>void emplace_back(Args&&... args) {
            if (_size >= _capacity)
                reserve((_capacity == 0) ? 1 : (_capacity * 2));

            new (&_Array[_size]) T(std::forward<Args>(args)...);
            _size++;
        }

        void reverse()
        {
            T* arr = new T[_capacity];
            size_t j= _size - 1;
            for (size_t i = 0; i < _size; i++){
                arr[j--] = _Array[i];
            }

            _delete_array_and_elements();
            _Array = arr;
        }

        void pop_back() noexcept {
            assert(_size > 0 && "Array is already empty!");
            _size--;
            _Array[_size].~T();
        }

        void clear() {
            std::destroy(begin(), end(), _Array);
            _size = 0;
        }

        void sort() { std::sort(_Array, _Array + _size); }

        size_t size() const noexcept{ return _size; }

        size_t capacity() const noexcept{ return _capacity; }
    };
}