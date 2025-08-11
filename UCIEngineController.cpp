#include "UCIEngineController.hpp"
#include "Board.hpp"
#include "UCIEngine.hpp"
#include "ChessUtils.hpp"
#include "SoundManager.hpp"
#include "State.hpp"
#include "GlobalRandomGenerator.hpp"
#include "UCIEngineLevel.hpp"
#include <chrono>


UCIEngineController::UCIEngineController(std::shared_ptr<Board> board, std::shared_ptr<State> state, std::shared_ptr<UCIEngine> uci, std::shared_ptr<SoundManager> soundManager, std::shared_ptr<bool> playerToMove, std::shared_ptr<const UCIEngineLevel> level) {
	this->needToMove = false;

	this->thinking = false;
	this->moveReady = false;

	this->board = board;
	this->state = state;
	this->uci = uci;
	this->soundManager = soundManager;
	this->playerToMove = playerToMove;
	this->level = level;
}
void UCIEngineController::setupMove() {
	this->needToMove = true;
}
void UCIEngineController::process() {
	if (needToMove && !thinking) {
		this->needToMove = false;
		thinking = true;
		std::array<std::array<char, 8>, 8> representation = this->board->getArray();
		std::thread([this] {
			auto n1 = std::chrono::system_clock::now();
			auto d1 = n1.time_since_epoch();
			auto m1 = std::chrono::duration_cast<std::chrono::milliseconds>(d1).count();
			auto mv = this->uci->getBestMove(this->board->getFen(), this->level->getValue());
			for (; ;) {
				auto n2 = std::chrono::system_clock::now();
				auto d2 = n2.time_since_epoch();
				auto m2 = std::chrono::duration_cast<std::chrono::milliseconds>(d2).count();
				if (m2 - m1 > 1000) {
					break;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
			{
				std::lock_guard<std::mutex> lock(this->mtx);
				this->pendingMove = mv;
				this->moveReady = true;
			}
			thinking = false;
			}).detach();
	}
	if (moveReady) {
		std::lock_guard<std::mutex> lock(this->mtx);
		std::array<std::array<char, 8>, 8> representation = this->board->getArray();
		if (pendingMove == "(none)") {
			if (this->uci->check(this->board->getFen())) {
				*this->state = State::WhiteWon;
			}
			else {
				*this->state = State::Draw;
			}
			this->soundManager->play("notify");
		}
		else {
			this->board->setFen(this->uci->move(this->board->getFen(), pendingMove));
			*this->playerToMove = true;
			std::vector<std::string> playerMoves = this->uci->getMoves(this->board->getFen());
			if (playerMoves.empty()) {
				if (this->uci->check(this->board->getFen())) {
					*this->state = State::BlackWon;
				}
				else {
					*this->state = State::Draw;
				}
				this->soundManager->play("notify");
			}
			else {
				if (representation[ChessUtils::cellToY(ChessUtils::moveToDst(pendingMove))][ChessUtils::cellToX(ChessUtils::moveToDst(pendingMove))] == ' ') {
					this->soundManager->play("move");
				}
				else {
					this->soundManager->play("capture");
				}
			}
		}
		moveReady = false;
	}
}