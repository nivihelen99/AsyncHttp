cmake_minimum_required(VERSION 3.10)
project(CppHttpClientLib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- Library ---
# For a header-only library, we mainly need to define the interface
# include directories.
add_library(CppHttpClientLib INTERFACE)
add_library(CppHttpClientLib::CppHttpClientLib ALIAS CppHttpClientLib)

target_include_directories(CppHttpClientLib INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# --- Examples ---
# Add the examples directory
add_subdirectory(examples)

# --- Tests ---
# Enable testing and add the tests directory
enable_testing()
add_subdirectory(tests)

# --- Installation ---
# Example of how to install the library (headers)
install(TARGETS CppHttpClientLib
    EXPORT CppHttpClientLibTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/
    DESTINATION include
)

install(EXPORT CppHttpClientLibTargets
    FILE CppHttpClientLibTargets.cmake
    NAMESPACE CppHttpClientLib::
    DESTINATION lib/cmake/CppHttpClientLib
)

# --- Packaging (CPack) ---
include(CPack)
