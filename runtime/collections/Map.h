#ifndef AERO_MAP_H
#define AERO_MAP_H

#include <unordered_map>
#include <stdexcept>
#include <utility>
#include <vector>

namespace aero {

template<typename K, typename V>
class Map {
protected:
    std::unordered_map<K, V> data;

public:
    Map() = default;
    virtual ~Map() = default;

    // Core operations
    virtual void put(const K& key, const V& value) {
        data[key] = value;
    }

    virtual V get(const K& key) const {
        auto it = data.find(key);
        if (it == data.end()) {
            throw std::out_of_range("Key not found");
        }
        return it->second;
    }

    virtual V getOrDefault(const K& key, const V& defaultValue) const {
        auto it = data.find(key);
        if (it == data.end()) {
            return defaultValue;
        }
        return it->second;
    }

    virtual bool containsKey(const K& key) const {
        return data.find(key) != data.end();
    }

    virtual bool containsValue(const V& value) const {
        for (const auto& pair : data) {
            if (pair.second == value) {
                return true;
            }
        }
        return false;
    }

    virtual V remove(const K& key) {
        auto it = data.find(key);
        if (it == data.end()) {
            throw std::out_of_range("Key not found");
        }
        V value = it->second;
        data.erase(it);
        return value;
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

    // Get all keys
    virtual std::vector<K> keys() const {
        std::vector<K> result;
        result.reserve(data.size());
        for (const auto& pair : data) {
            result.push_back(pair.first);
        }
        return result;
    }

    // Get all values
    virtual std::vector<V> values() const {
        std::vector<V> result;
        result.reserve(data.size());
        for (const auto& pair : data) {
            result.push_back(pair.second);
        }
        return result;
    }

    // Iterator support
    typename std::unordered_map<K, V>::iterator begin() {
        return data.begin();
    }

    typename std::unordered_map<K, V>::iterator end() {
        return data.end();
    }

    typename std::unordered_map<K, V>::const_iterator begin() const {
        return data.begin();
    }

    typename std::unordered_map<K, V>::const_iterator end() const {
        return data.end();
    }

    // Operator overloads
    V& operator[](const K& key) {
        return data[key];
    }

    const V& operator[](const K& key) const {
        auto it = data.find(key);
        if (it == data.end()) {
            throw std::out_of_range("Key not found");
        }
        return it->second;
    }
};

// HashMap - hash table implementation
template<typename K, typename V>
class HashMap : public Map<K, V> {
public:
    HashMap() = default;
    
    HashMap(int initialCapacity) {
        this->data.reserve(initialCapacity);
    }

    void ensureCapacity(int minCapacity) {
        if (minCapacity > this->data.bucket_count()) {
            this->data.reserve(minCapacity);
        }
    }

    // Merge another map into this one
    void putAll(const HashMap<K, V>& other) {
        for (const auto& pair : other) {
            this->data[pair.first] = pair.second;
        }
    }
};

} // namespace aero

#endif // AERO_MAP_H
