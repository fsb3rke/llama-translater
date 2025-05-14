#ifndef LLHTTP_HPP
#define LLHTTP_HPP

#include <string>


namespace llhttp {
    std::string GET(std::string url);
    std::string POST(std::string url, std::string body);
    size_t WriteCallBack(void* contents, size_t size, size_t nmemb, std::string* output);
}

#endif
