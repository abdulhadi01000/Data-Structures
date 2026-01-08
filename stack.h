#pragma once
#include <cassert>
#include <iostream>
namespace DataStructures {
	template <typename T>class array_stack {
	private:
		T* _array;
		size_t _capacity;
		size_t _size;
	public:
		array_stack(size_t s) :_array(new T[s]), _capacity(s), _size(0) {}
		array_stack(const array_stack& other) {
			_capacity = other._capacity;
			_size = other._size;
			for (size_t i = 0; i < other._size; i++) {
				_array[i] = other._array[i];
			}
		}
		~array_stack() {
			for (size_t i = 0; i < _size; ++i) {
				_array[i].~T();
			}
		}

		void push(const T& value) {
			if (_size >= _capacity) {
				throw std::length_error("Stack is full (push)");
			}
			_array[_size] = value;
			++_size;
		}

		template<typename... Args>
		void emplace(Args&&... value) {
			if (_size >= _capacity) {
				throw std::length_error("Stack is full (emplace)");
			}
			new (&_array[_size]) T(std::forward<Args>(value)...);
			_size++;
		}


		void pop() {
			if (is_empty()) {
				throw std::length_error("Stack is empty");
			}
			_array[_size - 1].~T();
			--_size;
		}

		T& top() {
			if (is_empty()) {
				throw std::out_of_range("Stack is empty");
			}
			return _array[_size - 1];
		}

		size_t size() { return _size; }
		bool is_empty() { return (_size == 0); }
	};
}