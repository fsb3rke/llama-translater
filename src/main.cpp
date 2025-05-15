#include <iostream>
#include <string>
#include <llhttp.hpp>
#include <sstream>
#include <Windows.h>

int main(int argc, char *argv[]) { // main.exe port model language text
    SetConsoleOutputCP(65001);
    if (argc < 5) {
        std::cerr << "llama-translater port model language text\n\tport\t\t=> served ollama port.\n\tmodel\t\t=> model to be used in ollama.\n\tlanguage\t=> language to be using at translation.\n\ttext" << std::endl;
        return 1;
    }

    std::string port = argv[1];
    std::string model = argv[2];
    std::string language = argv[3];
    std::string text = argv[4];

    if (text.front() == '"' && text.back() == '"') {
        text = text.substr(1, text.length() - 2);
    }

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

    std::string response = llhttp::POST(("http://localhost:" + port + "/api/generate"), oss.str());
    std::cout << response << std::endl;

    return 0;
}
