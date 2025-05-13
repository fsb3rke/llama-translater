#include <iostream>
#include <string>
#include <curl/curl.h>
#include <sstream>

size_t WriteCallBack(void* contents, size_t size, size_t nmemb, std::string* output) {
    output->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main(int argc, char *argv[]) { // main.exe port model language text
    if (argc < 5) {
        std::cerr << "Please provide text as argument." << std::endl;
        return 1;
    }

    std::string port = argv[1];
    std::string model = argv[2];
    std::string language = argv[3];
    std::string text = argv[4];

    if (text.front() == '"' && text.back() == '"') {
        text = text.substr(1, text.length() - 2);
    }

    CURL* curl = curl_easy_init();
    if (curl) {
        std::string response;
        std::ostringstream oss;
        oss << "{\n"
            << "    \"model\": \""   << model   << "\",\n"
            << "    \"prompt\": \"Translate only the quoted sentence into "
            << language
            << " without answering it or adding anything else: \\\""
            << text
            << "\\\"\",\n"
            << "    \"stream\": false\n"
            << "}";

        std::string jsonData = oss.str();

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, ("http://localhost:" + port + "/api/generate").c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallBack);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        CURLcode res = curl_easy_perform(curl);
        if (res == CURLE_OK)
            std::cout << response << std::endl;
        else
            std::cerr << "Error: " << curl_easy_strerror(res) << std::endl;

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    return 0;
}
