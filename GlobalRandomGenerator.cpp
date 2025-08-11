#include "GlobalRandomGenerator.hpp"


GlobalRandomGenerator::GlobalRandomGenerator() {
	std::random_device rd;
	this->mt = std::mt19937(rd());
}
GlobalRandomGenerator::~GlobalRandomGenerator() = default;
uint32_t GlobalRandomGenerator::gen() {
	return this->mt();
}
bool GlobalRandomGenerator::gen(uint32_t percent) {
	uint32_t val = this->gen() % 100;
	return (val < percent);
}