#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <string>
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/prettywriter.h"
#include <SFML/Graphics.hpp>

class Coureur {
public:
	std::string nom;
	std::vector<int> distance;
	bool abandon;
public:
	Coureur() : nom(""), abandon(false) {}
	friend void readFile(std::string const& path, std::vector<Coureur>& coureursTab);
};

class Timer {
private:
	sf::Clock clock;
	float startPause, pauseDuration, delta, maxTime;
	int speed;
	bool playing;
public:
	Timer(float max) : clock(sf::Clock()), startPause(0), pauseDuration(0), delta(0), speed(1), playing(true), maxTime(max) {};
	float getTime() const;
	void pause();
	void play();
	void setSpeed(int value);
	void addTime(int delta);
	bool getState();
};

std::string timeToString(int const& secondes);

void display(std::vector<Coureur>& coureurs);

template<typename T, typename S>
T min(T t, S s) {
	return t < s ? t : s;
}

template<typename T, typename S>
T max(T t, S s) {
	return t > s ? t : s;
}