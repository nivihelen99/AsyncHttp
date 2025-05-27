# C++ Async HTTP Client Library

A modern, lightweight, header-only C++17 HTTP client library designed to provide both synchronous and asynchronous operations with a clean, intuitive API.

**Note:** This library is currently under development. The network backend is **mocked**, meaning it does not perform real HTTP calls yet. It simulates responses to allow for API design and testing.

## Goals
-   Lightweight and header-only for easy integration.
-   Support for standard HTTP methods.
-   Asynchronous operations using futures.
-   Clean and intuitive API using modern C++ features.

## Core Features (Currently Implemented - Mocked Backend)

*   **HTTP Methods**:
    *   Synchronous `GET`
    *   Synchronous `POST` (with request body)
*   **Asynchronous Operations**:
    *   Asynchronous `GET` (returns `std::future<Response>`)
    *   Asynchronous `POST` (returns `std::future<Response>`)
    *   Utilizes an internal thread pool for async tasks.
*   **Request Customization**:
    *   Custom HTTP headers.
    *   Request timeouts.
*   **API Design**:
    *   Builder pattern for `http::Request` objects.
    *   Header-only library for easy integration.
    *   Uses C++17 features.

**Important**: All network operations are currently **mocked**. The library does not perform real network calls. Responses are simulated based on predefined rules in the mock backend, which is useful for API testing and development.

## Basic Usage Examples

Below are examples of how to use the library. Ensure you include the main client header:
```cpp
#include "cpp_http_client/Client.hpp" // For static get/post and async calls
#include "cpp_http_client/Request.hpp"  // For using the Request builder
#include <iostream>
#include <future> // Required for std::future
```

### 1. Synchronous GET Request
```cpp
#include "cpp_http_client/Client.hpp"
#include "cpp_http_client/Response.hpp"
#include <iostream>

int main() {
    http::Response response = http::Client::get("http://example.com/ok");
    std::cout << "Status: " << response.status_code() << std::endl;
    std::cout << "Body: " << response.body() << std::endl;
    return 0;
}
```

### 2. Synchronous POST Request with Body and Custom Headers
```cpp
#include "cpp_http_client/Request.hpp"
#include "cpp_http_client/Response.hpp"
#include <iostream>
#include <string>
#include <map>

int main() {
    http::Request post_request;
    post_request.url("http://example.com/submit")
                .method("POST")
                .header("Content-Type", "application/json")
                .header("X-Auth-Token", "my-secret-token")
                .body(R"({"name":"cpp-http-client","version":"0.1"})");

    http::Response response = post_request.send();
    std::cout << "Status: " << response.status_code() << std::endl;
    std::cout << "Body: " << response.body() << std::endl; // Mock server may echo body or headers
    
    std::cout << "Response Headers:" << std::endl;
    for(const auto& header_pair : response.headers()){
        std::cout << "  " << header_pair.first << ": " << header_pair.second << std::endl;
    }
    return 0;
}
```

### 3. Asynchronous GET Request
```cpp
#include "cpp_http_client/Client.hpp"
#include "cpp_http_client/Response.hpp"
#include <iostream>
#include <future> // Required for std::future

int main() {
    std::future<http::Response> future_response = http::Client::async_get("http://example.com/ok");

    std::cout << "Async request sent. Waiting for response..." << std::endl;
    // You can do other work here while the request is processed.

    try {
        http::Response response = future_response.get(); // Blocks until the result is available
        std::cout << "Async Response Status: " << response.status_code() << std::endl;
        std::cout << "Async Response Body: " << response.body() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
    return 0;
}
```

### 4. Setting a Timeout on a Request
The timeout is set on the `Request` object. The mock backend will simulate a timeout if its processing time exceeds this value.
```cpp
#include "cpp_http_client/Request.hpp"
#include "cpp_http_client/Response.hpp"
#include <iostream>
#include <chrono> // Required for std::chrono

int main() {
    http::Request request_with_timeout;
    request_with_timeout.url("http://example.com/timeout_sync") // Mocked to take ~100ms
                        .method("GET")
                        .timeout(std::chrono::milliseconds(50)); // Timeout set to 50ms

    http::Response response = request_with_timeout.send();
    // Expecting a 408 Request Timeout from the mock server
    std::cout << "Status (expecting 408): " << response.status_code() << std::endl; 
    std::cout << "Body: " << response.body() << std::endl;

    // For async requests, the future.get() will receive this 408 response.
    // Alternatively, you can use future.wait_for(timeout_duration) for a client-side wait timeout.
    // Example:
    // auto future_resp = http::Client::async_get("http://example.com/timeout_async");
    // std::future_status status = future_resp.wait_for(std::chrono::milliseconds(50));
    // if (status == std::future_status::timeout) {
    //     std::cout << "Client-side timeout waiting for future!" << std::endl;
    // } else if (status == std::future_status::ready) {
    //     http::Response resp_data = future_resp.get();
    //     // process resp_data, which might be a 408 if server_side_timeout < internal_mock_processing_time
    // }

    return 0;
}

```

## How to Build Examples

The library is header-only, so there's nothing to build for the library itself. You just need to include the headers in your project.

To build and run the provided examples:

1.  **Ensure you have CMake and a C++17 compatible compiler installed.**
2.  **Navigate to the root directory of this library (`cpp_http_client_lib`).**
3.  **Create a build directory and navigate into it:**
    ```bash
    mkdir build
    cd build
    ```
4.  **Run CMake to configure the build system (pointing to the parent directory where the main `CMakeLists.txt` is located):**
    ```bash
    cmake ..
    ```
5.  **Compile the examples:**
    ```bash
    make 
    # Or, if you're using a different build system generator with CMake (e.g., Ninja):
    # ninja
    # Or, on Windows with MSVC:
    # cmake --build .
    ```
6.  **Run the examples:**
    The executables will be located in the `build/examples` directory.
    The example names in the CMakeLists.txt are `SimpleExample` and `AsyncExample`.
    ```bash
    ./examples/SimpleExample
    ./examples/AsyncExample
    ```

This will compile `example.cpp` into `SimpleExample` and `example_async.cpp` into `AsyncExample`.
