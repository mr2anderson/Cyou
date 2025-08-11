#include "ScaryPlaylist.hpp"


ScaryPlaylist::ScaryPlaylist() {
	this->music.openFromFile("resources/music.ogg");
	this->music.setVolume(50);
	this->music.play();

	this->paradox.openFromFile("resources/paradox.ogg");
	this->paradox.setLoop(true);
	this->paradox.setVolume(1);
	this->paradox.play();

	this->stopped = false;
}
void ScaryPlaylist::update() {
	if (this->stopped) {
		return;
	}
	if (this->music.getStatus() != sf::Music::Playing) {
		if (this->afterStop == std::nullopt) {
			this->afterStop = std::make_optional<sf::Clock>();
		}
		if (this->afterStop.value().getElapsedTime().asSeconds() > 60) {
			this->music.stop();
			this->music.play();
			this->afterStop = std::nullopt;
		}
	}
}
void ScaryPlaylist::stop() {
	this->stopped = true;
	this->music.stop();
}