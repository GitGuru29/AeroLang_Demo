#ifndef AERO_LIST_H
#define AERO_LIST_H

#include <vector>
#include <stdexcept>
#include <memory>
#include <algorithm>

namespace aero {

template<typename T>
class List {
protected:
    std::vector<T> data;

public:
    List() = default;
    virtual ~List() = default;

    // Core operations
    virtual void add(const T& element) {
        data.push_back(element);
    }

    virtual void add(int index, const T& element) {
        if (index < 0 || index > size()) {
            throw std::out_of_range("Index out of bounds");
        }
        data.insert(data.begin() + index, element);
    }

    virtual T get(int index) const {
        if (index < 0 || index >= size()) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }

    virtual T removeAt(int index) {
        if (index < 0 || index >= size()) {
            throw std::out_of_range("Index out of bounds");
        }
        T element = data[index];
        data.erase(data.begin() + index);
        return element;
    }

    virtual bool remove(const T& element) {
        auto it = std::find(data.begin(), data.end(), element);
        if (it != data.end()) {
            data.erase(it);
            return true;
        }
        return false;
    }

    virtual void clear() {
        data.clear();
    }

    virtual int size() const {
        return static_cast<int>(data.size());
    }

    virtual bool isEmpty() const {
        return data.empty();
    }

    virtual bool contains(const T& element) const {
        return std::find(data.begin(), data.end(), element) != data.end();
    }

    virtual int indexOf(const T& element) const {
        auto it = std::find(data.begin(), data.end(), element);
        if (it != data.end()) {
            return static_cast<int>(std::distance(data.begin(), it));
        }
        return -1;
    }

    // Iterator support
    typename std::vector<T>::iterator begin() {
        return data.begin();
    }

    typename std::vector<T>::iterator end() {
        return data.end();
    }

    typename std::vector<T>::const_iterator begin() const {
        return data.begin();
    }

    typename std::vector<T>::const_iterator end() const {
        return data.end();
    }

    // Operator overloads
    T& operator[](int index) {
        if (index < 0 || index >= size()) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }

    const T& operator[](int index) const {
        if (index < 0 || index >= size()) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }
};

// ArrayList - dynamic array implementation
template<typename T>
class ArrayList : public List<T> {
public:
    ArrayList() = default;
    
    ArrayList(int initialCapacity) {
        this->data.reserve(initialCapacity);
    }

    void ensureCapacity(int minCapacity) {
        if (minCapacity > this->data.capacity()) {
            this->data.reserve(minCapacity);
        }
    }

    void trimToSize() {
        this->data.shrink_to_fit();
    }
};

} // namespace aero

#endif // AERO_LIST_H
