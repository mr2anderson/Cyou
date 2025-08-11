#pragma once


#include <unordered_map>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cstdint>


class Layout;
class Board;
struct GUIState;
enum State;
class UCIEngineLevel;


class Renderer {
public:
	Renderer(std::shared_ptr<const Board> board, std::shared_ptr<const State> state, std::shared_ptr<const UCIEngineLevel> level, std::shared_ptr<const bool> playerToMove, std::shared_ptr<const GUIState> guiState, std::shared_ptr<const Layout> boardLayout);
	void render(std::shared_ptr<sf::RenderWindow> window);
private:
	std::shared_ptr<const Board> board;
	std::shared_ptr<const State> state;
	std::shared_ptr<const UCIEngineLevel> level;
	std::shared_ptr<const bool> playerToMove;
	std::shared_ptr<const GUIState> guiState;
	std::shared_ptr<const Layout> boardLayout;

	std::unordered_map<char, sf::Texture> pieces;

	sf::Font ithaca;
	sf::Font mintsoda;
	sf::Font screamer;

	sf::Texture shadow;
	sf::Texture boy, boy2;

	sf::Shader shader;

	sf::Clock shiftTimer;
	bool firstShift;

	sf::Clock clock;

	void renderSquare(sf::RenderTexture &renderer);
	void renderBoard(sf::RenderTexture& renderer);
	void renderBoy(sf::RenderTexture& renderer);
	void renderLevel(sf::RenderTexture& renderer);
	void renderState(sf::RenderTexture& renderer);
	void renderButtons(sf::RenderTexture& renderer);
	void renderScreamer(sf::RenderTexture& renderer);
	void renderShadow(sf::RenderTexture& renderer);
	void renderLookingFromAbove(sf::RenderTexture& renderer);
	void renderDance(sf::RenderTexture& renderer);
	void renderAreYouOk(sf::RenderTexture& renderer);
};