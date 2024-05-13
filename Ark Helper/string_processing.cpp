#include "string_processing.h"
#include <iostream>

using namespace std::literals;

bool SplitIntoWordsAndFindWord(const std::string& text) noexcept
{
    std::vector<std::string> words;
    std::string word;
    for (const char c : text) {
        if (c == ' ' || c == '\n') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        }
        else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }
    for (const auto& a : words) {
        //Words kill and destroy, due to possible errors in recognition
        if (a == "destroyed!"s || a == "destroyed"s || a == "killed"s || a == "killed!"s || a == "destroyecy"s || a == "destrByed!"s || a == "killgu!"s || a == "killgu"s || a == "witHilled!"s || a == "witHilled"s || a == "killed?î"s || a == "kiiledt"s || a == "kinfer"s || a == "killedy"s || a == "killclr"s || a == "W@miiiiiin"s || a == "killedt"s) {
            return true;
        }
    }
    return false;
}

void MenuMessage() noexcept{

    system("CLS");
    std::cout << "F1 - Left mouse auto clicker in app"s << std::endl;
    std::cout << "F2 - Right mouse auto clicker in app"s << std::endl;
    std::cout << "F3 - Space button auto clicker in app"s << std::endl;
    std::cout << "F4 - Open GitHub"s << std::endl;
    std::cout << "F5 - Open YouTube"s << std::endl;
    std::cout << "F6 - Log mode"s << std::endl;
    std::cout << "F7 - Farm mode"s << std::endl;
    std::cout << "F8 - Set delay"s << std::endl;
    std::cout << "F9 - Spam mode"s << std::endl;

    std::cout << "\nNext page right click + F2"s << std::endl;
    std::cout << "Changing modes settings is on the second page"s << std::endl;

    std::cout << "\ndiscord: prizrak_p\n"s << std::endl;
    std::cout << "GitHub https://github.com/prizrak56\n\n"s << std::endl;

    std::cout << " \n";
    std::cout << "   ____    ____    ___   _____  ____       _      _  __          ____  \n"s;
    std::cout << "  |  _ \\  |  _ \\  |_ _| |__  / |  _ \\     / \\    | |/ /         |  _ \\ \n"s;
    std::cout << "  | |_) | | |_) |  | |    / /  | |_) |   / _ \\   | ' /          | |_) |\n"s;
    std::cout << "  |  __/  |  _ <   | |   / /_  |  _ <   / ___ \\  | . \\          |  __/ \n"s;
    std::cout << "  |_|     |_| \\_\\ |___| /____| |_| \\_\\ /_/   \\_\\ |_|\\_\\  _____  |_|    \n"s;
    std::cout << "                                                        |_____|        \n"s;

}

void MenuMessage2() noexcept{

    system("CLS");
    std::cout << "F1 - Drop mode"s << std::endl;
    std::cout << "F2 - Edit SpamMode settings"s << std::endl;
    std::cout << "F3 - Edit DropMode settings"s << std::endl;
    std::cout << "F4 - Edit Webhook"s << std::endl;
    std::cout << "F5 - Edit FarmCoord"s << std::endl;


    std::cout << "\nPrevious page right click + F1"s << std::endl;


    std::cout << "\ndiscord: prizrak_p\n"s << std::endl;
    std::cout << "GitHub https://github.com/prizrak56\n\n"s << std::endl;

    std::cout << " \n";
    std::cout << "   ____    ____    ___   _____  ____       _      _  __          ____  \n"s;
    std::cout << "  |  _ \\  |  _ \\  |_ _| |__  / |  _ \\     / \\    | |/ /         |  _ \\ \n"s;
    std::cout << "  | |_) | | |_) |  | |    / /  | |_) |   / _ \\   | ' /          | |_) |\n"s;
    std::cout << "  |  __/  |  _ <   | |   / /_  |  _ <   / ___ \\  | . \\          |  __/ \n"s;
    std::cout << "  |_|     |_| \\_\\ |___| /____| |_| \\_\\ /_/   \\_\\ |_|\\_\\  _____  |_|    \n"s;
    std::cout << "                                                        |_____|        \n"s;
}


std::string ConvertLPCWSTRToString(LPCWSTR lpcwsz_str) noexcept{
    
    int str_length = WideCharToMultiByte(CP_UTF8, 0, lpcwsz_str, -1, nullptr, 0, nullptr, nullptr);

    std::string str(str_length, 0);

    WideCharToMultiByte(CP_UTF8, 0, lpcwsz_str, -1, &str[0],str_length, nullptr, nullptr);

    return str;
}

std::pair<int, int> ParseCoords(std::string& str) {

    int x = 0;
    int y = 0;
    auto pos = str.find_first_of(':');
    str = str.substr(pos + 1, str.length());

    pos = str.find_first_of(',');

    x = std::stoi(str.substr(0, pos));
    y = std::stoi(str.substr(pos + 1, str.length()));
    return std::make_pair(x,y);
}

std::vector<std::string> ParseName(std::string& str) {
    std::vector<std::string> result;
    auto pos = str.find_first_of(':');
    str = str.substr(pos + 2, str.length());
    pos = str.find_first_of(',');
    while (pos != std::string::npos) {
        result.push_back(str.substr(0, pos));
        str = str.substr(pos + 2, str.length());
        pos = str.find_first_of(',');
    }
    result.push_back(str);
    return result;
}