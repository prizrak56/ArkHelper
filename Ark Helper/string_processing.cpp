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
    for (auto a : words) {
        //Words kill and destroy, due to possible errors in recognition
        if (a == "destroyed!"s || a == "destroyed"s || a == "killed"s || a == "killed!"s || a == "destroyecy"s || a == "destrByed!"s || a == "killgu!"s || a == "killgu"s || a == "witHilled!"s || a == "witHilled"s || a == "killed?î"s || a == "kiiledt"s || a == "kinfer"s || a == "killedy"s || a == "killclr"s || a == "W@miiiiiin"s || a == "killedt"s) {
            return true;
        }
    }
    return false;
}

void InfoMessage() noexcept{

    system("CLS");
    std::cout << "F1 - Left mouse auto clicker in app"s << std::endl;
    std::cout << "F2 - Right mouse auto clicker in app"s << std::endl;
    std::cout << "F3 - Space button auto clicker in app"s << std::endl;
    std::cout << "F4 - Open GitHub"s << std::endl;
    std::cout << "F5 - Open YouTube"s << std::endl;
    std::cout << "F6 - Log mode"s << std::endl;
    std::cout << "F7 - Farm mode"s << std::endl;
    std::cout << "F8 - Set delay"s << std::endl;


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


std::string ConvertLPCWSTRToString(LPCWSTR lpcwszStr) noexcept{
    
    int strLength = WideCharToMultiByte(CP_UTF8, 0, lpcwszStr, -1, nullptr, 0, nullptr, nullptr);

    std::string str(strLength, 0);

    WideCharToMultiByte(CP_UTF8, 0, lpcwszStr, -1, &str[0],strLength, nullptr, nullptr);

    return str;
}

// yes it's terrible
void Text_to_codes(std::vector<WPARAM>& spam_words, std::string& spam_text){

    for (auto a : spam_text) {
        if (a == 97) {
            spam_words.push_back(0x41);
        }
        // ' '
        else if (a == 32) {
            spam_words.push_back(0x20);
        }
        else if (a == 98) {
            spam_words.push_back(0x42);
        }
        else if (a == 99) {
            spam_words.push_back(0x43);
        }
        else if (a == 100) {
            spam_words.push_back(0x44);
        }
        else if (a == 101) {
            spam_words.push_back(0x45);
        }
        else if (a == 102) {
            spam_words.push_back(0x46);
        }
        else if (a == 103) {
            spam_words.push_back(0x47);
        }
        else if (a == 104) {
            spam_words.push_back(0x48);
        }
        else if (a == 105) {
            spam_words.push_back(0x49);
        }
        else if (a == 106) {
            spam_words.push_back(0x4A);
        }
        else if (a == 107) {
            spam_words.push_back(0x4B);
        }
        else if (a == 108) {
            spam_words.push_back(0x4C);
        }
        else if (a == 109) {
            spam_words.push_back(0x4D);
        }
        else if (a == 110) {
            spam_words.push_back(0x4E);
        }
        else if (a == 111) {
            spam_words.push_back(0x4F);
        }
        else if (a == 112) {
            spam_words.push_back(0x50);
        }
        else if (a == 113) {
            spam_words.push_back(0x51);
        }
        else if (a == 114) {
            spam_words.push_back(0x52);
        }
        else if (a == 115) {
            spam_words.push_back(0x53);
        }
        else if (a == 116) {
            spam_words.push_back(0x54);
        }
        else if (a == 117) {
            spam_words.push_back(0x55);
        }
        else if (a == 118) {
            spam_words.push_back(0x56);
        }
        else if (a == 119) {
            spam_words.push_back(0x57);
        }
        else if (a == 120) {
            spam_words.push_back(0x58);
        }
        else if (a == 121) {
            spam_words.push_back(0x59);
        }
        else if (a == 122) {
            spam_words.push_back(0x5A);
        }
        //-
        else if (a == 45) {
            spam_words.push_back(0x6D);
        }
        // /
        else if (a == 47) {
            spam_words.push_back(0xBF);
        }
        //.
        else if (a == 46) {
            spam_words.push_back(0xBE);
        }
        // ?
        else if (a == 63) {
            spam_words.push_back(0x3F);
        }
        // _
        else if (a == 95) {
            spam_words.push_back(0x5F);
        }
    }
}
