#define NOMINMAX


#include "InputController.hpp"
#include "UCIEngine.hpp"
#include "Board.hpp"
#include "GUIState.hpp"
#include "ChessUtils.hpp"
#include "SoundManager.hpp"
#include "Layout.hpp"
#include "UCIEngineController.hpp"
#include "State.hpp"
#include "HorrorManager.hpp"
#include "UCIEngineLevel.hpp"
#include <chrono>
#include <fstream>
#include <iostream>


InputController::InputController(std::shared_ptr<Board> board, std::shared_ptr<State> state, std::shared_ptr<UCIEngine> uci, std::shared_ptr<UCIEngineController> uciController, std::shared_ptr<GUIState> guiState, std::shared_ptr<SoundManager> soundManager, std::shared_ptr<bool> playerToMove, std::shared_ptr<const Layout> boardLayout, std::shared_ptr<UCIEngineLevel> level, std::shared_ptr<bool> closeFlag, std::shared_ptr<HorrorManager> horrorManager) {
	this->board = board;
	this->state = state;
	this->uci = uci;
	this->uciController = uciController;
	this->guiState = guiState;
	this->soundManager = soundManager;
	this->playerToMove = playerToMove;
	this->boardLayout = boardLayout;
	this->level = level;
	this->closeFlag = closeFlag;
	this->horrorManager = horrorManager;
}
void InputController::onMouseButtonPressed(uint32_t mouseX, uint32_t mouseY) {
	auto n = std::chrono::system_clock::now();
	auto d = n.time_since_epoch();
	auto m = std::chrono::duration_cast<std::chrono::milliseconds>(d).count();
	if (std::get<0>(this->guiState->screamer) > m) {
		return;
	}

	if (*this->playerToMove or (*this->state) != State::GameInProgress) {
		if (mouseX >= this->boardLayout->getRestartButtonX() and mouseX < this->boardLayout->getRestartButtonX() + this->boardLayout->getButtonW() and mouseY >= this->boardLayout->getRestartButtonY() and mouseY < this->boardLayout->getRestartButtonY() + this->boardLayout->getButtonH()) {
			restart();
		}
		else if (mouseX >= this->boardLayout->getExitButtonX() and mouseX < this->boardLayout->getExitButtonX() + this->boardLayout->getButtonW() and mouseY >= this->boardLayout->getExitButtonY() and mouseY < this->boardLayout->getExitButtonY() + this->boardLayout->getButtonH()) {
			exit();
		}
		else if (*this->state == State::GameInProgress) {
			bool mouseInBoard = (mouseX >= this->boardLayout->getBoardX() and mouseY >= this->boardLayout->getBoardY() and mouseX < this->boardLayout->getBoardX() + this->boardLayout->getBoardSize() and mouseY < this->boardLayout->getBoardY() + this->boardLayout->getBoardSize());
			if (mouseInBoard) {
				mouseX = mouseX - this->boardLayout->getBoardX();
				mouseY = mouseY - this->boardLayout->getBoardY();
				uint8_t x = mouseX / this->boardLayout->getCellSize();
				uint8_t y = 7 - mouseY / this->boardLayout->getCellSize();
				std::array<std::array<char, 8>, 8> representation = this->board->getArray();
				if (this->guiState->selected == std::nullopt) {
					if (isupper(representation[y][x])) {
						this->select(y, x);
					}
				}
				else if (std::get<0>(this->guiState->selected.value()) == x and std::get<1>(this->guiState->selected.value()) == y) {
					this->unselect();
				}
				else if (isupper(representation[y][x])) {
					this->select(y, x);
				}
				else {
					if (this->guiState->shadows[y][x] != ' ') {
						this->move(x, y);
					}
				}
			}
		}
	}
}
void InputController::select(uint8_t x, uint8_t y) {
	this->guiState->selected = std::make_optional<std::pair<uint8_t, uint8_t>>(y, x);
	for (uint8_t y = 0; y < 8; y = y + 1) {
		for (uint8_t x = 0; x < 8; x = x + 1) {
			this->guiState->shadows[y][x] = ' ';
		}
	}
	std::array<std::array<char, 8>, 8> representation = this->board->getArray();
	std::vector<std::string> moves = this->uci->getMoves(this->board->getFen());
	for (const auto& move : moves) {
		std::string src = ChessUtils::moveToSrc(move);
		std::string dst = ChessUtils::moveToDst(move);
		uint8_t srcX = ChessUtils::cellToX(src);
		uint8_t srcY = ChessUtils::cellToY(src);
		uint8_t dstX = ChessUtils::cellToX(dst);
		uint8_t dstY = ChessUtils::cellToY(dst);
		if (std::get<0>(this->guiState->selected.value()) == srcX and std::get<1>(this->guiState->selected.value()) == srcY) {
			this->guiState->shadows[dstY][dstX] = representation[srcY][srcX];
		}
	}
	this->soundManager->play("move");
}
void InputController::unselect() {
	this->guiState->selected = std::nullopt;
	for (uint8_t y = 0; y < 8; y = y + 1) {
		for (uint8_t x = 0; x < 8; x = x + 1) {
			this->guiState->shadows[y][x] = ' ';
		}
	}
	this->soundManager->play("move");
}
void InputController::move(uint8_t x, uint8_t y) {
	std::array<std::array<char, 8>, 8> representation = this->board->getArray();
	std::string src = ChessUtils::xyToCell(std::get<0>(this->guiState->selected.value()), std::get<1>(this->guiState->selected.value()));
	std::string dst = ChessUtils::xyToCell(x, y);
	std::string mv = ChessUtils::srcDstToMove(src, dst);
	if (y == 7 and representation[std::get<1>(this->guiState->selected.value())][std::get<0>(this->guiState->selected.value())] == 'P') {
		mv.push_back('q');
	}
	this->guiState->selected = std::nullopt;
	for (uint8_t y = 0; y < 8; y = y + 1) {
		for (uint8_t x = 0; x < 8; x = x + 1) {
			this->guiState->shadows[y][x] = ' ';
		}
	}
	this->board->setFen(this->uci->move(this->board->getFen(), mv));
	*this->playerToMove = false;
	if (representation[y][x] == ' ') {
		this->soundManager->play("move");
	}
	else {
		this->soundManager->play("capture");
	}
	this->horrorManager->onPlayerMove();
	this->uciController->setupMove();
}
void InputController::restart() {
	if (*this->state == State::WhiteWon) {
		this->level->increase();
	}
	*this->board = Board();
	*this->playerToMove = true;
	*this->state = State::GameInProgress;
	this->guiState->selected = std::nullopt;
	for (uint8_t y = 0; y < 8; y = y + 1) {
		for (uint8_t x = 0; x < 8; x = x + 1) {
			this->guiState->shadows[y][x] = ' ';
		}
	}
	this->soundManager->play("click");
}
void InputController::exit() {
	*this->closeFlag = true;
}