#pragma once
#include <stdexcept>
#include <algorithm>
#include <cassert>
#include <sstream>
#include <initializer_list>
#include <string>

namespace DataStructures {
template <typename T>class queue {
	struct node {
		T value;
		node* next;
		node(node* next, const T& value) :
			value(value), 
			next(next) 
		{}

		template <typename... Args>
		node(node* next, Args&&... value) :
			value(std::forward<Args>(value)...),
			next(next)
		{}
	};
	node* _head;
	node* _tail;
	size_t _size;

public:
	queue():_head(nullptr),
		_tail(nullptr),
		_size(0)
	{}
	~queue() { clear(); }


	void push(const T& value) {
		node* new_node = new node(nullptr, value);
		if (is_empty()){
			_head = new_node;
			_tail = new_node;
		}
		else {
			_tail->next = new_node;
			_tail = new_node;
		}
		_size++;
	}

	template<typename... Args>
	void emplace(Args&&... value){
		node* new_node = new node(nullptr, std::forward<Args>(value)...);
		if (is_empty()) {
			_head = new_node;
			_tail = new_node;
		}
		else {
			_tail->next = new_node;
			_tail = new_node;
		}
		_size++;
	}

	void pop() { 
		if (is_empty())return;

		node* new_head = _head->next;
		delete _head;
		_head = new_head;

		_size--;
	}

	void clear(){
		while (_head != nullptr) {
			node* new_head = _head->next;
			delete _head;
			_head = new_head;
		}
		_head = nullptr;
		_tail = nullptr;
		_size = 0;
	}

	T& back() { return _tail->value; }
	T& front() { return _head->value; }
	size_t size() { return _size; }
	bool is_empty() { return _head == nullptr; }
};
}