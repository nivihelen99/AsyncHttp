cmake_minimum_required(VERSION 3.10)

# For a header-only library, the "linking" is primarily about making sure
# the include directories are correctly set up for the example executable.

# The CppHttpClientLib target is an INTERFACE library, so its
# INTERFACE_INCLUDE_DIRECTORIES will be propagated.

# Add the executable
add_executable(SimpleExample example.cpp)

# "Link" to the header-only library. This makes its include directories available.
# It also establishes a dependency, so if the library were not header-only and
# needed to be built, CMake would build it before the example.
target_link_libraries(SimpleExample PRIVATE CppHttpClientLib::CppHttpClientLib)

message(STATUS "SimpleExample executable added in examples/CMakeLists.txt")

# Add the asynchronous example executable
add_executable(AsyncExample example_async.cpp)

# Link AsyncExample to the header-only library and pthreads (for std::thread)
target_link_libraries(AsyncExample PRIVATE CppHttpClientLib::CppHttpClientLib Threads::Threads)

message(STATUS "AsyncExample executable added in examples/CMakeLists.txt")
