cmake_minimum_required(VERSION 3.20)
project(AeroLang VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Project directories
set(AEROLANG_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(AEROLANG_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(AEROLANG_RUNTIME_DIR ${CMAKE_CURRENT_SOURCE_DIR}/runtime)
set(AEROLANG_TOOLS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tools)

# Compiler library (lexer, parser, code generator)
add_library(aerolang_compiler STATIC
    ${AEROLANG_SRC_DIR}/Lexer.cpp
    ${AEROLANG_SRC_DIR}/Parser.cpp
    ${AEROLANG_SRC_DIR}/AST.cpp
    ${AEROLANG_SRC_DIR}/SemanticAnalyzer.cpp
    ${AEROLANG_SRC_DIR}/CodeGenerator.cpp
)

target_include_directories(aerolang_compiler PUBLIC
    ${AEROLANG_INCLUDE_DIR}
)

# Compiler executable
add_executable(aeroc
    ${AEROLANG_TOOLS_DIR}/aeroc.cpp
)

target_link_libraries(aeroc PRIVATE aerolang_compiler)

# Runtime library (requires Android NDK - only build if available)
if(ANDROID_NDK)
    add_library(aerolang_runtime STATIC
        ${AEROLANG_RUNTIME_DIR}/Activity.cpp
        ${AEROLANG_RUNTIME_DIR}/View.cpp
        ${AEROLANG_RUNTIME_DIR}/String.cpp
        ${AEROLANG_RUNTIME_DIR}/Intent.cpp
        ${AEROLANG_RUNTIME_DIR}/Fragment.cpp
        ${AEROLANG_RUNTIME_DIR}/FragmentManager.cpp
        ${AEROLANG_RUNTIME_DIR}/SharedPreferences.cpp
        ${AEROLANG_RUNTIME_DIR}/JSON.cpp
        ${AEROLANG_RUNTIME_DIR}/FileIO.cpp
        ${AEROLANG_RUNTIME_DIR}/File.cpp
        ${AEROLANG_RUNTIME_DIR}/Math.cpp
        ${AEROLANG_RUNTIME_DIR}/DateTime.cpp
        ${AEROLANG_RUNTIME_DIR}/Regex.cpp
        ${AEROLANG_RUNTIME_DIR}/Base64.cpp
        ${AEROLANG_RUNTIME_DIR}/Crypto.cpp
        ${AEROLANG_RUNTIME_DIR}/URLEncoding.cpp
        ${AEROLANG_RUNTIME_DIR}/Path.cpp
        ${AEROLANG_RUNTIME_DIR}/URL.cpp
        ${AEROLANG_RUNTIME_DIR}/Stream.cpp
        ${AEROLANG_RUNTIME_DIR}/UUID.cpp
        ${AEROLANG_RUNTIME_DIR}/Log.cpp
        ${AEROLANG_RUNTIME_DIR}/Console.cpp
        ${AEROLANG_RUNTIME_DIR}/Timer.cpp
        ${AEROLANG_RUNTIME_DIR}/Database.cpp
        ${AEROLANG_RUNTIME_DIR}/HttpClient.cpp
        ${AEROLANG_RUNTIME_DIR}/Response.cpp
        ${AEROLANG_RUNTIME_DIR}/ImageLoader.cpp
        ${AEROLANG_RUNTIME_DIR}/WebSocket.cpp
        ${AEROLANG_RUNTIME_DIR}/Service.cpp
        ${AEROLANG_RUNTIME_DIR}/WorkManager.cpp
        ${AEROLANG_RUNTIME_DIR}/JNICache.cpp
        ${AEROLANG_RUNTIME_DIR}/BroadcastReceiver.cpp
        ${AEROLANG_RUNTIME_DIR}/NotificationManager.cpp
        ${AEROLANG_RUNTIME_DIR}/media/Camera.cpp
        ${AEROLANG_RUNTIME_DIR}/media/MediaPlayer.cpp
        ${AEROLANG_RUNTIME_DIR}/media/MediaRecorder.cpp
        ${AEROLANG_RUNTIME_DIR}/media/VideoView.cpp
        ${AEROLANG_RUNTIME_DIR}/hardware/LocationManager.cpp
        ${AEROLANG_RUNTIME_DIR}/hardware/SensorManager.cpp
    )
    
    target_include_directories(aerolang_runtime PUBLIC
        ${AEROLANG_RUNTIME_DIR}
    )
    target_link_libraries(aerolang_runtime PUBLIC log)
endif()

# Testing
enable_testing()
add_subdirectory(tests)

# Installation
install(TARGETS aeroc DESTINATION bin)
install(DIRECTORY ${AEROLANG_INCLUDE_DIR}/ DESTINATION include/aerolang)
install(DIRECTORY ${AEROLANG_RUNTIME_DIR}/ DESTINATION lib/aerolang/runtime)
