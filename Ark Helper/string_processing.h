#pragma once
#include <vector>
#include <string>
#include <Windows.h>

bool SplitIntoWordsAndFindWord(const std::string& text) noexcept;

void InfoMessage() noexcept;

std::string ConvertLPCWSTRToString(LPCWSTR lpcwszStr) noexcept;

void Text_to_codes(std::vector<WPARAM>& spam_words, std::string& spam_text);