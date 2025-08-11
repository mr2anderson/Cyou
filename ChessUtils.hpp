#pragma once


#include <cstdint>
#include <string>
#include <tuple>
#include <array>
#include <sstream>
#include <cctype>


namespace ChessUtils {
	static std::string moveToSrc(const std::string& move) {
		return move.substr(0, 2);
	}
	static std::string moveToDst(const std::string& move) {
		return move.substr(2, 2);
	}
	static uint8_t cellToX(const std::string& cell) {
		return cell[0] - 'a';
	}
	static uint8_t cellToY(const std::string& cell) {
		return cell[1] - '1';
	}
	static std::string xyToCell(uint8_t x, uint8_t y) {
		std::string res;
		res.resize(2);
		res[0] = 'a' + x;
		res[1] = '1' + y;
		return res;
	}
	static std::string srcDstToMove(const std::string& src, const std::string& dst) {
		return src + dst;
	}
	static std::array<std::array<char, 8>, 8> getArray(const std::string &fen) {
		std::stringstream ss(fen);
		std::string shortFen;
		ss >> shortFen;
		std::array<std::array<char, 8>, 8> array;
		for (uint8_t y = 0; y < 8; y = y + 1) {
			for (uint8_t x = 0; x < 8; x = x + 1) {
				array[y][x] = ' ';
			}
		}
		uint8_t x = 0;
		uint8_t y = 7;
		for (auto buff : shortFen) {
			if (buff == '/') {
				x = 0;
				y = y - 1;
			}
			else if (std::isdigit(buff)) {
				x = x + buff - '0';
			}
			else {
				array[y][x] = buff;
				x = x + 1;
			}
		}
		return array;
	}
}