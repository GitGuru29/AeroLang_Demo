cmake_minimum_required(VERSION 3.10)

# Test executable for Lexer
add_executable(test_lexer test_lexer.cpp)
target_link_libraries(test_lexer aerolang_compiler)
target_include_directories(test_lexer PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Test executable for Collections
add_executable(test_collections test_collections.cpp)
target_include_directories(test_collections PRIVATE ${CMAKE_SOURCE_DIR}/runtime)

# Test executable for HashMap
add_executable(test_hashmap test_hashmap.cpp)
target_include_directories(test_hashmap PRIVATE ${CMAKE_SOURCE_DIR}/runtime)

# Test executable for Set
add_executable(test_set test_set.cpp)
target_include_directories(test_set PRIVATE ${CMAKE_SOURCE_DIR}/runtime)

# Add tests to CTest
enable_testing()
add_test(NAME LexerTest COMMAND test_lexer)
add_test(NAME CollectionsTest COMMAND test_collections)
add_test(NAME HashMapTest COMMAND test_hashmap)
add_test(NAME SetTest COMMAND test_set)
