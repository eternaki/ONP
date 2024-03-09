#include <cstdlib>
#include "stack.h"

template <typename T>
Stack<T>::Stack(size_t initialCapacity) : capacity(initialCapacity), size(0) {
    elements = static_cast<T*>(malloc(capacity * sizeof(T)));
}

template <typename T>
Stack<T>::~Stack() {
    free(elements);
}

template <typename T>
bool Stack<T>::push(const T& element) {
    if (size == capacity) {
        capacity *= 2;
        elements = static_cast<T*>(realloc(elements, capacity * sizeof(T)));
        if (!elements) {
            return false; // Ошибка при перевыделении памяти
        }
    }

    elements[size++] = element;
    return true;
}

template <typename T>
bool Stack<T>::pop() {
    if (!empty()) {
        --size;
        return true;
    }
    return false; // Стек пуст
}

template <typename T>
bool Stack<T>::top(T& result) const {
    if (!empty()) {
        result = elements[size - 1];
        return true;
    }
    return false; // Стек пуст
}

template <typename T>
bool Stack<T>::empty() const {
    return size == 0;
}

template <typename T>
size_t Stack<T>::getSize() const {
    return size;
}
