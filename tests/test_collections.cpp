#include "../runtime/collections/List.h"
#include <iostream>
#include <cassert>

void testIntList() {
    aero::ArrayList<int> numbers;
    
    // Test add
    numbers.add(10);
    numbers.add(20);
    numbers.add(30);
    assert(numbers.size() == 3);
    
    // Test get
    assert(numbers.get(0) == 10);
    assert(numbers.get(1) == 20);
    assert(numbers.get(2) == 30);
    
    // Test contains
    assert(numbers.contains(20) == true);
    assert(numbers.contains(99) == false);
    
    // Test indexOf
    assert(numbers.indexOf(20) == 1);
    assert(numbers.indexOf(99) == -1);
    
    // Test remove by index
    int removed = numbers.removeAt(1);
    assert(removed == 20);
    assert(numbers.size() == 2);
    assert(numbers.get(1) == 30);
    
    // Test remove by value
    assert(numbers.remove(30) == true);
    assert(numbers.size() == 1);
    assert(numbers.remove(999) == false);
    
    // Test clear
    numbers.clear();
    assert(numbers.isEmpty() == true);
    
    std::cout << "✅ Int list tests passed!" << std::endl;
}

void testStringList() {
    aero::ArrayList<std::string> words;
    
    words.add("Hello");
    words.add("World");
    words.add("AeroLang");
    
    assert(words.size() == 3);
    assert(words.get(0) == "Hello");
    assert(words.contains("World") == true);
    
    // Test insert at index
    words.add(1, "Beautiful");
    assert(words.size() == 4);
    assert(words.get(1) == "Beautiful");
    assert(words.get(2) == "World");
    
    std::cout << "✅ String list tests passed!" << std::endl;
}

void testIterator() {
    aero::ArrayList<int> numbers;
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

void testOperators() {
    aero::ArrayList<int> numbers;
    numbers.add(10);
    numbers.add(20);
    numbers.add(30);
    
    // Test [] operator
    assert(numbers[0] == 10);
    assert(numbers[1] == 20);
    
    // Test [] assignment
    numbers[1] = 200;
    assert(numbers[1] == 200);
    
    std::cout << "✅ Operator tests passed!" << std::endl;
}

void testCapacity() {
    aero::ArrayList<int> numbers(100);
    
    // Add many elements
    for (int i = 0; i < 50; i++) {
        numbers.add(i);
    }
    
    assert(numbers.size() == 50);
    
    numbers.trimToSize();
    
    std::cout << "✅ Capacity tests passed!" << std::endl;
}

int main() {
    std::cout << "Running List<T> tests...\n" << std::endl;
    
    testIntList();
    testStringList();
    testIterator();
    testOperators();
    testCapacity();
    
    std::cout << "\n🎉 All tests passed!" << std::endl;
    
    return 0;
}
