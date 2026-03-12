#ifndef AERO_SET_H
#define AERO_SET_H

#include <unordered_set>
#include <stdexcept>
#include <vector>

namespace aero {

template<typename T>
class Set {
protected:
    std::unordered_set<T> data;

public:
    Set() = default;
    virtual ~Set() = default;

    // Core operations
    virtual bool add(const T& element) {
        auto result = data.insert(element);
        return result.second;  // true if inserted, false if already exists
    }

    virtual bool remove(const T& element) {
        return data.erase(element) > 0;
    }

    virtual bool contains(const T& element) const {
        return data.find(element) != data.end();
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

    // Set operations
    virtual Set<T> union_(const Set<T>& other) const {
        Set<T> result;
        result.data = this->data;
        for (const auto& element : other.data) {
            result.data.insert(element);
        }
        return result;
    }

    virtual Set<T> intersection(const Set<T>& other) const {
        Set<T> result;
        for (const auto& element : this->data) {
            if (other.contains(element)) {
                result.add(element);
            }
        }
        return result;
    }

    virtual Set<T> difference(const Set<T>& other) const {
        Set<T> result;
        for (const auto& element : this->data) {
            if (!other.contains(element)) {
                result.add(element);
            }
        }
        return result;
    }

    virtual bool isSubsetOf(const Set<T>& other) const {
        for (const auto& element : this->data) {
            if (!other.contains(element)) {
                return false;
            }
        }
        return true;
    }

    // Convert to vector
    virtual std::vector<T> toVector() const {
        std::vector<T> result;
        result.reserve(data.size());
        for (const auto& element : data) {
            result.push_back(element);
        }
        return result;
    }

    // Iterator support
    typename std::unordered_set<T>::iterator begin() {
        return data.begin();
    }

    typename std::unordered_set<T>::iterator end() {
        return data.end();
    }

    typename std::unordered_set<T>::const_iterator begin() const {
        return data.begin();
    }

    typename std::unordered_set<T>::const_iterator end() const {
        return data.end();
    }
};

// HashSet - hash table implementation
template<typename T>
class HashSet : public Set<T> {
public:
    HashSet() = default;
    
    HashSet(int initialCapacity) {
        this->data.reserve(initialCapacity);
    }

    void ensureCapacity(int minCapacity) {
        if (minCapacity > this->data.bucket_count()) {
            this->data.reserve(minCapacity);
        }
    }

    // Add all elements from another set
    void addAll(const HashSet<T>& other) {
        for (const auto& element : other) {
            this->data.insert(element);
        }
    }

    // Remove all elements in another set
    void removeAll(const HashSet<T>& other) {
        for (const auto& element : other) {
            this->data.erase(element);
        }
    }
};

} // namespace aero

#endif // AERO_SET_H
