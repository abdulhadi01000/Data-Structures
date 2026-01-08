#pragma once
#include <utility> 
namespace DataStructures {
template <typename T> class dl_stack{
	struct node {
		T value;
		node* next;

		node(const T& _value,node* _next = nullptr) :
			value(_value),next(_next){}

		template <typename... Args>
		node(node* next_ptr, Args&&... _value) :
			value(std::forward<Args>(_value)...),
			next(next_ptr){}
	};
	node* _head;
	size_t _size;
public:
	dl_stack() : _head(nullptr), _size(0) {}
	~dl_stack() { clear(); }

	template<typename...Args>
	void emplace(Args&&... args) {
		node* newnode = new node(_head, std::forward<Args>(args)...);
		_head = newnode;
		_size++;
	}

	void push(const T& value) {
		node* newnode = new node(value,_head);
		_head = newnode;
		_size++;
	}

	void pop() {
		if (_head == nullptr) return;
		node* old_head = _head;
		_head = _head->next;
		delete old_head;
		_size--;
	}

	void clear() {
		while (_head != nullptr) { pop(); }
		_head = nullptr;
		_size = 0;
	}
	
	void swap(dl_stack& other) noexcept {
		if (this == &other)return;

		node* temp_head = other._head;
		size_t temp_size = other._size;
		
		other._head = this->_head;
		other._size = this->_size;
		
		this->_head = temp_head;
		this->_size = temp_size;
	}

	size_t size() { return _size; }
	bool is_empty() { return (_head == nullptr); }
	T& top() { return _head->value; }
};
}