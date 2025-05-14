#include <llhttp.hpp>
#include <iostream>
#include <curl/curl.h>


size_t llhttp::WriteCallBack(void *contents, size_t size, size_t nmemb, std::string *output) {
    output->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string llhttp::GET(std::string url) {
    CURL* curl = curl_easy_init();
    if (curl) {
        std::string response;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallBack);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        CURLcode res = curl_easy_perform(curl);
        if (res == CURLE_OK)
            return response;
        else
            std::cerr << "Error: " << curl_easy_strerror(res) << std::endl;

        curl_easy_cleanup(curl);
    }

    return "NULL:GET";
}

std::string llhttp::POST(std::string url, std::string body) {
    CURL* curl = curl_easy_init();
    if (curl) {
        std::string response;

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallBack);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        CURLcode res = curl_easy_perform(curl);
        if (res == CURLE_OK)
            return response;
        else
            std::cerr << "Error: " << curl_easy_strerror(res) << std::endl;

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    return "NULL:POST";
}
