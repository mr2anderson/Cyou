#include "App.hpp"
#include "Layout.hpp"
#include "Board.hpp"
#include "Renderer.hpp"
#include "GUIState.hpp"
#include "InputController.hpp"
#include "UCIEngineController.hpp"
#include "SoundManager.hpp"
#include "UCIEngine.hpp"
#include "State.hpp"
#include "HorrorManager.hpp"
#include "OSTools.hpp"
#include "UCIEngineLevel.hpp"
#include "ScaryPlaylist.hpp"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


App::App() = default;
void App::launch() {
	if (!std::filesystem::exists("appdata")) {
		std::cout << "this program can interact with your operation system.\nhowever, it is absolutely safe.\npress enter to continue...";
		std::string buff;
		std::getline(std::cin, buff);
		std::filesystem::create_directory("appdata");
	}

	this->soundManager = std::make_shared<SoundManager>();

	this->window = nullptr;
	if (std::filesystem::exists("appdata/crush.cfg") or std::filesystem::exists("appdata/theend.cfg")) {
		if (std::filesystem::exists("appdata/crush.cfg")) {
			std::filesystem::remove("appdata/crush.cfg");
		}
		this->soundManager->play("crush");
		this->window = std::make_shared<sf::RenderWindow>(sf::VideoMode::getDesktopMode(), "Chess.exe", sf::Style::Fullscreen);
		this->window->setVerticalSyncEnabled(true);
		for (uint32_t i = 0; i < 4 * 60; i = i + 1) {
			this->window->clear(sf::Color(0, 0, 175 * (i % 2 == 0)));
			this->window->display();
		}
	}

	if (std::filesystem::exists("appdata/theend.cfg")) {
		this->window->close();
		OSTools::error(L"diagnosis.txt");
		return;
	}

	this->playlist = std::make_shared<ScaryPlaylist>();
	this->level = std::make_shared<UCIEngineLevel>();
	this->playerToMove = std::make_shared<bool>(true);
	this->closeFlag = std::make_shared<bool>(false);
	this->error = std::make_shared<std::wstring>();
	this->state = std::make_shared<State>(State::GameInProgress);
	this->guiState = std::make_shared<GUIState>();
	this->horrorManager = std::make_shared<HorrorManager>(this->guiState, this->soundManager, this->error, this->playlist);
	this->board = std::make_shared<Board>();
	if (this->window == nullptr) {
		this->window = std::make_shared<sf::RenderWindow>(sf::VideoMode::getDesktopMode(), "Chess.exe", sf::Style::Fullscreen);
		this->window->setVerticalSyncEnabled(true);
	}
	this->boardLayout = std::make_shared<Layout>(this->window->getSize().x, this->window->getSize().y);
	this->uciEngine = std::make_shared<UCIEngine>();
	this->uciController = std::make_shared<UCIEngineController>(this->board, this->state, this->uciEngine, this->soundManager, this->playerToMove, this->level);
	this->boardRenderer = std::make_shared<Renderer>(this->board, this->state, this->level, this->playerToMove, this->guiState, this->boardLayout);
	this->inputController = std::make_shared<InputController>(this->board, this->state, this->uciEngine, this->uciController, this->guiState, this->soundManager, this->playerToMove, this->boardLayout, this->level, this->closeFlag, this->horrorManager);

	for (; ;) {
		sf::Event event;
		while (this->window->pollEvent(event)) {
			if (event.type == sf::Event::MouseButtonPressed) {
				this->inputController->onMouseButtonPressed(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
			}
		}
		this->boardRenderer->render(this->window);
		this->window->display();
		this->uciController->process();
		this->playlist->update();
		this->horrorManager->update();
		if (*this->closeFlag or !(*this->error).empty()) {
			this->window->close();
			break;
		}
	}
	if (!(*this->error).empty()) {
		OSTools::error(*this->error);
	}
}