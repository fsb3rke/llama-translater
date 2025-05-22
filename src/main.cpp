//
// llama-translater v1.0.0
// Windows only translater application with using ollama
//
// Copyright (c) 2025-present @fsb3rke (Berke A.)
//
// This application is released under the GGPLv3 License.
// https://github.com/fsb3rke/llama-translater/blob/main/LICENSE
//

#ifdef _WIN32
#include <Windows.h>
#endif

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <cstdio>
#endif

#include <cstdlib>
#include <iostream>
#include <string>
#include <llhttp.hpp>
#include <sstream>
#include <tabulate.hpp>
#include <nlohmann/json.hpp>

#ifdef _WIN32
size_t get_terminal_width() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    return 80;
}
#else
#include <sys/ioctl.h>
#include <unistd.h>
size_t get_terminal_width() {
    struct winsize w{};
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        return w.ws_col;
    }
    return 80;
}
#endif

constexpr auto BK_LEFT = 331;
constexpr auto BK_RIGHT = 333;
constexpr auto BK_ESC = 27;

#ifdef _WIN32
constexpr auto BK_ENTER = 13;
#else
constexpr auto BK_ENTER = 10;
#endif

int bgetch(void);
void crossClear(void);

int main() {
#ifdef _WIN32
	SetConsoleOutputCP(65001);
#endif
    tabulate::Table table;
    table.add_row({"Port", "Model", "Language", "Text", "OK", "OUTPUT"});
    table.add_row({"", "", "", "", "BTN", ""});
    table.format().width(get_terminal_width() / 10);

    table[0][0].format().font_color(tabulate::Color::red);
    table[1][0].format().font_color(tabulate::Color::red);
    int x = 0;
    bool isEditing = false;
    std::string buffer;

    while (1) {
        crossClear();

        if (isEditing) {
            table[1][x].set_text(buffer + "|");
        }

        for (size_t row = 0; row < table.size(); ++row) {
            for (size_t col = 0; col < table[row].size(); ++col) {
                table[row][col].format().width(get_terminal_width() / 10);
            }
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

                std::string response = llhttp::POST(("http://localhost:" + table[1][0].get_text() + "/api/generate"), oss.str());

                nlohmann::json parsed = nlohmann::json::parse(response);

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

void crossClear(void) {
	#ifdef _WIN32
		system("cls");
	#else
		system("clear");
	#endif
}

int bgetch(void) {
#ifdef _WIN32
    int ch = _getch();
    if (ch == 0 || ch == 224)
        ch = 256 + _getch();
    return ch;
#else
    struct termios oldt, newt;
    int ch;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    ch = getchar();

    if (ch == 27) {
        if (getchar() == '[') {
            char asd = getchar();
            switch (asd) {
                case 'A': ch = 1;
                case 'B': ch = 1;
                case 'D': ch = BK_LEFT;  break;
                case 'C': ch = BK_RIGHT; break;
                default: break;
            }
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
#endif
}
