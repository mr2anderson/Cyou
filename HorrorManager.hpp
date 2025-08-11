#pragma once


#include <memory>
#include <cstdint>
#include <string>


class SoundManager;
struct GUIState;


class HorrorManager {
public:
	HorrorManager(std::shared_ptr<GUIState> guiState, std::shared_ptr<SoundManager> soundManager, std::shared_ptr<std::wstring> error);
	void onPlayerMove();
private:
	uint32_t moves;
	std::shared_ptr<GUIState> guiState;
	std::shared_ptr<SoundManager> soundManager;
	std::shared_ptr<std::wstring> error;

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
};