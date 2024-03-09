#include <stddef.h>

template <typename T>
class Stack {
private:
    T* elements;
    size_t capacity;
    size_t size;

public:
    Stack(size_t initialCapacity = 10);
    ~Stack();
    bool push(const T& element);
    bool pop();
    bool top(T& result) const;
    bool empty() const;
    size_t getSize() const;
};

