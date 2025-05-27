#include "cpp_http_client/Client.hpp"
#include <iostream>
#include <future>
#include <vector>
#include <chrono> // For std::this_thread::sleep_for
#include <string>   // For std::string

void process_response(const std::string& url_desc, std::future<http::Response>& future_response) {
    std::cout << "--- Testing: " << url_desc << " ---" << std::endl;
    std::cout << "Async request sent. Waiting for response..." << std::endl;

    // Simulate doing other work
    // std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
    // std::cout << "Done with other work for " << url_desc << std::endl;

    try {
        http::Response response = future_response.get(); // Blocks until the result is available
        std::cout << "Async Response Status (" << url_desc << "): " << response.status_code() << std::endl;
        std::cout << "Async Response Body (" << url_desc << "): " << response.body() << std::endl;
        std::cout << "Headers (" << url_desc << "):" << std::endl;
        for(const auto& header : response.headers()) {
            std::cout << "  " << header.first << ": " << header.second << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception caught for " << url_desc << ": " << e.what() << std::endl;
    }
    std::cout << "--- Test finished for: " << url_desc << " ---\n" << std::endl;
}

int main() {
    std::cout << "Starting Asynchronous HTTP Client Example..." << std::endl;

    // Test case 1: Successful response
    auto future_response_ok = http::Client::async_get("http://example.com/ok");
    // Test case 2: Error response
    auto future_response_error = http::Client::async_get("http://example.com/error");
    // Test case 3: Created response
    auto future_response_created = http::Client::async_get("https://example.com/created"); // Test HTTPS as well
    // Test case 4: Unknown URL for mock
    auto future_response_unknown = http::Client::async_get("http://nonexistent.com/data");

    std::cout << "\nAll async requests dispatched.\nNow processing responses as they become available (or blocking on each one sequentially for this demo):\n" << std::endl;

    // Process responses (future.get() will block for each one)
    process_response("http://example.com/ok", future_response_ok);
    process_response("http://example.com/error", future_response_error);
    process_response("https://example.com/created", future_response_created);
    process_response("http://nonexistent.com/data", future_response_unknown);

    std::cout << "Example finished. Thread pool will now shut down." << std::endl;
    // Note: The ThreadPool destructor will be called when Client's static pool goes out of scope
    // (effectively at program termination), which will join its threads.
    // To see the thread pool shutdown explicitly before main ends, one would need to manage
    // the pool's lifecycle more directly (e.g. global pointer, explicit shutdown call).
    // For this example, relying on static destruction order is fine.
    
    // Explicitly wait a tiny bit to ensure all background tasks complete if any output is pending
    // especially if the thread pool is destroyed very late.
    // std::this_thread::sleep_for(std::chrono::milliseconds(100));


    // To demonstrate that the thread pool is indeed shared and can be reused:
    std::cout << "\n--- Second batch of requests to demonstrate pool reuse ---" << std::endl;
    auto future_response_ok_2 = http::Client::async_get("http://example.com/ok");
    process_response("http://example.com/ok (batch 2)", future_response_ok_2);

    std::cout << "\n========= Async POST Examples =========\n" << std::endl;

    // Async POST example 1
    std::string async_post_body_1 = R"({"data":"async_payload_1"})";
    auto future_async_post_1 = http::Client::async_post("http://example.com/submit_async", async_post_body_1);
    process_response("http://example.com/submit_async (POST 1)", future_async_post_1);

    // Async POST example 2 (different body to same URL)
    std::string async_post_body_2 = R"({"item_id":789,"status":"pending_async"})";
    auto future_async_post_2 = http::Client::async_post("http://example.com/submit_async", async_post_body_2);
    process_response("http://example.com/submit_async (POST 2)", future_async_post_2);
    
    // Async POST example 3 (to create_resource URL)
    std::string async_post_body_3 = R"({"product":"async_widget","quantity":50})";
    auto future_async_post_3 = http::Client::async_post("https://example.com/create_resource", async_post_body_3); // Test HTTPS POST
    process_response("https://example.com/create_resource (POST 3)", future_async_post_3);

    std::cout << "\n========= Async Custom Headers Examples =========\n" << std::endl;

    // Async GET with custom headers
    auto future_custom_get = http::Client::get_thread_pool().enqueue([]{
        http::Request req;
        req.url("http://example.com/echo_headers_async")
           .method("GET")
           .header("X-Async-Header", "Value-Async-GET")
           .header("X-Mirror-Me", "AsyncGETEcho");
        return req.send();
    });
    process_response("http://example.com/echo_headers_async (GET with Custom Headers)", future_custom_get);

    // Async POST with custom headers
    std::string async_post_body_custom = R"({"item_id":101,"type":"async_custom_post"})";
    auto future_custom_post = http::Client::get_thread_pool().enqueue([async_post_body_custom]{ // Capture body
        http::Request req;
        req.url("https://example.com/echo_headers_async") // Test HTTPS
           .method("POST")
           .header("X-Async-Header", "Value-Async-POST")
           .header("X-Custom-Auth", "Bearer async_token_post")
           .header("X-Mirror-Me", "AsyncPOSTEcho")
           .body(async_post_body_custom);
        return req.send();
    });
    process_response("https://example.com/echo_headers_async (POST with Custom Headers)", future_custom_post);

    std::cout << "\n========= Timeout Examples (Asynchronous) =========\n" << std::endl;
    // MOCK_PROCESSING_TIME is 100ms as defined in Request.hpp

    // Async timeout example - Request timeout is shorter than mock processing (expect 408 from mock)
    std::cout << "Testing GET to: http://example.com/timeout_async with short request timeout" << std::endl;
    auto future_async_req_timeout_short = http::Client::get_thread_pool().enqueue([]{
        http::Request req;
        req.url("http://example.com/timeout_async") // Mocked to "work" for MOCK_PROCESSING_TIME (100ms)
           .method("GET")
           .timeout(std::chrono::milliseconds(50)); // Request timeout is 50ms
        return req.send(); // Mock send will see timeout is 50ms, "work" is 100ms, returns 408 Response
    });
    process_response("http://example.com/timeout_async (Request Timeout Short)", future_async_req_timeout_short);


    // Async timeout example - Request timeout is longer than mock processing (expect 200 from mock)
    std::cout << "Testing GET to: http://example.com/timeout_async with long request timeout" << std::endl;
    auto future_async_req_timeout_long = http::Client::get_thread_pool().enqueue([]{
        http::Request req;
        req.url("http://example.com/timeout_async") // Mocked to "work" for MOCK_PROCESSING_TIME (100ms)
           .method("GET")
           .timeout(std::chrono::milliseconds(200)); // Request timeout is 200ms
        return req.send(); // Mock send will see timeout is 200ms, "work" is 100ms, returns 200 Response
    });
    process_response("http://example.com/timeout_async (Request Timeout Long)", future_async_req_timeout_long);


    // Async timeout example - Using future.wait_for() for client-side timeout
    std::cout << "\nTesting GET to: http://example.com/timeout_async_waitfor with future.wait_for()" << std::endl;
    auto future_async_client_waitfor = http::Client::get_thread_pool().enqueue([]{
        http::Request req;
        // This URL also uses MOCK_PROCESSING_TIME (100ms) in send()
        req.url("http://example.com/timeout_async_waitfor") 
           .method("GET")
           .timeout(std::chrono::milliseconds(200)); // Request's own timeout (longer than mock processing)
                                                    // so mock send() will return 200 OK.
        //std::this_thread::sleep_for(std::chrono::milliseconds(5)); // Small delay before send if needed
        return req.send(); 
    });
    
    std::cout << "Main thread: Waiting on future_async_client_waitfor with 50ms client-side timeout..." << std::endl;
    std::future_status status = future_async_client_waitfor.wait_for(std::chrono::milliseconds(50)); // Client waits for only 50ms

    if (status == std::future_status::timeout) {
        std::cout << "Async Client Timeout (wait_for): Timed out waiting for future! (Expected for 50ms wait on 100ms task)" << std::endl;
        // Note: The task in the thread pool is still running to completion (or until its own timeout).
        // We are just not waiting for its result anymore in this specific 'wait_for' call.
        // To get the actual result later (if it completed):
        try {
             // This will block until the task actually finishes, or throw if it had an exception.
             // If the task completed successfully after the wait_for timeout, this .get() will retrieve the value.
            http::Response resp_after_waitfor_timeout = future_async_client_waitfor.get(); 
            std::cout << "Async Client Timeout (wait_for): Future completed after client timeout with status: "
                      << resp_after_waitfor_timeout.status_code() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Async Client Timeout (wait_for): Exception getting future result after client timeout: " << e.what() << std::endl;
        }

    } else if (status == std::future_status::ready) {
        std::cout << "Async Client Timeout (wait_for): Future was ready within 50ms." << std::endl;
        try {
            http::Response resp = future_async_client_waitfor.get();
            std::cout << "Async Client Timeout (wait_for): Got response with status " << resp.status_code() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Async Client Timeout (wait_for): Exception getting future result: " << e.what() << std::endl;
        }
    } else if (status == std::future_status::deferred) {
         std::cout << "Async Client Timeout (wait_for): Future was deferred." << std::endl;
    }


    std::cout << "\nAll async GET, POST, custom header, and timeout examples completed." << std::endl;

    return 0;
}
