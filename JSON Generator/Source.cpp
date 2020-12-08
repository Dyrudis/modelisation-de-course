#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <Windows.h>
#include "rapidjson/document.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/prettywriter.h"

using namespace std;
using namespace rapidjson;

template <typename T>
T getRandom(T const& min, T const& max) {

	T random;
	do random = (float)rand() / RAND_MAX * (max - min) + min;
	while (random == max);
	return random;

}

int main() {

	SetConsoleOutputCP(CP_UTF8);
	srand(time(NULL));
	set<string> noms;
	ifstream nameFile("noms.txt");
	if (!nameFile.is_open()) {
		cout << "Fichier \"noms.txt\" introuvable." << endl << endl;
		system("pause");
		return -1;
	}

	string nom;
	while (getline(nameFile, nom))
		noms.insert(nom);
	nameFile.close();

	Document doc;
	doc.SetObject();
	Document::AllocatorType& allocator = doc.GetAllocator();

	Value coureurs(kArrayType);

	for (int i = 0; i < 100; i++) {
		Value coureur(kObjectType);
		coureur.AddMember("Numéro de dossard", i + 1, allocator);
		set<string>::iterator it = next(noms.begin(), getRandom(0, (int)noms.size() - 1));
		string nom = *it;
		noms.erase(nom);
		nom = u8"" + nom;
		Value s(nom.c_str(), static_cast<SizeType>(nom.size()), allocator);
		coureur.AddMember("Nom", s, allocator);
		coureur.AddMember("Poids", getRandom(45.f, 120.f), allocator);
		coureur.AddMember("Taille", getRandom(1.3f, 2.f), allocator);
		coureur.AddMember("Poids de la chaussure", getRandom(100.f, 300.f), allocator);
		coureur.AddMember("Vitesse moyenne précédente", getRandom(7 / 3.6f, 20 / 3.6f), allocator);
		coureur.AddMember("Semaines de préparation", getRandom(8, 17), allocator);
		coureurs.PushBack(coureur, allocator);
	}
	
	doc.AddMember("Coureurs", coureurs, allocator);

	Value parcours(kObjectType);

	Value deniveles(kArrayType);
	for (int distance = 0; distance < 42125; distance += getRandom(200, 5000)) {
		Value denivele(kObjectType);
		denivele.AddMember("Dénivelé", getRandom(-15, 15), allocator);
		denivele.AddMember("Début du dénivelé", distance, allocator);
		deniveles.PushBack(denivele, allocator);
	}
	parcours.AddMember("Dénivelés", deniveles, allocator);

	Value ravitaillements(kArrayType);
	for (int distance = getRandom(5000, 6500); distance < 42125; distance += getRandom(4000, 6000)) {
		Value ravitaillement(kObjectType);
		ravitaillement.AddMember("Emplacement", distance, allocator);
		ravitaillements.PushBack(distance, allocator);
	}
	parcours.AddMember("Ravitaillements", ravitaillements, allocator);

	doc.AddMember("Parcours", parcours, allocator);

	FILE* file;
	fopen_s(&file, "file.json", "wb");
	if (file == 0) return -1;
	char buffer[15000];

	FileWriteStream os(file, buffer, sizeof(buffer));
	PrettyWriter<FileWriteStream> writer(os);
	doc.Accept(writer);
	fclose(file);

	cout << u8"Fichier \"file.json\" créé avec succès." << endl << endl;
	system("pause");

	return 0;

}