#pragma once


#include <memory>


class Board;
enum State;
class UCIEngine;
struct GUIState;
class SoundManager;
class Layout;
class UCIEngineController;
class HorrorManager;
class UCIEngineLevel;


class InputController {
public:
	InputController(std::shared_ptr<Board> board, std::shared_ptr<State> state, std::shared_ptr<UCIEngine> uci, std::shared_ptr<UCIEngineController> uciController, std::shared_ptr<GUIState> guiState, std::shared_ptr<SoundManager> soundManager, std::shared_ptr<bool> playerToMove, std::shared_ptr<const Layout> boardLayout, std::shared_ptr<UCIEngineLevel> level, std::shared_ptr<bool> closeFlag, std::shared_ptr<HorrorManager> horrorManager);
	void onMouseButtonPressed(uint32_t mouseX, uint32_t mouseY);
private:
	std::shared_ptr<Board> board;
	std::shared_ptr<State> state;
	std::shared_ptr<UCIEngine> uci;
	std::shared_ptr<UCIEngineController> uciController;
	std::shared_ptr<GUIState> guiState;
	std::shared_ptr<SoundManager> soundManager;
	std::shared_ptr<bool> playerToMove;
	std::shared_ptr<const Layout> boardLayout;
	std::shared_ptr<UCIEngineLevel> level;
	std::shared_ptr<bool> closeFlag;
	std::shared_ptr<HorrorManager> horrorManager;

	void select(uint8_t x, uint8_t y);
	void unselect();
	void move(uint8_t x, uint8_t y);
	void restart();
	void exit();
};