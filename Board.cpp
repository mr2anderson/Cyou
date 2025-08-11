#include "Board.hpp"
#include "ChessUtils.hpp"


Board::Board() {
	this->fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
}
Board::Board(const std::string& fen) {
	this->fen = fen;
}
std::string Board::getFen() const {
	return this->fen;
}
void Board::setFen(const std::string& fen) {
	this->fen = fen;
}
std::array<std::array<char, 8>, 8> Board::getArray() const {
	return ChessUtils::getArray(this->fen);
}