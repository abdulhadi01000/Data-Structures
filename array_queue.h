#pragma once
#include <algorithm>
#include <sstream>


namespace DataStructures {
template<typename T>
class array_queue {
	size_t _capacity;
	size_t _size;
	T* _array;
public:
	array_queue(size_t capacity):
		_capacity(capacity),_size(0),
		_array(static_cast<T*>(::operator
			new(capacity * sizeof(T)))){}

	array_queue(std::initializer_list<T> list):
		_capacity(list.size()),_size(list.size()),
		_array(static_cast<T*>(::operator 
			new(list.size() * sizeof(T)))){
		size_t i = 0;
		for (const auto& data:list)_array[i++] = data;
	}

	~array_queue() {
		for (int i = 0; i < _size; i++)_array[i].~T();
		::operator delete (_array);
	}

	void enqueue(T data) {
		if (_size == _capacity) return;
		new (&_array[_size]) T(data);
		_size++;
	}

	void dequeue() {
		if (_size == 0) return;
		_array[0].~T();
		for (size_t i = 0; i < _size - 1; i++) {
			_array[i] = std::move(_array[i + 1]);
		}
		_size--;
	}

	std::string to_string(const char* sep = " ,") {
		std::stringstream ss;
		ss << "{";
		ss << _array[0];
		for (size_t i = 1; i < _size; i++) {
			ss << sep <<_array[i] ;
		}
		ss << "}";
		return ss.str();
	}

	T& front() { return _array[0]; }
	T& rear() { return _array[_size - 1]; }
	size_t size() { return _size; }
	bool is_empty() { return _size == 0; }
	bool is_full() { return _size >= _capacity; }
};
}