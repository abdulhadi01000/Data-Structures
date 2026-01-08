#pragma once
#include <stdexcept>
#include <algorithm>
#include <cassert>
#include <sstream>
#include <initializer_list>

namespace DataStructures {
	template<typename T>class dlist {
	private:
		struct node {
			T data;
			node* next;
			node* prev;
			node(const T& value, node* next = nullptr, node* prev = nullptr) :
				data(value), next(next), prev(prev) {}

			template <typename... Args>
			node(node* next_ptr = nullptr, node* prev_ptr= nullptr, Args&&... args) :
				data(std::forward<Args>(args)...), next(next_ptr), prev(prev_ptr) {
			}
		};
		node* _head;
		node* _tail;
		size_t _size;

		node* split(node* head) {
			node* fast = head;
			node* slow = head;

			while (fast->next != nullptr && fast->next->next != nullptr) {
				fast = fast->next->next;
				slow = slow->next;
			}

			node* temp = slow->next;
			slow->next = nullptr;
			if (temp) temp->prev = nullptr;
			return temp;
		}

		node* merge(node* first, node* second) {
			if (!first) return second;
			if (!second) return first;

			if (first->data < second->data) {
				first->next = merge(first->next, second);
				if (first->next) first->next->prev = first;
				first->prev = nullptr;
				return first;
			}
			else {
				second->next = merge(first, second->next);
				if (second->next) second->next->prev = second;
				second->prev = nullptr;
				return second;
			}
		}

		node* mergeSort(node* head) {
			if (!head || !head->next) return head;
			node* second = split(head);
			head = mergeSort(head);
			second = mergeSort(second);
			return merge(head, second);
		}

		void bubble_sort() {
			if (!_head || !_head->next)return;
			bool isSwaped;
			node* current, *last_ptr = nullptr;

			do{
				isSwaped = false;
				current = _head;
				while (current->next != last_ptr) {

					if (current->data > current->next->data) {
						T temp = current->data;
						current->data = current->next->data;
						current->next->data = temp;
						isSwaped = true;
					}
					current = current->next;
				}
				last_ptr = current;
			}while (isSwaped);
		}

	public:
		class reverse_iterator {
			friend class dlist;
			node* nodePtr;
			reverse_iterator(node* newPtr) : nodePtr(newPtr) {}
		public:
			reverse_iterator() : nodePtr(nullptr) {}

			bool operator !=(const reverse_iterator& iter)const {
				return nodePtr != iter.nodePtr;
			}

			bool operator ==(const reverse_iterator& iter) const {
				return nodePtr == iter.nodePtr;
			}

			T& operator*() const {
				return nodePtr->data;
			}

			T* operator->() const {
				return &(nodePtr->data);
			}

			reverse_iterator& operator++() {
				nodePtr = nodePtr->prev;
				return *this;
			}

			reverse_iterator operator++(int) {
				reverse_iterator temp = *this;
				nodePtr = nodePtr->prev;
				return temp;
			}

		};
		class const_reverse_iterator {
			friend class dlist;
			node* nodePtr;
			const_reverse_iterator(node* newPtr) : nodePtr(newPtr) {}
		public:
			const_reverse_iterator() : nodePtr(nullptr) {}

			bool operator !=(const const_reverse_iterator& iter) const {
				return nodePtr != iter.nodePtr;
			}

			bool operator ==(const const_reverse_iterator& iter) const {
				return nodePtr == iter.nodePtr;
			}

			const T& operator*() const {
				return nodePtr->data;
			}

			const T* operator->() const {
				return &(nodePtr->data);
			}

			const_reverse_iterator& operator++() {
				nodePtr = nodePtr->prev;
				return *this;
			}

			const_reverse_iterator operator++(int) {
				const_reverse_iterator temp = *this;
				nodePtr = nodePtr->prev;
				return temp;
			}

		};
		class iterator {
			friend class dlist;
			node* nodePtr;
			iterator(node* newPtr) : nodePtr(newPtr) {}
		public:
			iterator() : nodePtr(nullptr) {}

			bool operator !=(const iterator& iter)const {
				return nodePtr != iter.nodePtr;
			}

			bool operator ==(const iterator& iter) const {
				return nodePtr == iter.nodePtr;
			}

			T& operator*() const {
				return nodePtr->data;
			}

			T* operator->() const {
				return &(nodePtr->data);
			}

			iterator& operator++() {
				nodePtr = nodePtr->next;
				return *this;
			}

			iterator& operator++(int) {
				iterator* temp = this;
				nodePtr = nodePtr->next;
				return *temp;
			}

		};
		class const_iterator {
			friend class dlist;
			node* nodePtr;
			const_iterator(node* newPtr) : nodePtr(newPtr) {}
		public:
			const_iterator() : nodePtr(nullptr) {}

			bool operator !=(const const_iterator& iter) const {
				return nodePtr != iter.nodePtr;
			}

			bool operator ==(const const_iterator& iter) const {
				return nodePtr == iter.nodePtr;
			}

			const T& operator*() const {
				return nodePtr->data;
			}

			const T* operator->() const {
				return &(nodePtr->data);
			}

			const_iterator& operator++() {
				nodePtr = nodePtr->next;
				return *this;
			}

			const_iterator operator++(int) {
				const_iterator temp = *this;
				nodePtr = nodePtr->next;
				return temp;
			}
		};
		reverse_iterator rbegin() { return reverse_iterator(_tail); }
		reverse_iterator rend() { return reverse_iterator(nullptr); }
		const_reverse_iterator crbegin() const { return const_reverse_iterator(_tail); }
		const_reverse_iterator crend() const { return const_reverse_iterator(nullptr); }
		iterator begin() { return iterator(_head); }
		iterator end() { return   iterator(nullptr); }
		const_iterator begin() const { return const_iterator(_head); }
		const_iterator end() const { return const_iterator(nullptr); }


		dlist() :_size(0), _head(nullptr), _tail(nullptr) {}
		dlist(const std::initializer_list<T>& list) 
			:_size(0), _head(nullptr), _tail(nullptr) {
			for (const T& l : list)emplace_back(l);
		}
		~dlist(){clear();}

		template<typename... Args>
		void emplace(size_t index,Args&&... value) {
			if (index > _size) return;
			if (index == 0) { emplace_front(std::forward<Args>(value)...); return; }
			if (index == _size) { emplace_back(std::forward<Args>(value)...); return; }
			node* atIndex = _head;

			for (size_t counter = 0; counter < index; counter++) {
				atIndex = atIndex->next;
			}

			node* newnode = new node(nullptr, nullptr, std::forward<Args>(value)...);
			node* preIndex = atIndex->prev;

			newnode->next = atIndex;
			newnode->prev = preIndex;

			preIndex->next = newnode;
			atIndex->prev = newnode;

			_size++;
		}
		template<typename... Args>
		void emplace_front(Args&&... value) {
			node* newnode = new node(nullptr, nullptr, std::forward<Args>(value)...);

			if (is_empty()) {
				_head = newnode;
				_tail = newnode;
			}
			else {
				newnode->next = _head;
				_head->prev = newnode;
				_head = newnode;
			}
			_size++;
		}
		template<typename... Args>
		void emplace_back(Args&&... value){
			node* newnode = new node(nullptr,nullptr,std::forward<Args>(value)...);

			if (is_empty()) {
				_head = newnode;
				_tail = newnode;
			}
			else {
				newnode->prev = _tail;
				_tail->next = newnode;
				_tail = newnode;
			}

			_size++;
		}

		void push_back (const T& value) {
			node* newnode = new node(value);
			
			if (is_empty()) {
				_head = newnode;
				_tail = newnode;
			}
			else {
				newnode->prev = _tail;
				_tail->next = newnode;
				_tail = newnode;
			}

			_size++;
		}
		void push_front (const T& value) {	
			node* newnode = new node(value);
			newnode->next = _head;

			if (is_empty()) { 
				_head = newnode;
				_tail = newnode; 
			}
			else {
				_head->prev = newnode;
				_head = newnode;
			}
			_size++;
		}
		void insert (const T& value, size_t index) {
			if (index > _size) return;
			if (index == 0) { push_front(value); return; }
			if (index == _size) { push_back(value); return; }
			node* atIndex = _head;
			
			for (size_t counter = 0; counter < index; counter++) {
				atIndex = atIndex->next;
			}

			node* newnode = new node(value);
			node* preIndex = atIndex->prev;

			newnode->next = atIndex;
			newnode->prev = preIndex;

			preIndex->next = newnode;
			atIndex->prev = newnode;

			_size++;
		}

		bool find (const T& value) const {
			if (is_empty())return false;

			node* temp = _head;
			while (temp != nullptr) {
				if (temp->data == value) return true;
				temp = temp->next;
			}

			return false;
		}
		size_t find_at (const T& value) const {
			if (is_empty())return _size;

			size_t index = 0;
			for (node* temp = _head; temp != nullptr; temp = temp->next) {
				if (temp->data == value)return index;
				index++;
			}

			return _size;
		}

		bool rfind (const T& value) const {
			if (is_empty())return false;
			
			node* temp = _tail;

			while (temp != nullptr) {
				if (temp->data == value)return true;
				temp = temp->prev;
			}
			return false;
		}
		size_t rfind_at (const T& value) const {
			if (is_empty())return _size;
			size_t index = 0;

			for (node* temp = _tail; temp != nullptr; temp = temp->prev) {
				if (temp->data == value)return index;
				index++;
			}
			return _size;
		}

		std::string to_string (const char* sep = " ,") const {
			if (is_empty()) return "{}";
			std::stringstream ss;

			node* temp = _head;
			ss << "{" << temp->data;
			temp = temp->next;
			while (temp != nullptr) {
				ss << sep << temp->data;
				temp = temp->next;
			}
			ss << "}";

			return ss.str();
		
		}
		void print(const char* sep = " ,")const {
			std::cout << to_string(sep) << "\n";
		}

		std::string rto_string(const char*sep = " , ")const{
			if (is_empty())return "{}";
			std::stringstream ss;
			node* temp = _tail;
			ss << "{" << temp->data;
			temp = temp->prev;
			while (temp) {
				ss << sep << temp->data;
				temp = temp->prev;
			}
			ss << "}";
			return ss.str();
		}
		void rprint(const char* sep = " ,")const { 
			std::cout << rto_string(sep) << "\n";
		}
		
		void pop_back() {
			if (_tail == nullptr)return;

			node* old_tail = _tail;
			_tail = _tail->prev;
			delete old_tail;

			if (_tail == nullptr)
				_head = nullptr;
			else
				_tail->next = nullptr;
			_size--;
		}
		void pop_front() {
			if (_head == nullptr)return;

			node* old_head = _head;
			_head = _head->next;
			delete old_head;

			if (_head == nullptr)
				_tail = nullptr;
			else
				_head->prev = nullptr;

			_size--;
		}

#if 1
		void remove(const T& value) {
			if (is_empty())return;
			node* temp = _head;
			while (temp) {
				node* next = temp->next;
				if (temp->data == value) {
					if (temp == _head) { pop_front(); }
					else if (temp == _tail) { pop_back(); }
					else {
						temp->prev->next = temp->next;
						temp->next->prev = temp->prev;
						delete temp;
						--_size;
					}
				}
				temp = next;
			}
		}
#else
		void remove(const T& value) {
			if (is_empty()) return;
			if (value == _head->data)
			{
				pop_front();
				return;
			}
			if (value == _tail->data)
			{
				pop_back();
				return;
			}

			node* temp = _head;
			while (temp != nullptr) {
				if (temp->data == value) {
					temp->prev->next = temp->next;
					temp->next->prev = temp->prev;
					delete temp;
					--_size;
					return;
				}
				temp = temp->next;
			}
		}

#endif
		void remove(const size_t& index){
			if (index >= _size)return;
			if (index == 0) {pop_front(); return;}
			if (index == _size - 1) {pop_back(); return;}

			node* temp = _head;
			for (size_t counter = 0; counter < index; counter++) {
				temp = temp->next;
			}

			temp->prev->next = temp->next;
			temp->next->prev = temp->prev;
			delete temp;
			_size--;
		}	

		void delete_smallest() {
			if (is_empty())return;
			node* temp1 = _head,* temp2 = _head;
			node* smallestNode = _head;
			while (temp1) {
				while (temp2) {
					if (temp2->data < temp1->data) 
					{smallestNode = temp2;}
					temp2 = temp2->next;
				}
				temp1 = temp1->next;
			}

			node* temp = _head;
			T smallest = smallestNode->data;
			while (temp) {
				node* next = temp->next;
				if (temp->data == smallest)
				{
					if (temp->data == _head->data){
						_head = _head->next;
						delete temp;
						_head->prev = nullptr;
					}
					else if (temp->data == _tail->data){
						_tail = _tail->prev;
						delete temp;
						_tail->next = nullptr;
					}
					else{
						temp->prev->next = temp->next;
						temp->next->prev = temp->prev;
						delete temp;
					}
				}
				temp = next;
			}
		}

		void reverse() {
			if (_size == 1 || is_empty())return;
	
			node* current = _head;
			node* temp = nullptr;
			while (current) {
				temp = current->prev;
				current->prev = current->next;
				current->next = temp;
				current = current->prev;
			}
	
			temp = _head;
			_head = _tail;
			_tail = temp;
		}

		void clear() {
			node* next;
			while (_head){
				next = _head->next;
				delete _head;
				_head = next;
			}
			_head = nullptr;
			_tail = nullptr;
			_size = 0;
		}

		T& operator[](size_t index) { 
			return const_cast<T&>(static_cast<const dlist<T>&>(*this).at(index)); }
		const T& operator[](size_t index) const { return at(index); }
		T& at(size_t index) {
			return const_cast<T&>(static_cast<const dlist<T>&>(*this).at(index));
		}
		const T& at(size_t index) const {
			if (is_empty())throw std::out_of_range("list is empty");
			if (index >= _size) throw std::out_of_range("Index out of range");
			if (index == 0)return front();
			if (index == _size - 1) return back();

			node* temp = nullptr;
			if (index < _size/2) {
				temp = _head;
				for (size_t i = 0; i < index; i++){
					temp = temp->next;
				}
			}
			else {
				temp = _tail;
				for (size_t i = _size - 1; i > index; i--){
					temp = temp->prev;
				}
			}
			return temp->data;
		}

		T& front() { 
			if (is_empty())throw std::runtime_error("list is empty");
			return _head->data; }
		T& back() { 
			if (is_empty())throw std::runtime_error("list is empty");
			return _tail->data; 
		}

		const T& front() const {
			if (is_empty())throw std::runtime_error("list is empty");
			return _head->data; }
		const T& back() const {
			if (is_empty())throw std::runtime_error("list is empty");
			return _tail->data; }

		void sort() {
		/*	_head = mergeSort(_head);
			node* current = _head;
			while (current && current->next) {
				current = current->next;
			}
			_tail = current;*/
			bubble_sort();
		}

		size_t size() const { return _size; }
		bool is_empty() const { return !_head; }
	};
}