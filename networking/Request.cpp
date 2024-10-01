#include "Request.h"

Request::Request()
{
    curl_ = curl_easy_init();
    if (!curl_)
    {
        throw std::runtime_error("(Request) Failed to initialize curl");
    }
    curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &responseData_);
}

Request::~Request()
{
    if (curl_)
    {
        curl_easy_cleanup(curl_);
    }
    if (headerList_)
    {
        curl_slist_free_all(headerList_);
    }
}

nlohmann::json Request::get(const std::string &url, const std::unordered_map<std::string, std::string> &headersMap)
{
    prepareRequest(url);
    setHeaders(headersMap);
    return performRequest();
}

void Request::prepareRequest(const std::string &url)
{
    responseData_.clear();
    curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());
}

void Request::setHeaders(const std::unordered_map<std::string, std::string> &headersMap)
{
    if (headerList_)
    {
        curl_slist_free_all(headerList_);
        headerList_ = nullptr;
    }
    for (const auto &[headerName, headerValue] : headersMap)
    {
        std::string header = headerName + ": " + headerValue;
        headerList_ = curl_slist_append(headerList_, header.c_str());
    }
    curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headerList_);
}

nlohmann::json Request::performRequest()
{
    CURLcode res = curl_easy_perform(curl_);
    if (res != CURLE_OK)
    {
        throw std::runtime_error(curl_easy_strerror(res));
    }
    return nlohmann::json::parse(responseData_);
}

size_t Request::writeCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}