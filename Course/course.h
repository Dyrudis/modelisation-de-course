#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <Windows.h>
#include <sstream>
#include <iomanip>
#include <string>
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/prettywriter.h"
#include <SFML/Graphics.hpp>

class Parcours;

class Coureur {
private:
	int numero;
	std::string nom;
	float poids, taille, poidsChaussure, vitesseMoyenne, preparation, hydratation, vitesse, soushydratation, Ptmax;
	std::vector<int> distance;
	bool abandon;

	float getVitesse(Parcours const& parcours, float const& vent);
	float getVitesseIntrinseque(float const& vent);
	float coefChaussure();
	float coefPreparation();
	float coefHydratation();
	float coefDenivele(Parcours const& parcours);
public:
	Coureur() : numero(0), nom(""), poids(0), taille(0), poidsChaussure(0), vitesseMoyenne(0), preparation(0), hydratation(0), vitesse(0), soushydratation(0), Ptmax(0), abandon(false) {}
	bool estArrive(Parcours const& parcours) const;
	void calculDistance(Parcours const& parcours, float const& vent);
	friend void readFile(std::string const& path, std::vector<Coureur>& coureursTab, Parcours& parcours);
	friend void saveFile(std::string const& path, std::vector<Coureur>& coureursTab);
};

class Parcours {
private:
	float distance = 42195;
	std::vector<int> ravitaillements;
	std::vector<std::vector<int>> deniveles;
public:
	float getDistance() const;
	std::vector<int> const& getRavitaillements() const;
	std::vector<std::vector<int>> const& getDeniveles() const;
	friend void readFile(std::string const& path, std::vector<Coureur>& coureursTab, Parcours& parcours);
};

// Retourne une valeur x tel que : min <= x < max
template <typename T>
T getRandom(T const& min, T const& max);

//std::string timeToString(int const& secondes);

//void display(std::vector<Coureur>& coureurs);