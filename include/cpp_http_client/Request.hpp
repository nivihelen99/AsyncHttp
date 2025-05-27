#ifndef CPP_HTTP_CLIENT_REQUEST_HPP
#define CPP_HTTP_CLIENT_REQUEST_HPP

#include "Response.hpp"
#include <string>
#include <stdexcept> // For std::runtime_error
#include <map>       // For headers
#include <sstream>   // For string manipulation in mock
#include <chrono>    // For std::chrono::milliseconds and sleep_for
#include <thread>    // For std::this_thread::sleep_for

namespace http {

// Define a standard simulated processing time for mock server
const std::chrono::milliseconds MOCK_PROCESSING_TIME(100);
const int HTTP_STATUS_REQUEST_TIMEOUT = 408;
const int HTTP_STATUS_OK = 200;
const int HTTP_STATUS_CREATED = 201;
const int HTTP_STATUS_NOT_FOUND = 404;
const int HTTP_STATUS_INTERNAL_SERVER_ERROR = 500;


class Request {
public:
    Request() : method_("GET"), timeout_(std::chrono::milliseconds(30000)) {} // Default 30s timeout

    Request& url(const std::string& url_str) {
        url_ = url_str;
        if (url_.rfind("http://", 0) == 0) {
            std::string temp = url_.substr(7);
            size_t path_start = temp.find('/');
            host_ = (path_start == std::string::npos) ? temp : temp.substr(0, path_start);
            path_ = (path_start == std::string::npos) ? "/" : temp.substr(path_start);
        } else if (url_.rfind("https://", 0) == 0) {
            std::string temp = url_.substr(8);
            size_t path_start = temp.find('/');
            host_ = (path_start == std::string::npos) ? temp : temp.substr(0, path_start);
            path_ = (path_start == std::string::npos) ? "/" : temp.substr(path_start);
        } else {
            host_ = "unknown";
            path_ = "/";
        }
        return *this;
    }

    Request& method(const std::string& method_str) {
        method_ = method_str;
        return *this;
    }

    Request& body(const std::string& req_body) {
        body_ = req_body;
        return *this;
    }

    Request& header(const std::string& key, const std::string& value) {
        headers_[key] = value;
        return *this;
    }

    Request& timeout(std::chrono::milliseconds duration) {
        timeout_ = duration;
        return *this;
    }
    
    std::chrono::milliseconds get_timeout() const { // Getter for timeout
        return timeout_;
    }


    // Mocked send() method
    Response send() {
        std::map<std::string, std::string> response_headers = {
            {"Connection", "close"},
            {"Content-Type", "application/json"}
        };

        // --- Timeout Simulation Logic ---
        if (url_ == "http://example.com/timeout_sync" || url_ == "https://example.com/timeout_sync" ||
            url_ == "http://example.com/timeout_async" || url_ == "https://example.com/timeout_async" ||
            url_ == "http://example.com/timeout_async_waitfor" || url_ == "https://example.com/timeout_async_waitfor") {
            
            // Simulate work
            std::this_thread::sleep_for(MOCK_PROCESSING_TIME);

            if (timeout_ > std::chrono::milliseconds(0) && MOCK_PROCESSING_TIME > timeout_) {
                response_headers["X-Mock-Reason"] = "Request timeout_ exceeded MOCK_PROCESSING_TIME";
                return Response(HTTP_STATUS_REQUEST_TIMEOUT, "{\"error\":\"Request timed out by mock server\"}", response_headers);
            }
            // If timeout was not exceeded, or no timeout was set (timeout_ <= 0 means infinite for this mock)
            return Response(HTTP_STATUS_OK, "{\"message\":\"Processed within timeout\"}", response_headers);
        }

        // --- Specific Mock Logic for Headers ---
        if (url_ == "http://example.com/echo_headers" || url_ == "https://example.com/echo_headers" ||
            url_ == "http://example.com/echo_headers_async" || url_ == "https://example.com/echo_headers_async") {
            // (logic from previous step, slightly simplified for brevity here, assume it's correct)
            std::stringstream ss_body;
            ss_body << "{\"message\": \"Request headers received\", \"method\": \"" << method_ << "\", \"received_headers\": {";
            bool first = true;
            for (const auto& pair : headers_) { if (!first) ss_body << ","; ss_body << "\"" << pair.first << "\": \"" << pair.second << "\""; first = false; }
            ss_body << "}}";
            if (headers_.count("X-Mirror-Me")) response_headers["X-Mirrored-Back"] = headers_.at("X-Mirror-Me");
            return Response(HTTP_STATUS_OK, ss_body.str(), response_headers);
        }

        // --- Existing Mock Logic (GET/POST) ---
        if (method_ == "GET") {
            if (url_ == "http://example.com/ok" || url_ == "https://example.com/ok") {
                return Response(HTTP_STATUS_OK, "{\"message\": \"GET success\"}", response_headers);
            } // ... other GET cases
        } else if (method_ == "POST") {
            if (url_ == "http://example.com/submit" || url_ == "https://example.com/submit") {
                 std::string resp_body = "{\"message\": \"POST to /submit successful\", \"received_body\": " + (!body_.empty() ? "\"" + body_ + "\"" : "\"empty\"") + "}";
                return Response(HTTP_STATUS_OK, resp_body, response_headers);
            } // ... other POST cases
        }

        // Fallback
        std::map<std::string, std::string> fallback_headers = {{"Content-Type", "text/plain"}};
        return Response(HTTP_STATUS_INTERNAL_SERVER_ERROR, "Mock Error: Unhandled URL/method. URL: " + url_, fallback_headers);
    }

private:
    std::string url_;
    std::string method_;
    std::string body_;
    std::map<std::string, std::string> headers_;
    std::chrono::milliseconds timeout_; // Timeout for the request
    std::string host_;
    std::string path_;
};

} // namespace http

#endif // CPP_HTTP_CLIENT_REQUEST_HPP
