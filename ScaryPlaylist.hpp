#pragma once


#include <SFML/Audio.hpp>
#include <optional>


class ScaryPlaylist {
public:
	ScaryPlaylist();
	void update();
private:
	sf::Music music;
	std::optional<sf::Clock> afterStop;
};