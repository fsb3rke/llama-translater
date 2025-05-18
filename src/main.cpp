// AUTHOR: fsb3rke 2025

// TODO: (0) = make cross platform
// TODO: (1) = make cross platform getch function
// TODO: (2) = make character limit for each line

#include <Windows.h> // TODO: (0)
#include <cstdlib>
#include <iostream>
#include <string>
#include <llhttp.hpp>
#include <sstream>
#include <conio.h> // TODO: (1)
#include <tabulate.hpp>
#include <nlohmann/json.hpp>


constexpr auto BK_LEFT = 331;
constexpr auto BK_RIGHT = 333;
constexpr auto BK_ESC = 27;
constexpr auto BK_ENTER = 13;

int bgetch(void);

int main(int argc, char *argv[]) { // main.exe port model language text
    SetConsoleOutputCP(65001); // TODO: (0)

    tabulate::Table table;
    table.add_row({"Port", "Model", "Language", "Text", "OK", "OUTPUT"});
    table.add_row({"", "", "", "", "BTN", ""});
    table[0][0].format().font_color(tabulate::Color::red);
    table[1][0].format().font_color(tabulate::Color::red);
    int x = 0;
    bool isEditing = false;
    std::string buffer;

    while (1) {
        system("cls");

        if (isEditing) {
            table[1][x].set_text(buffer + "|");
        }

        std::cout << table << std::endl;

        int ch = bgetch();

        if (!isEditing && ch == BK_ESC) break;

        if (ch == BK_ENTER) {
            if (x == 4) {
                // Buton işlevi
                std::cout << "Sending to ollama..." << std::endl;

                std::ostringstream oss;
                oss << "{\n"
                    << "    \"model\": \""   << table[1][1].get_text()   << "\",\n"
                    << "    \"prompt\": \"Translate only the quoted sentence into "
                    << table[1][2].get_text()
                    << " without answering it or adding anything else: \\\""
                    << table[1][3].get_text()
                    << "\\\"\",\n"
                    << "    \"stream\": false\n"
                    << "}";

                std::string response = llhttp::POST(("http://localhost:" + port + "/api/generate"), oss.str());

                nlohmann::json parsed = nlohmann::json::parse(response); // TODO: (2)

                table[1][5].set_text(parsed["response"]);
            } else {
                if (isEditing) {
                    table[1][x].set_text(buffer);
                    buffer.clear();
                } else {
                    buffer = std::string(table[1][x].get_text());
                }
                isEditing = !isEditing;
                continue;
            }
        }

        if (!isEditing) {
            if (ch == BK_LEFT && x > 0) {
                table[0][x].format().font_color(tabulate::Color::none);
                table[1][x].format().font_color(tabulate::Color::none);
                x--;
                table[0][x].format().font_color(tabulate::Color::red);
                table[1][x].format().font_color(tabulate::Color::red);
            } else if (ch == BK_RIGHT && x < 4) {
                table[0][x].format().font_color(tabulate::Color::none);
                table[1][x].format().font_color(tabulate::Color::none);
                x++;
                table[0][x].format().font_color(tabulate::Color::red);
                table[1][x].format().font_color(tabulate::Color::red);
            }
        } else {
            if (ch == 8 && !buffer.empty()) {
                buffer.pop_back();
            } else if (ch >= 32 && ch <= 126) {
                buffer += static_cast<char>(ch);
            }
        }
    }

    return 0;
}

int bgetch(void) {
    int ch = _getch();

    if (ch == 0 || ch == 224)
        ch = 256 + _getch();

    return ch;
}
