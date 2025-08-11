#include "SoundManager.hpp"
#include "OSTools.hpp"


SoundManager::SoundManager() {
    this->map["move"].loadFromFile("resources/move.ogg");
    this->map["capture"].loadFromFile("resources/capture.ogg");
    this->map["notify"].loadFromFile("resources/notify.ogg");
    this->map["click"].loadFromFile("resources/click.ogg");
    this->map["screamer1"].loadFromFile("resources/screamer1.ogg");
    this->map["clicks"].loadFromFile("resources/clicks.ogg");
    this->map["calmmusic"].loadFromFile("resources/calmmusic.ogg");
    this->map["rotate"].loadFromFile("resources/rotate.ogg");
    this->map["lookingfromabove"].loadFromFile("resources/lookingfromabove.ogg");
    this->map["crush"].loadFromFile("resources/crush.ogg");
    this->map["bsod"].loadFromFile("resources/bsod.ogg");
    this->map["boy2"].loadFromFile("resources/boy2.ogg");
    this->map["areyouok"].loadFromFile("resources/areyouok.ogg");
    this->map["mother"].loadFromFile("resources/mother.ogg");
    this->map["mystery"].loadFromFile("resources/mystery.ogg");
    this->map["glitch2"].loadFromFile("resources/glitch2.ogg");
    this->map["sorry"].loadFromFile("resources/sorry.ogg");
    this->map["imissyou"].loadFromFile("resources/imissyou.ogg");
    this->map["always"].loadFromFile("resources/always.ogg");
    this->map["ghost"].loadFromFile("resources/ghost.ogg");
    this->map["secret"].loadFromFile("resources/secret.ogg");
    this->map["untouched"].loadFromFile("resources/untouched.ogg");
    this->map["wideopen"].loadFromFile("resources/wideopen.ogg");
    this->map["nowayout"].loadFromFile("resources/nowayout.ogg");
}
void SoundManager::play(const std::string &name) {
    while (!this->queue.empty() and this->queue.front().getStatus() != sf::Sound::Status::Playing) {
        this->queue.erase(this->queue.begin());
    }
    this->queue.emplace_back();
    this->queue.back().setBuffer(this->map[name]);
    this->queue.back().play();
}