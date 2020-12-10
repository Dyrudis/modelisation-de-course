#include "affichage.h"

using namespace std;

int main() {
	
	vector<Coureur> coureurs;

	cout << "Demarrage de la lecture..." << endl;
	sf::Clock start;
	readFile("file.json", coureurs);
	cout << "Temps de la lecture : " << start.getElapsedTime().asMicroseconds() / 1000000.f << " secondes." << endl << endl;

	display(coureurs);

	return 0;
}