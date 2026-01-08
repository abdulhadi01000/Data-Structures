#pragma once
#include <stdexcept>
#include <algorithm>
#include <cassert>
#include <sstream>
#include <initializer_list>
#include <string>

namespace DataStructures {
    template<typename T>class LinkedList {
    private:
        struct node {
            T data;
            node* next;
            node(const T& e, node* next = nullptr) : 
                data(e), next(next) {}
        };
        size_t _size;
        node* _head;
        node* _tail;


        class iterator {
            friend class LinkedList;
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

            iterator operator++(int) {
                iterator temp = *this;
                nodePtr = nodePtr->next;
                return temp;
            }

        };
        class const_iterator {
            friend class LinkedList;
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
    public:
        iterator begin() { return iterator(_head); }
        iterator end() { return iterator(nullptr); }

        const_iterator begin() const { return const_iterator(_head); }
        const_iterator end() const { return const_iterator(nullptr); }

        void pop_front() {
            node* to_delete = _head;
            _head = _head->next;
            delete to_delete;
            --_size;
            if (_size == 0)_tail = nullptr;
        }
        void pop_back(){
            if (empty())return;
            node* prev = _head;
            while (prev->next != _tail) {
                prev = prev->next;
            }
            _tail = prev;
            delete prev->next;
            prev->next = nullptr;
            --_size;
        }
        void remove(const T& value) {
            if (empty())return;
            if (_head->data == value) { pop_front(); return; }

            node* prev = _head;
            node* current = _head->next;
            while (current != nullptr && current->data != value) {
                prev = current;
                current = current->next;
            }

            if (current == nullptr)return;

            prev->next = current->next;
            delete current;
            --_size;
        }
        void remove(const size_t& index) {
            if (index >= _size)return;

            if (empty())return;

            if (0 == index) { pop_front(); return; }

            node* prev = _head;
            for (size_t i = 0; i < index - 1; i++) {
                prev = prev->next;
            }
            node* current = prev->next;

            if (current == nullptr)return;
            if (current == _tail)_tail = prev;

            prev->next = current->next;
            delete current;
            --_size;
        }

        void reverse() {
            if (_size <= 1)return;

            LinkedList temp;
            node* current = _head;
            while (current != nullptr) {
                temp.push_front(current->data);
                current = current->next;
            }

            this->clear();
            *this = temp;
        }

        T& at(const size_t& index) {
            return const_cast<T&>(static_cast<const LinkedList<T>&>(*this).at(index));
        }
        const T& at(const size_t& index) const {
            if (empty())throw std::out_of_range("list is empty");
            if (index >= _size) throw std::out_of_range("out of bound");
            if (index == 0)return front();
            if (index == _size - 1)return back();

            node* temp = _head;
            for (size_t i = 0; i < index; i++) {
                temp = temp->next;
            }
            return (temp->data);
        }

        bool find(const T& value) const {
            if (empty())return false;

            node* temp = _head;
            while (temp != nullptr) {
                if (temp->data == value) return true;
                temp = temp->next;
            }

            return false;
        }
        size_t find_at(const T& value) const {
            if (empty())return _size;

            node* temp = _head;
            size_t index = 0;
            while (temp != nullptr) {
                if (temp->data == value) return index;
                index++;
                temp = temp->next;
            }

            return _size;
        }

        void push_front(const T& value) {
            node* Newnode = new node(value);
            Newnode->next = _head; 
            if (empty()) {_tail = Newnode;}
            _head = Newnode;
            _size++;
        }
        void push_back(const T& value) {
            node* Newnode = new node(value,nullptr);
            
            if (empty()) {
                _head = Newnode;
                _tail = Newnode;
            }
            else {
                _tail->next = Newnode;
                _tail = Newnode;
            }
            _size++;
        }
        void insert(const T& value, const size_t& index) {
            if (index > _size) {
                throw std::out_of_range("out of bound");
            }
            
            if (index == 0) {
                push_front(value);
                return;
            }
            else if (index == _size) {
                push_back(value);
                return;
            }
            else {
                node* newNode = new node(value);
                node* temp = _head;
                for (size_t counter = 0; counter < index - 1; ++counter) {
                    temp = temp->next;
                }
                newNode->next = temp->next;
                temp->next = newNode;
                _size++;
            }
        }
        void insert_after(const T& value, size_t index) {
            node* Newnode = new node(value);
            if (index >= _size) { index = _size - 1; }

            if (empty()) {
                _head = Newnode;
                _tail = Newnode;
                _size++;
                return;
            }

            node* temp = _head;
            for (size_t counter = 0; counter < index; counter++) {
                temp = temp->next;
            }

            Newnode->next = temp->next;
            temp->next = Newnode;
            if (temp = _tail)
                _tail = Newnode;
            _size++;
        }

        std::string toString(const char* sep = ", ") const {
            if (empty()) return "{}";
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
        void print(const char* sep = ", ")const { std::cout << toString(sep) << "\n"; }
        void printFirst() const {
            if (empty()) { std::cout << "{}"; return; }
            std::cout << "{" << front() << "}";
        }
        void printLast() const {
            if (empty()) { std::cout << "{}"; return; }
            std::cout << "{" << back() << "}";
        }

        LinkedList() :_size(0), _head(nullptr), _tail(nullptr) {}
        LinkedList(const LinkedList& other) :_size(0), _head(nullptr), _tail(nullptr) {
            node* current = other._head;
            while (current != nullptr) {
                push_back(current->data);
                current = current->next;
            }
        }
        LinkedList(size_t size) :_size(0), _head(nullptr), _tail(nullptr) {
            for (size_t i = 0; i < size; i++)push_back(T());
        }
        LinkedList(const std::initializer_list<T>& init_list) :
            _size(0), _head(nullptr), _tail(nullptr) {
            for (const T& value : init_list)
                push_back(std::move(const_cast<T&>(value)));
        }
        ~LinkedList() { clear(); }

        void clear() {
            while (_head != nullptr) {
                node* next = _head->next;
                delete _head;
                _head = next;
            }
            _head = nullptr;
            _tail = nullptr;
            _size = 0;
        }

        T& operator[](const size_t& index) { 
            return const_cast<T&>(static_cast<const LinkedList<T>&>(*this).at(index));
        }
        const T& operator[](const size_t& index) const { return at(index); }
        LinkedList& operator=(LinkedList other) noexcept {
            std::swap(_head, other._head);
            std::swap(_tail, other._tail);
            std::swap(_size, other._size);
            return *this;
        }

        bool empty() const { return (_head == nullptr); }
        T& back() const { return _tail->data; }
        T& front() const { return _head->data; }
        size_t size() const { return _size; }
    };
}