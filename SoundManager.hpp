#pragma once


#include <unordered_map>
#include <SFML/Audio.hpp>
#include <string>
#include <list>



class SoundManager {
public:
	SoundManager();
	void play(const std::string& name);
private:
	std::unordered_map<std::string, sf::SoundBuffer> map;
	std::list<sf::Sound> queue;
};