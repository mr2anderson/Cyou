#include "HorrorManager.hpp"
#include "GUIState.hpp"
#include "SoundManager.hpp"
#include "GlobalRandomGenerator.hpp"
#include "OSTools.hpp"
#include "ScaryPlaylist.hpp"
#include <filesystem>
#include <fstream>
#include <chrono>


#define USE_BSOD


HorrorManager::HorrorManager(std::shared_ptr<GUIState> guiState, std::shared_ptr<SoundManager> soundManager, std::shared_ptr<std::wstring> error, std::shared_ptr<ScaryPlaylist> playlist) {
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
	this->playlist = playlist;
}
void HorrorManager::onPlayerMove() {
	const uint32_t screamer1 = 53;
	const uint32_t lookingFromAbove = screamer1 + 19;
	const uint32_t shadow = lookingFromAbove + 12;
	const uint32_t areYouOk = shadow + 1;
	const uint32_t rotate = areYouOk + 19;
	const uint32_t clicks = rotate + 14;
	const uint32_t translateLabel = clicks + 18;
	const uint32_t crush = translateLabel + 9;
	const uint32_t calmMusic = crush + 19;
	const uint32_t dance = calmMusic + 16;
	const uint32_t boy2 = dance + 7;
	const uint32_t bsod = boy2 + 8;
	this->moves += 1;
	std::ofstream file("appdata/progress.cfg");
	file << std::to_string(this->moves);
	file.close();
	switch (this->moves) {
	case screamer1:
		this->screamer1();
		break;
	case lookingFromAbove:
		this->lookingFromAbove();
		break;
	case shadow:
		this->shadow();
		break;
	case areYouOk:
		this->areYouOk();
		break;
	case rotate:
		this->rotate();
		break;
	case clicks:
		this->clicks();
		break;
	case translateLabel:
		this->translateLabel();
		break;
	case crush:
		this->crush();
		break;
	case calmMusic:
		this->calmMusic();
		break;
	case dance:
		this->dance();
		break;
	case boy2:
		this->boy2();
		break;
	case bsod:
		this->bsod();
		break;
	}
}
void HorrorManager::update() {
	if (this->timeEventTimer.getElapsedTime().asSeconds() > 5 * 60) {
		this->timeEventTimer.restart();
		uint32_t random = GlobalRandomGenerator::get().gen() % 6;
		switch (random) {
		case 0:
			this->_mother();
			break;
		case 1:
			this->_mystery();
			break;
		case 2:
			this->_glitch();
			break;
		case 3:
			this->_glitch2();
			break;
		case 4:
			this->_sorry();
			break;
		case 5:
			this->_imissyou();
		}
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
	this->playlist->stop();
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
	this->playlist->stop();
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
void HorrorManager::_mother() {
	this->soundManager->play("mother");
}
void HorrorManager::_mystery() {
	this->soundManager->play("mystery");
}
void HorrorManager::_glitch() {
	this->soundManager->play("glitch");
}
void HorrorManager::_glitch2() {
	this->soundManager->play("glitch2");
}
void HorrorManager::_sorry() {
	this->soundManager->play("sorry");
}
void HorrorManager::_imissyou() {
	this->soundManager->play("imissyou");
}