#ifndef REQUEST_H
#define REQUEST_H

#include <nlohmann/json.hpp>
#include <vector>
#include <string>
#include <curl/curl.h>

class Request
{
public:
    Request();
    ~Request();

    nlohmann::json get(const std::string &url, const std::unordered_map<std::string, std::string> &headersMap = {});

private:
    CURL *curl_;
    std::string responseData_;
    struct curl_slist *headerList_ = nullptr;

    void prepareRequest(const std::string &url);
    void setHeaders(const std::unordered_map<std::string, std::string> &headersMap);
    nlohmann::json performRequest();
    static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp);
};

#endif