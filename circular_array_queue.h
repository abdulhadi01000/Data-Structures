#pragma once
#include <initializer_list>
#include <stdexcept>
#include <new>

namespace DataStructures {
    template<typename T>
    class circular_array_queue {
        size_t _capacity;
        size_t _size;
        size_t _front;
        size_t _rear;
        T* _array;

    public:
        circular_array_queue(size_t capacity) :
            _capacity(capacity), _size(0),
            _front(0), _rear(0),
            _array(static_cast<T*>(::operator new(capacity * sizeof(T)))) {
        }

        circular_array_queue(std::initializer_list<T> list) :
            _capacity(list.size()), _size(0),
            _front(0), _rear(0),
            _array(static_cast<T*>(::operator new(list.size() * sizeof(T)))) {
            for (const auto& data : list) {
                enqueue(data);
            }
        }

        ~circular_array_queue() {
            while (!is_empty())dequeue();
            ::operator delete(_array);
        }

        void enqueue(T data) {
            if (is_full()) return;
            new (&_array[_rear]) T(data);
            _rear = (_rear + 1) % _capacity;
            _size++;
        }
        void dequeue() {
            if (is_empty()) return;
            _array[_front].~T();
            _front = (_front + 1) % _capacity;
            _size--;
        }

        std::string to_string(const char* sep = " , ") {
            std::stringstream ss;
            ss << "{";
            ss << _array[0];
            for (size_t i = 1; i < _size; i++) {
                ss << sep << _array[i];
            }
            ss << "}";
            return ss.str();
        }

        T& front() {
            if (is_empty()) throw std::runtime_error("Queue is empty");
            return _array[_front];
        }

        T& rear() {
            if (is_empty()) throw std::runtime_error("Queue is empty");
            return _array[(_rear == 0) ? (_capacity - 1) : (_rear - 1)];
        }

        size_t size() const { return _size; }
        bool is_empty() const { return _size == 0; }
        bool is_full() const { return _size == _capacity; }
    };
}
