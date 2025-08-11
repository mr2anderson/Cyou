#include "HorrorManager.hpp"
#include "GUIState.hpp"
#include "SoundManager.hpp"
#include "GlobalRandomGenerator.hpp"
#include "OSTools.hpp"
#include <filesystem>
#include <fstream>
#include <chrono>


#define USE_BSOD


HorrorManager::HorrorManager(std::shared_ptr<GUIState> guiState, std::shared_ptr<SoundManager> soundManager, std::shared_ptr<std::wstring> error) {
	if (std::filesystem::exists("appdata/progress.cfg")) {
		std::ifstream file("appdata/progress.cfg");
		std::string buff;
		std::getline(file, buff);
		this->moves = std::stoi(buff);
	}
	else {
		this->moves = 0;
	}
	this->guiState = guiState;
	this->soundManager = soundManager;
	this->error = error;
}
void HorrorManager::onPlayerMove() {
	this->moves += 1;
	std::ofstream file("appdata/progress.cfg");
	file << std::to_string(this->moves);
	file.close();
	switch (this->moves) {
	case 23:
		this->screamer1();
		break;
	case 37:
		this->lookingFromAbove();
		break;
	case 44:
		this->shadow();
		break;
	case 45:
		this->areYouOk();
		break;
	case 59:
		this->rotate();
		break;
	case 68:
		this->clicks();
		break;
	case 86:
		this->translateLabel();
		break;
	case 92:
		this->crush();
		break;
	case 97:
		this->calmMusic();
		break;
	case 108:
		this->dance();
		break;
	case 112:
		this->boy2();
		break;
	case 115:
		this->bsod();
		break;
	}
}
void HorrorManager::screamer1() {
	this->soundManager->play("screamer1");
}
void HorrorManager::clicks() {
	this->soundManager->play("clicks");
}
void HorrorManager::rotate() {
	auto n = std::chrono::system_clock::now();
	auto d = n.time_since_epoch();
	auto m = std::chrono::duration_cast<std::chrono::milliseconds>(d).count() + 2500;
	for (uint8_t y = 0; y < 8; y = y + 1) {
		for (uint8_t x = 0; x < 8; x = x + 1) {
			if (GlobalRandomGenerator::get().gen() % 2 == 0) {
				this->guiState->rotatedBeforeMs[y][x] = m;
			}
		}
	}
	this->soundManager->play("rotate");
}
void HorrorManager::calmMusic() {
	auto n = std::chrono::system_clock::now();
	auto d = n.time_since_epoch();
	auto m = std::chrono::duration_cast<std::chrono::milliseconds>(d).count() + 50000;
	this->soundManager->play("calmmusic");
}
void HorrorManager::translateLabel() {
	this->guiState->translatedLabel = true;
}
void HorrorManager::shadow() {
	auto n = std::chrono::system_clock::now();
	auto d = n.time_since_epoch();
	auto m = std::chrono::duration_cast<std::chrono::milliseconds>(d).count() + 250;
	this->guiState->screamer = std::make_tuple(m, "shadow");
}
void HorrorManager::lookingFromAbove() {
	auto n = std::chrono::system_clock::now();
	auto d = n.time_since_epoch();
	auto m = std::chrono::duration_cast<std::chrono::milliseconds>(d).count() + 1000;
	this->guiState->screamer = std::make_tuple(m, "lookingfromabove");
	this->soundManager->play("lookingfromabove");
}
void HorrorManager::crush() {
	std::ofstream file("appdata/crush.cfg");
	file.close();
	*this->error = L"Вы смотрите на себя со стороны?";
}
void HorrorManager::dance() {
	auto n = std::chrono::system_clock::now();
	auto d = n.time_since_epoch();
	auto m = std::chrono::duration_cast<std::chrono::milliseconds>(d).count() + 10000;
	this->guiState->screamer = std::make_tuple(m, "dance");
}
void HorrorManager::bsod() {
	this->soundManager->play("bsod");

	std::ofstream file1("appdata/theend.cfg");
	file1.close();

	std::ofstream file2("C:\\Users\\" + std::string(getenv("username")) + "\\Desktop\\diagnosis.txt");
	file2 << "Пациент: " << getenv("username") << "\n";
	file2 << "Диагноз: Диссоциативное расстройство идентичности\n";
	file2 << "Симптомы: Видит себя персонажем компьютерных шахмат\n";
	file2 << "Прогноз: Неблагоприятный\n";
	file2.close();
	
	#ifdef USE_BSOD
		OSTools::bsod();
	#else
		std::terminate();
	#endif
}
void HorrorManager::boy2() {
	this->soundManager->play("boy2");
	auto n = std::chrono::system_clock::now();
	auto d = n.time_since_epoch();
	auto m = std::chrono::duration_cast<std::chrono::milliseconds>(d).count() + 10000;
	this->guiState->useBoy2BeforeMs = m;
}
void HorrorManager::areYouOk() {
	auto n = std::chrono::system_clock::now();
	auto d = n.time_since_epoch();
	auto m = std::chrono::duration_cast<std::chrono::milliseconds>(d).count() + 5000;
	this->guiState->screamer = std::make_tuple(m, "areyouok");
	this->soundManager->play("areyouok");
}