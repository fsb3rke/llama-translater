g++ -Wall -Werror -Wpedantic -std=c++17 -Iinclude -Llib .\src\main.cpp .\src\llhttp.cpp -lcurl -lws2_32 -lwinmm -lcrypt32 -o ./bin/main.exe
