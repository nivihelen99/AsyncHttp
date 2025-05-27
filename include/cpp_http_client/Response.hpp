#ifndef CPP_HTTP_CLIENT_RESPONSE_HPP
#define CPP_HTTP_CLIENT_RESPONSE_HPP

#include <string>
#include <vector>
#include <map>

namespace http {

class Response {
public:
    Response(int status_code, const std::string& body, const std::map<std::string, std::string>& headers)
        : status_code_(status_code), body_(body), headers_(headers) {}

    int status_code() const {
        return status_code_;
    }

    const std::string& body() const {
        return body_;
    }

    const std::map<std::string, std::string>& headers() const {
        return headers_;
    }

private:
    int status_code_;
    std::string body_;
    std::map<std::string, std::string> headers_;
};

} // namespace http

#endif // CPP_HTTP_CLIENT_RESPONSE_HPP
