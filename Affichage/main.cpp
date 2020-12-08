#include "affichage.h"

using namespace std;

int main() {

	SetConsoleOutputCP(CP_UTF8);
	
	vector<Coureur> coureurs;

	cout << u8"Démarrage de la lecture..." << endl;
	sf::Clock start;
	readFile("file.json", coureurs);
	cout << u8"Temps de la lecture : " << start.getElapsedTime().asMicroseconds() / 1000000.f << " secondes." << endl << endl;

	display(coureurs);

	return 0;

}