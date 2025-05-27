#include "cpp_http_client/Client.hpp" // Adjust path if necessary based on include setup
#include <iostream>

int main() {
    std::cout << "Testing with URL: http://example.com/ok" << std::endl;
    try {
        http::Response response_ok = http::Client::get("http://example.com/ok");
        std::cout << "Status Code: " << response_ok.status_code() << std::endl;
        std::cout << "Body: " << response_ok.body() << std::endl;
        std::cout << "Headers:" << std::endl;
        for(const auto& header : response_ok.headers()) {
            std::cout << "  " << header.first << ": " << header.second << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    std::cout << "\n-----------------------------------\n" << std::endl;

    std::cout << "Testing with URL: https://example.com/ok (HTTPS test)" << std::endl;
    try {
        http::Response response_ok_https = http::Client::get("https://example.com/ok");
        std::cout << "Status Code: " << response_ok_https.status_code() << std::endl;
        std::cout << "Body: " << response_ok_https.body() << std::endl;
        std::cout << "Headers:" << std::endl;
        for(const auto& header : response_ok_https.headers()) {
            std::cout << "  " << header.first << ": " << header.second << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    std::cout << "\n-----------------------------------\n" << std::endl;

    std::cout << "Testing with URL: http://example.com/error" << std::endl;
    try {
        http::Response response_error = http::Client::get("http://example.com/error");
        std::cout << "Status Code: " << response_error.status_code() << std::endl;
        std::cout << "Body: " << response_error.body() << std::endl;
        std::cout << "Headers:" << std::endl;
        for(const auto& header : response_error.headers()) {
            std::cout << "  " << header.first << ": " << header.second << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    std::cout << "\n-----------------------------------\n" << std::endl;

    std::cout << "Testing with URL: http://example.com/created" << std::endl;
    try {
        http::Response response_created = http::Client::get("http://example.com/created");
        std::cout << "Status Code: " << response_created.status_code() << std::endl;
        std::cout << "Body: " << response_created.body() << std::endl;
        std::cout << "Headers:" << std::endl;
        for(const auto& header : response_created.headers()) {
            std::cout << "  " << header.first << ": " << header.second << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    
    std::cout << "\n-----------------------------------\n" << std::endl;

    std::cout << "Testing with URL: http://unknown.com/resource" << std::endl;
    try {
        http::Response response_unknown = http::Client::get("http://unknown.com/resource");
        std::cout << "Status Code: " << response_unknown.status_code() << std::endl;
        std::cout << "Body: " << response_unknown.body() << std::endl;
        std::cout << "Headers:" << std::endl;
        for(const auto& header : response_unknown.headers()) {
            std::cout << "  " << header.first << ": " << header.second << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    std::cout << "\n========= POST Examples =========\n" << std::endl;

    // Synchronous POST example 1
    std::cout << "Testing POST to: http://example.com/submit" << std::endl;
    std::string post_body_1 = R"({"name":"cpp-http-client","version":"0.1"})";
    try {
        http::Response post_response_1 = http::Client::post("http://example.com/submit", post_body_1);
        std::cout << "POST Response Status: " << post_response_1.status_code() << std::endl;
        std::cout << "POST Response Body: " << post_response_1.body() << std::endl;
        std::cout << "POST Response Headers:" << std::endl;
        for(const auto& header : post_response_1.headers()) {
            std::cout << "  " << header.first << ": " << header.second << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "POST Exception: " << e.what() << std::endl;
    }

    std::cout << "\n-----------------------------------\n" << std::endl;

    // Synchronous POST example 2 (create resource)
    std::cout << "Testing POST to: http://example.com/create_resource" << std::endl;
    std::string post_body_2 = R"({"item":"new_gadget","type":"electronics"})";
    try {
        http::Response post_response_2 = http::Client::post("http://example.com/create_resource", post_body_2);
        std::cout << "POST Create Status: " << post_response_2.status_code() << std::endl;
        std::cout << "POST Create Body: " << post_response_2.body() << std::endl;
        std::cout << "POST Create Headers:" << std::endl;
        for(const auto& header : post_response_2.headers()) {
            std::cout << "  " << header.first << ": " << header.second << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "POST Create Exception: " << e.what() << std::endl;
    }
    
    std::cout << "\n-----------------------------------\n" << std::endl;

    // Synchronous POST example 3 (empty body)
    std::cout << "Testing POST to: http://example.com/submit (with empty body)" << std::endl;
    std::string post_body_3 = "";
    try {
        http::Response post_response_3 = http::Client::post("http://example.com/submit", post_body_3);
        std::cout << "POST Response Status (empty body): " << post_response_3.status_code() << std::endl;
        std::cout << "POST Response Body (empty body): " << post_response_3.body() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "POST Exception (empty body): " << e.what() << std::endl;
    }

    std::cout << "\n========= Custom Headers Examples (Synchronous) =========\n" << std::endl;

    // Custom Headers GET example
    std::cout << "Testing GET to: http://example.com/echo_headers with custom headers" << std::endl;
    try {
        http::Request custom_get_req;
        custom_get_req.url("http://example.com/echo_headers")
                      .method("GET")
                      .header("X-Custom-Header", "my_value_get")
                      .header("Authorization", "Bearer test_token_get")
                      .header("X-Mirror-Me", "HelloGET");
        
        http::Response ch_get_response = custom_get_req.send();
        std::cout << "Custom Header GET Rsp Status: " << ch_get_response.status_code() << std::endl;
        std::cout << "Custom Header GET Rsp Body (should contain headers): " << ch_get_response.body() << std::endl;
        std::cout << "Custom Header GET Rsp Headers:" << std::endl;
        for(const auto& pair : ch_get_response.headers()) {
            std::cout << "  " << pair.first << ": " << pair.second << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Custom Header GET Exception: " << e.what() << std::endl;
    }

    std::cout << "\n-----------------------------------\n" << std::endl;

    // Custom Headers POST example
    std::cout << "Testing POST to: https://example.com/echo_headers with custom headers" << std::endl;
    try {
        http::Request custom_post_req;
        custom_post_req.url("https://example.com/echo_headers") // Test HTTPS
                       .method("POST")
                       .header("X-Custom-Header", "my_value_post")
                       .header("Content-Type", "application/json-custom") // Override default mock content type
                       .header("X-Mirror-Me", "HelloPOST")
                       .body(R"({"custom_data":"example_payload"})");
        
        http::Response ch_post_response = custom_post_req.send();
        std::cout << "Custom Header POST Rsp Status: " << ch_post_response.status_code() << std::endl;
        std::cout << "Custom Header POST Rsp Body (should contain headers and body): " << ch_post_response.body() << std::endl;
        std::cout << "Custom Header POST Rsp Headers:" << std::endl;
        for(const auto& pair : ch_post_response.headers()) {
            std::cout << "  " << pair.first << ": " << pair.second << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Custom Header POST Exception: " << e.what() << std::endl;
    }

    std::cout << "\n========= Timeout Examples (Synchronous) =========\n" << std::endl;
    // MOCK_PROCESSING_TIME is 100ms as defined in Request.hpp

    // Synchronous timeout example - Short timeout (expect 408)
    std::cout << "Testing GET to: http://example.com/timeout_sync with short timeout" << std::endl;
    try {
        http::Request req_timeout_short;
        req_timeout_short.url("http://example.com/timeout_sync")
                         .method("GET")
                         .timeout(std::chrono::milliseconds(50)); // Timeout < MOCK_PROCESSING_TIME
        
        http::Response resp_short = req_timeout_short.send();
        std::cout << "Sync Timeout Short Status: " << resp_short.status_code() << std::endl; // Expect 408
        std::cout << "Sync Timeout Short Body: " << resp_short.body() << std::endl;
        std::cout << "Sync Timeout Short Headers:" << std::endl;
        for(const auto& pair : resp_short.headers()) {
            std::cout << "  " << pair.first << ": " << pair.second << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Sync Timeout Short Exception: " << e.what() << std::endl;
    }

    std::cout << "\n-----------------------------------\n" << std::endl;

    // Synchronous timeout example - Long timeout (expect 200)
    std::cout << "Testing GET to: http://example.com/timeout_sync with long timeout" << std::endl;
    try {
        http::Request req_timeout_long;
        req_timeout_long.url("http://example.com/timeout_sync")
                        .method("GET")
                        .timeout(std::chrono::milliseconds(200)); // Timeout > MOCK_PROCESSING_TIME
        
        http::Response resp_long = req_timeout_long.send();
        std::cout << "Sync Timeout Long Status: " << resp_long.status_code() << std::endl; // Expect 200
        std::cout << "Sync Timeout Long Body: " << resp_long.body() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Sync Timeout Long Exception: " << e.what() << std::endl;
    }
    
    std::cout << "\n-----------------------------------\n" << std::endl;

    // Synchronous timeout example - Zero timeout (expect 200, as 0 means infinite for mock)
    std::cout << "Testing GET to: http://example.com/timeout_sync with zero timeout (infinite)" << std::endl;
    try {
        http::Request req_timeout_zero;
        req_timeout_zero.url("http://example.com/timeout_sync")
                        .method("GET")
                        .timeout(std::chrono::milliseconds(0)); // Zero timeout
        
        http::Response resp_zero = req_timeout_zero.send();
        std::cout << "Sync Timeout Zero Status: " << resp_zero.status_code() << std::endl; // Expect 200
        std::cout << "Sync Timeout Zero Body: " << resp_zero.body() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Sync Timeout Zero Exception: " << e.what() << std::endl;
    }

    return 0;
}
