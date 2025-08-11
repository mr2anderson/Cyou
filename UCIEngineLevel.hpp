#pragma once


#include <cstdint>


class UCIEngineLevel {
public:
	UCIEngineLevel();
	void increase();
	uint32_t getValue() const;
private:
	uint32_t value;
};