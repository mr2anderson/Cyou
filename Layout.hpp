#pragma once


#include <cstdint>


class Layout {
public:
	Layout(uint32_t windowW, uint32_t windowH);

	uint32_t getWindowW() const;
	uint32_t getWindowH() const;

	uint32_t getCellSize() const;
	uint32_t getBoardSize() const;
	uint32_t getBoardX() const;
	uint32_t getBoardY() const;
	uint32_t getSquareX() const;
	uint32_t getSquareY() const;
	uint32_t getSquareSize() const;
	uint32_t getCellX(uint8_t x) const;
	uint32_t getCellY(uint8_t y) const;

	uint32_t getButtonW() const;
	uint32_t getButtonH() const;
	uint32_t getRestartButtonX() const;
	uint32_t getRestartButtonY() const;
	uint32_t getExitButtonX() const;
	uint32_t getExitButtonY() const;
private:
	uint32_t windowW, windowH;
	uint32_t cellSize, boardSize, boardX, boardY, squareX, squareY, squareSize;
	uint32_t buttonW, buttonH, restartButtonX, restartButtonY, exitButtonX, exitButtonY;
};