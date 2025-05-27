#ifndef CPP_HTTP_CLIENT_CLIENT_HPP
#define CPP_HTTP_CLIENT_CLIENT_HPP

#include "Request.hpp"
#include "Response.hpp"
#include "ThreadPool.hpp" // Added ThreadPool
#include <string>
#include <future>    // Added for std::future
#include <functional> // Added for std::bind or lambdas if needed directly here

namespace http {

class Client {
public:
    // Get the default thread pool instance
    // Using a function static to ensure lazy initialization and thread safety (C++11 onwards)
    static utils::ThreadPool& get_thread_pool() {
        // By default, create a pool with std::thread::hardware_concurrency() threads.
        // This will be created once and reused.
        static utils::ThreadPool default_pool(std::thread::hardware_concurrency());
        return default_pool;
    }

    // Static method to perform a synchronous GET request
    static Response get(const std::string& url) {
        Request request;
        request.method("GET");
        request.url(url);
        return request.send(); // This is the mock send
    }

    // Static method to perform an asynchronous GET request
    static std::future<Response> async_get(const std::string& url) {
        // The task to be executed in the thread pool.
        // It captures 'url' by value to ensure it's valid when the task runs.
        // It needs to return a Response object.
        auto task = [url]() -> Response {
            // This part is the same as the synchronous 'get' method's core logic.
            Request request;
            request.method("GET");
            request.url(url);
            return request.send(); // Calls the mock send
        };

        // Enqueue the task into our thread pool.
        // The ThreadPool's enqueue method should return a std::future<Response>.
        return get_thread_pool().enqueue(task);
    }

    // Static method to perform a synchronous POST request
    static Response post(const std::string& url, const std::string& body) {
        Request request;
        request.method("POST");
        request.url(url);
        request.body(body);
        return request.send(); // This is the mock send
    }

    // Static method to perform an asynchronous POST request
    static std::future<Response> async_post(const std::string& url, const std::string& body) {
        // The task to be executed in the thread pool.
        // Captures 'url' and 'body' by value.
        auto task = [url, body]() -> Response {
            Request request;
            request.method("POST");
            request.url(url);
            request.body(body);
            return request.send(); // Calls the mock send
        };
        return get_thread_pool().enqueue(task);
    }

    // Potentially other methods like put, delete could be added here later
    // static Response put(const std::string& url, const std::string& body);
    // static std::future<Response> async_put(const std::string& url, const std::string& body);
    // static Response del(const std::string& url); // 'delete' is a keyword
    // static std::future<Response> async_del(const std::string& url);
};

} // namespace http

#endif // CPP_HTTP_CLIENT_CLIENT_HPP
