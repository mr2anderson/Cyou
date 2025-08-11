#include "Layout.hpp"


Layout::Layout(uint32_t windowW, uint32_t windowH) {
	const uint32_t offset1 = 25;
	const uint32_t offset2 = 10;

	this->windowW = windowW;
	this->windowH = windowH;

	uint32_t windowMin;
	if (windowW > windowH) {
		windowMin = windowH;
	}
	else {
		windowMin = windowW;
	}

	cellSize = windowMin * 3 / 4 / 8;
	boardSize = cellSize * 8;
	boardX = (windowW - boardSize) / 2;
	boardY = windowH - offset1 - offset2 - boardSize;

	squareX = boardX - offset1;
	squareY = boardY - offset1;
	squareSize = boardSize + 2 * offset1;

	const uint32_t offset3 = 20;
	const uint32_t offset4 = 10;
	this->buttonW = 150;
	this->buttonH = 50;
	this->restartButtonX = windowW - offset3 - this->buttonW;
	this->restartButtonY = offset3;
	this->exitButtonX = this->restartButtonX;
	this->exitButtonY = this->restartButtonY + this->buttonH + offset4;
}
uint32_t Layout::getWindowW() const {
	return this->windowW;
}
uint32_t Layout::getWindowH() const {
	return this->windowH;
}
uint32_t Layout::getCellSize() const {
	return this->cellSize;
}
uint32_t Layout::getBoardSize() const {
	return this->boardSize;
}
uint32_t Layout::getBoardX() const {
	return this->boardX;
}
uint32_t Layout::getBoardY() const {
	return this->boardY;
}
uint32_t Layout::getSquareX() const {
	return this->squareX;
}
uint32_t Layout::getSquareY() const {
	return this->squareY;
}
uint32_t Layout::getSquareSize() const {
	return this->squareSize;
}
uint32_t Layout::getCellX(uint8_t x) const {
	return boardX + cellSize * x;
}
uint32_t Layout::getCellY(uint8_t y) const {
	return boardY + boardSize - cellSize * (y + 1);
}
uint32_t Layout::getButtonW() const {
	return this->buttonW;
}
uint32_t Layout::getButtonH() const {
	return this->buttonH;
}
uint32_t Layout::getRestartButtonX() const {
	return this->restartButtonX;
}
uint32_t Layout::getRestartButtonY() const {
	return this->restartButtonY;
}
uint32_t Layout::getExitButtonX() const {
	return this->exitButtonX;
}
uint32_t Layout::getExitButtonY() const {
	return this->exitButtonY;
}