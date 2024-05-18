#include <iostream>
#include "string_processing.h"

namespace processing {
    using namespace std::literals;

    std::string ConvertLPCWSTRToString(LPCWSTR lpcwsz_str) noexcept {

        int str_length = WideCharToMultiByte(CP_UTF8, 0, lpcwsz_str, -1, nullptr, 0, nullptr, nullptr);
        std::string str(str_length, 0);
        WideCharToMultiByte(CP_UTF8, 0, lpcwsz_str, -1, &str[0], str_length, nullptr, nullptr);

        return str;
    }

    utils::Point ParseCoords(std::string_view str) {
        auto x_pos = str.find_first_of(':');
        auto y_pos = str.find_first_of(',');

        std::string x = std::move(std::string(str.substr(x_pos + 2, y_pos - x_pos - 2)));
        std::string y = std::move(std::string(str.substr(y_pos + 2, str.length() - y_pos - 2)));

        return { std::stoi(x), std::stoi(y) };
    }

    std::vector<std::string> ParseName(std::string_view to_parse) {
        std::vector<std::string> result;

        auto pos = to_parse.find_first_of(':');
        std::string_view taken = to_parse.substr(pos + 2, to_parse.length() - pos - 2);
        pos = taken.find_first_of(',');

        while (pos != std::string_view::npos) {
            result.push_back(std::string(taken.substr(0, pos)));
            taken = taken.substr(pos + 2, taken.length() - pos - 2);
            pos = taken.find_first_of(',');
        }

        result.push_back(std::string(taken));
        return result;
    }

    void PrintMenuMessage1() noexcept {

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

    void PrintMenuMessage2() noexcept {

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

    bool SplitIntoWordsAndFindWord(const std::string& text) noexcept {
        std::vector<std::string> words;
        std::string word;

        for (const char ch : text) {
            if (ch == ' ' || ch == '\n') {
                if (!word.empty()) {
                    words.push_back(word);
                    word.clear();
                }
            }
            else {
                word += ch;
            }
        }

        if (!word.empty()) {
            words.push_back(word);
        }

        for (const auto& word : words) {
            // Different options of words kill and destroy, due to possible errors in recognition
            if (word == "destroyed!"s || word == "destroyed"s|| word == "killed"s || word == "killed!"s ||
                word == "destroyecy"s || word == "destrByed!"s || word == "killgu!"s || word == "killgu"s ||
                word == "witHilled!"s || word == "witHilled"s || word == "killed?î"s || word == "kiiledt"s ||
                word == "kinfer"s || word == "killedy"s || word == "killclr"s || word == "W@miiiiiin"s || word == "killedt"s) {

                return true;
            }
        }
        return false;
    }
} // processing