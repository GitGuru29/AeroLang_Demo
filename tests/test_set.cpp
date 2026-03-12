#include "../runtime/collections/Set.h"
#include <iostream>
#include <cassert>
#include <string>

void testBasicOperations() {
    aero::HashSet<int> numbers;
    
    // Test add
    assert(numbers.add(10) == true);
    assert(numbers.add(20) == true);
    assert(numbers.add(10) == false);  // Duplicate
    
    assert(numbers.size() == 2);
    assert(numbers.contains(10) == true);
    assert(numbers.contains(99) == false);
    
    std::cout << "✅ Basic operations passed!" << std::endl;
}

void testRemove() {
    aero::HashSet<std::string> tags;
    
    tags.add("coding");
    tags.add("design");
    tags.add("music");
    
    assert(tags.size() == 3);
    assert(tags.remove("design") == true);
    assert(tags.size() == 2);
    assert(tags.remove("missing") == false);
    
    std::cout << "✅ Remove tests passed!" << std::endl;
}

void testUnion() {
    aero::HashSet<int> setA;
    aero::HashSet<int> setB;
    
    setA.add(1);
    setA.add(2);
    setA.add(3);
    
    setB.add(3);
    setB.add(4);
    setB.add(5);
    
    auto result = setA.union_(setB);
    assert(result.size() == 5);
    assert(result.contains(1) == true);
    assert(result.contains(5) == true);
    
    std::cout << "✅ Union tests passed!" << std::endl;
}

void testIntersection() {
    aero::HashSet<int> setA;
    aero::HashSet<int> setB;
    
    setA.add(1);
    setA.add(2);
    setA.add(3);
    
    setB.add(2);
    setB.add(3);
    setB.add(4);
    
    auto result = setA.intersection(setB);
    assert(result.size() == 2);
    assert(result.contains(2) == true);
    assert(result.contains(3) == true);
    assert(result.contains(1) == false);
    
    std::cout << "✅ Intersection tests passed!" << std::endl;
}

void testDifference() {
    aero::HashSet<int> setA;
    aero::HashSet<int> setB;
    
    setA.add(1);
    setA.add(2);
    setA.add(3);
    
    setB.add(2);
    setB.add(3);
    
    auto result = setA.difference(setB);
    assert(result.size() == 1);
    assert(result.contains(1) == true);
    assert(result.contains(2) == false);
    
    std::cout << "✅ Difference tests passed!" << std::endl;
}

void testSubset() {
    aero::HashSet<int> setA;
    aero::HashSet<int> setB;
    
    setA.add(1);
    setA.add(2);
    
    setB.add(1);
    setB.add(2);
    setB.add(3);
    
    assert(setA.isSubsetOf(setB) == true);
    assert(setB.isSubsetOf(setA) == false);
    
    std::cout << "✅ Subset tests passed!" << std::endl;
}

void testToVector() {
    aero::HashSet<std::string> words;
    
    words.add("hello");
    words.add("world");
    words.add("test");
    
    auto vec = words.toVector();
    assert(vec.size() == 3);
    
    std::cout << "✅ ToVector tests passed!" << std::endl;
}

void testIterator() {
    aero::HashSet<int> numbers;
    
    numbers.add(1);
    numbers.add(2);
    numbers.add(3);
    
    int sum = 0;
    for (const auto& num : numbers) {
        sum += num;
    }
    assert(sum == 6);
    
    std::cout << "✅ Iterator tests passed!" << std::endl;
}

void testClear() {
    aero::HashSet<int> numbers;
    
    numbers.add(1);
    numbers.add(2);
    numbers.add(3);
    
    assert(numbers.isEmpty() == false);
    numbers.clear();
    assert(numbers.isEmpty() == true);
    assert(numbers.size() == 0);
    
    std::cout << "✅ Clear tests passed!" << std::endl;
}

void testAddAll() {
    aero::HashSet<int> setA;
    aero::HashSet<int> setB;
    
    setA.add(1);
    setA.add(2);
    
    setB.add(3);
    setB.add(4);
    
    setA.addAll(setB);
    assert(setA.size() == 4);
    assert(setA.contains(3) == true);
    
    std::cout << "✅ AddAll tests passed!" << std::endl;
}

int main() {
    std::cout << "Running Set<T> tests...\n" << std::endl;
    
    testBasicOperations();
    testRemove();
    testUnion();
    testIntersection();
    testDifference();
    testSubset();
    testToVector();
    testIterator();
    testClear();
    testAddAll();
    
    std::cout << "\n🎉 All Set tests passed!" << std::endl;
    
    return 0;
}
