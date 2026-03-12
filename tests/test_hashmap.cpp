#include "../runtime/collections/Map.h"
#include <iostream>
#include <cassert>
#include <string>

void testBasicOperations() {
    aero::HashMap<std::string, int> map;
    
    // Test put and get
    map.put("one", 1);
    map.put("two", 2);
    map.put("three", 3);
    
    assert(map.size() == 3);
    assert(map.get("one") == 1);
    assert(map.get("two") == 2);
    assert(map.get("three") == 3);
    
    std::cout << "✅ Basic operations passed!" << std::endl;
}

void testContains() {
    aero::HashMap<std::string, std::string> contacts;
    
    contacts.put("Alice", "555-0101");
    contacts.put("Bob", "555-0102");
    
    assert(contacts.containsKey("Alice") == true);
    assert(contacts.containsKey("Charlie") == false);
    assert(contacts.containsValue("555-0101") == true);
    assert(contacts.containsValue("555-9999") == false);
    
    std::cout << "✅ Contains tests passed!" << std::endl;
}

void testRemove() {
    aero::HashMap<int, std::string> map;
    
    map.put(1, "one");
    map.put(2, "two");
    map.put(3, "three");
    
    std::string removed = map.remove(2);
    assert(removed == "two");
    assert(map.size() == 2);
    assert(map.containsKey(2) == false);
    
    std::cout << "✅ Remove tests passed!" << std::endl;
}

void testUpdate() {
    aero::HashMap<std::string, int> scores;
    
    scores.put("Alice", 100);
    assert(scores.get("Alice") == 100);
    
    // Update value
    scores.put("Alice", 200);
    assert(scores.get("Alice") == 200);
    assert(scores.size() == 1);  // Still one entry
    
    std::cout << "✅ Update tests passed!" << std::endl;
}

void testKeys() {
    aero::HashMap<std::string, int> map;
    
    map.put("a", 1);
    map.put("b", 2);
    map.put("c", 3);
    
    auto keys = map.keys();
    assert(keys.size() == 3);
    
    // Check all keys present
    bool hasA = false, hasB = false, hasC = false;
    for (const auto& key : keys) {
        if (key == "a") hasA = true;
        if (key == "b") hasB = true;
        if (key == "c") hasC = true;
    }
    assert(hasA && hasB && hasC);
    
    std::cout << "✅ Keys tests passed!" << std::endl;
}

void testValues() {
    aero::HashMap<int, std::string> map;
    
    map.put(1, "one");
    map.put(2, "two");
    map.put(3, "three");
    
    auto values = map.values();
    assert(values.size() == 3);
    
    std::cout << "✅ Values tests passed!" << std::endl;
}

void testGetOrDefault() {
    aero::HashMap<std::string, int> map;
    
    map.put("exists", 42);
    
    assert(map.getOrDefault("exists", 0) == 42);
    assert(map.getOrDefault("missing", 99) == 99);
    
    std::cout << "✅ GetOrDefault tests passed!" << std::endl;
}

void testOperators() {
    aero::HashMap<std::string, int> map;
    
    map["one"] = 1;
    map["two"] = 2;
    
    assert(map["one"] == 1);
    assert(map["two"] == 2);
    assert(map.size() == 2);
    
    // Update via operator
    map["one"] = 100;
    assert(map["one"] == 100);
    
    std::cout << "✅ Operator tests passed!" << std::endl;
}

void testIterator() {
    aero::HashMap<std::string, int> map;
    
    map.put("a", 1);
    map.put("b", 2);
    map.put("c", 3);
    
    int sum = 0;
    for (const auto& pair : map) {
        sum += pair.second;
    }
    assert(sum == 6);
    
    std::cout << "✅ Iterator tests passed!" << std::endl;
}

void testClear() {
    aero::HashMap<int, int> map;
    
    map.put(1, 1);
    map.put(2, 2);
    map.put(3, 3);
    
    assert(map.isEmpty() == false);
    
    map.clear();
    assert(map.isEmpty() == true);
    assert(map.size() == 0);
    
    std::cout << "✅ Clear tests passed!" << std::endl;
}

int main() {
    std::cout << "Running HashMap<K,V> tests...\n" << std::endl;
    
    testBasicOperations();
    testContains();
    testRemove();
    testUpdate();
    testKeys();
    testValues();
    testGetOrDefault();
    testOperators();
    testIterator();
    testClear();
    
    std::cout << "\n🎉 All HashMap tests passed!" << std::endl;
    
    return 0;
}
