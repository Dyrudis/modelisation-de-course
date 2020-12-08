#include "course.h"

using namespace std;

int main() {
	SetConsoleOutputCP(CP_UTF8);
	srand(time(NULL));

	vector<Coureur> coureurs;
	Parcours parcours;

	sf::Int64 execTime = 0;

	/*----------------------------------*\
	|*  Lecture du fichier "file.json"  *|
	\* ---------------------------------*/
	cout << u8"Démarrage de la lecture..." << endl;
	sf::Clock start;
	readFile("file.json", coureurs, parcours);
	cout << u8"Temps de la lecture : " << start.getElapsedTime().asMicroseconds() / 1000000.f << " secondes." << endl << endl;
	execTime += start.getElapsedTime().asMicroseconds();


	/*--------------------------------------------*\
	|*  Calcul des performances de chaque joueur  *|
	\* -------------------------------------------*/
	cout << u8"Démarrage des calculs..." << endl;
	start = sf::Clock();
	int totalTime = 0;
	float vent = 0;
	while (any_of(coureurs.begin(), coureurs.end(), [&parcours](Coureur const& c) { return !c.estArrive(parcours); })) {
		vent += getRandom(-0.1f, 0.1f);
		for (auto& coureur : coureurs) {
			if (!coureur.estArrive(parcours))
				coureur.calculDistance(parcours, getRandom(-5.f, 5.f));
		}
		totalTime++;
	}
	/*for (auto& coureur : coureurs)
		coureur.distance.back() = parcours.getDistance();*/
	cout << u8"Temps des calculs : " << start.getElapsedTime().asMicroseconds() / 1000000.f << " secondes." << endl << endl;
	execTime += start.getElapsedTime().asMicroseconds();




	/*-----------------------------------*\
	|*  Écriture du fichier "file.json"  *|
	\* ----------------------------------*/
	cout << u8"Démarrage de l'écriture..." << endl;
	start = sf::Clock();
	saveFile("result.json", coureurs);
	cout << u8"Temps de l'écriture : " << start.getElapsedTime().asMicroseconds() / 1000000.f << " secondes." << endl << endl;
	execTime += start.getElapsedTime().asMicroseconds();

	cout << u8"Temps total de l'éxécution : " << (execTime + start.getElapsedTime().asMicroseconds()) / 1000000.f << " secondes." << endl << endl;

	system("pause");

	return 0;

}
