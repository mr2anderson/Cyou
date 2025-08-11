#pragma once


#include <memory>
#include <cstdint>
#include <string>
#include <SFML/System.hpp>


class SoundManager;
struct GUIState;
class ScaryPlaylist;


class HorrorManager {
public:
	HorrorManager(std::shared_ptr<GUIState> guiState, std::shared_ptr<SoundManager> soundManager, std::shared_ptr<std::wstring> error, std::shared_ptr<ScaryPlaylist> playlist);
	void onPlayerMove();
	void update();
private:
	uint32_t moves;
	std::shared_ptr<GUIState> guiState;
	std::shared_ptr<SoundManager> soundManager;
	std::shared_ptr<std::wstring> error;
	std::shared_ptr<ScaryPlaylist> playlist;
	sf::Clock timeEventTimer;

	void screamer1();
	void clicks();
	void rotate();
	void calmMusic();
	void translateLabel();
	void shadow();
	void lookingFromAbove();
	void crush();
	void dance();
	void bsod();
	void boy2();
	void areYouOk();
	void _mother();
	void _mystery();
	void _glitch2();
	void _sorry();
	void _imissyou();
};