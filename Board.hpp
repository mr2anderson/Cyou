#pragma once


#include <string>
#include <vector>
#include <array>
#include <cstdint>


#pragma once


class Board {
public:
	Board();
	Board(const std::string& fen);
	std::string getFen() const;
	void setFen(const std::string& fen);
	std::array<std::array<char, 8>, 8> getArray() const;
private:
	std::string fen;
};