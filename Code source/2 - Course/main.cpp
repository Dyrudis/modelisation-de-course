#include "course.h"

using namespace std;

int main() {
	SetConsoleOutputCP(CP_UTF8);
	srand(time(NULL));

	vector<Coureur> coureurs;
	Parcours parcours;

	auto execTime = chrono::high_resolution_clock::now(); // Temps d'exécution

	/*----------------------------------*\
	|*  Lecture du fichier "file.json"  *|
	\* ---------------------------------*/
	cout << u8"Démarrage de la lecture..." << endl;
	auto start = chrono::high_resolution_clock::now();
	readFile("file.json", coureurs, parcours);
	cout << u8"Temps de la lecture : " << (chrono::high_resolution_clock::now() - start) / chrono::milliseconds(1) / 1000.f << " secondes." << endl << endl;



	/*--------------------------------------------*\
	|*  Calcul des performances de chaque joueur  *|
	\* -------------------------------------------*/
	cout << u8"Démarrage des calculs..." << endl;
	start = chrono::high_resolution_clock::now();
	int totalTime = 0;
	float vent = 0;
	while (any_of(coureurs.begin(), coureurs.end(), [&parcours](Coureur const& c) { return !c.estArrive(parcours); })) {
		vent += getRandom(-0.1f, 0.1f); // Légère variantion du vent
		for (auto& coureur : coureurs) {
			if (!coureur.estArrive(parcours))
				coureur.calculDistance(parcours, getRandom(-5.f, 5.f));
		}
		totalTime++;
	}
	cout << u8"Temps des calculs : " << (chrono::high_resolution_clock::now() - start) / chrono::milliseconds(1) / 1000.f << " secondes." << endl << endl;



	/*-----------------------------------*\
	|*  Écriture du fichier "file.json"  *|
	\* ----------------------------------*/
	cout << u8"Démarrage de l'écriture..." << endl;
	start = chrono::high_resolution_clock::now();
	saveFile("result.json", coureurs);
	cout << u8"Temps de l'écriture : " << (chrono::high_resolution_clock::now() - start) / chrono::milliseconds(1) / 1000.f << " secondes." << endl << endl;

	cout << u8"Temps total de l'éxécution : " << (chrono::high_resolution_clock::now() - execTime) / chrono::milliseconds(1) / 1000.f << " secondes." << endl << endl;

	system("pause");

	return 0;

}
