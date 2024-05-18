#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <Windows.h>

#include "utils.h"

namespace processing {
	std::string ConvertLPCWSTRToString(LPCWSTR lpcwszStr) noexcept;
	utils::Point ParseCoords(std::string_view str);
	std::vector<std::string> ParseName(std::string_view str);
	void PrintMenuMessage1() noexcept;
	void PrintMenuMessage2() noexcept;
	bool SplitIntoWordsAndFindWord(const std::string& text) noexcept;
} // processing