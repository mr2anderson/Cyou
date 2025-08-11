#include "UCIEngineLevel.hpp"
#include <filesystem>
#include <fstream>


UCIEngineLevel::UCIEngineLevel() {
	if (std::filesystem::exists("appdata/level.cfg")) {
		std::ifstream file("appdata/level.cfg");
		std::string buff;
		std::getline(file, buff);
		file.close();
		this->value = std::stoi(buff);
	}
	else {
		this->value = 0;
	}
}
void UCIEngineLevel::increase() {
	this->value += 1;
	std::ofstream file("appdata/level.cfg");
	file << std::to_string(this->value);
	file.close();
}
uint32_t UCIEngineLevel::getValue() const {
	return this->value;
}