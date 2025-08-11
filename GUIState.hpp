#pragma once


#include <cstdint>
#include <optional>
#include <tuple>
#include <array>
#include <string>


struct GUIState {
	GUIState();
	std::optional<std::tuple<uint8_t, uint8_t>> selected;
	std::array<std::array<char, 8>, 8> shadows;
	std::array<std::array<uint64_t, 8>, 8> rotatedBeforeMs;
	bool translatedLabel;
	std::tuple<uint64_t, std::string> screamer;
	uint64_t useBoy2BeforeMs;
};