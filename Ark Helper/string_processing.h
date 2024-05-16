#pragma once
#include <vector>
#include <string>
#include <Windows.h>
#include "utils.h"

bool SplitIntoWordsAndFindWord(const std::string& text) noexcept;

void MenuMessage() noexcept;

void MenuMessage2() noexcept;

std::string ConvertLPCWSTRToString(LPCWSTR lpcwszStr) noexcept;

Point ParseCoords(std::string str);

std::vector<std::string> ParseName(std::string& str);