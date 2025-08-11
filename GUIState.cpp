#include "GUIState.hpp"


GUIState::GUIState() {
	for (uint8_t y = 0; y < 8; y = y + 1) {
		for (uint8_t x = 0; x < 8; x = x + 1) {
			this->shadows[y][x] = ' ';
			this->rotatedBeforeMs[y][x] = 0;
		}
	}
	this->translatedLabel = false;
	this->screamer = std::make_tuple(0, "");
	this->useBoy2BeforeMs = 0;
}