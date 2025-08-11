#pragma once


#include <memory>
#include <cstdint>
#include <string>


class Layout;
class Board;
class Renderer;
namespace sf {
	class RenderWindow;
}
struct GUIState;
class InputController;
class SoundManager;
class UCIEngine;
class UCIEngineController;
enum State;
class HorrorManager;
class UCIEngineLevel;


class App {
public:
	App();
	void launch();
private:
	std::shared_ptr<Layout> boardLayout;
	std::shared_ptr<Board> board;
	std::shared_ptr<Renderer> boardRenderer;
	std::shared_ptr<sf::RenderWindow> window;
	std::shared_ptr<GUIState> guiState;
	std::shared_ptr<bool> playerToMove;
	std::shared_ptr<State> state;
	std::shared_ptr<InputController> inputController;
	std::shared_ptr<SoundManager> soundManager;
	std::shared_ptr<UCIEngine> uciEngine;
	std::shared_ptr<UCIEngineController> uciController;
	std::shared_ptr<UCIEngineLevel> level;
	std::shared_ptr<bool> closeFlag;
	std::shared_ptr<std::wstring> error;
	std::shared_ptr<HorrorManager> horrorManager;
};