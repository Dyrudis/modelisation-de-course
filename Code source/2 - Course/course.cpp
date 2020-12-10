#include "course.h"

/*----------------------------------*\
|*  Fonctions de la classe Coureur  *|
\*----------------------------------*/
// Private
float Coureur::getVitesse(Parcours const& parcours, float const& vent) {
	vitesse = getVitesseIntrinseque(vent) * coefChaussure() * coefPreparation() * coefHydratation() * coefDenivele(parcours);
	return vitesse;
}
float Coureur::getVitesseIntrinseque(float const& vent) {
	float Pa = 0.5f * 1.205f * 0.137f * taille * pow(vitesse + vent, 2.f) * vitesse;
	float Pr = Ptmax - Pa;
	float nouvelleVitesse = Pr / (poids * 0.98f);
	return nouvelleVitesse;
}
float Coureur::coefChaussure() {
	return -0.011f / 100 * poidsChaussure + 1;
}
float Coureur::coefPreparation() {
	float distanceTotale = 42195;
	// Seuil à partir duquel le coureur commence à perdre de la vitesse linéairement pour finir à 80%
	float seuil = distanceTotale * preparation / 16;

	// Si le coureur n'a pas depassé le seuil, la vitesse est inchangé
	if (distance.back() <= seuil)
		return 1;
	// Si le coureur a depassé le seuil, la vitesse diminue linéairement
	else {
		float coefficient = (0.8f - 1) / (distanceTotale - seuil);
		return coefficient * (distance.back() - seuil) + 1;
	}
}
float Coureur::coefHydratation() {
	float heures = distance.size() / 3600.f;
	float performance = hydratation / (0.5f * heures);
	if (performance > 0.9f) {
		soushydratation = 0; // Reset de la distance d'abandon
		return 1.f;
	}
	else if (performance > 0.4f) {
		soushydratation = 0; // Reset de la distance d'abandon
		float coefficient = (0.99f - 0.8f) / (0.9f - 0.4f);
		return coefficient * (performance - 0.4f) + 0.8f;
	}
	else {
		if (soushydratation && distance.back() - soushydratation > 1) {
			abandon = true;
			distance.back() = -1;
			return 0;
		}
		else if (soushydratation == 0)
			soushydratation = distance.back();
		return 0.8f;
	}
}
float Coureur::coefDenivele(Parcours const& parcours) {
	std::vector<std::vector<int>> const& deniveles = parcours.getDeniveles();
	int denivele = deniveles[0][0];
	for (auto const& d : deniveles)
		if (d[1] < distance.back()) denivele = d[0];

	if (denivele < 0)
		return 1 - 0.0035 * (denivele / 1.5f);
	else
		return 1 - 0.01 * (denivele / 1.5f);

}
// Public
bool Coureur::estArrive(Parcours const& parcours) const {
	return distance.back() >= parcours.getDistance() || abandon;
}
void Coureur::calculDistance(Parcours const& parcours, float const& vent) {
	distance.push_back(distance.back() + getVitesse(parcours, vent));

	std::vector<int> ravitaillements = parcours.getRavitaillements();
	std::vector<int>::iterator prochainRavitaillement = std::lower_bound(ravitaillements.begin(), ravitaillements.end(), distance.at(distance.size() - 2));
	if (distance.size() > 1 && prochainRavitaillement != ravitaillements.end()) {
		if (*prochainRavitaillement < distance.back()) {
			hydratation += getRandom(0.f, 0.5f);
		}
	}
}



/*-----------------------------------*\
|*  Fonctions de la classe Parcours  *|
\*-----------------------------------*/
// Public
float Parcours::getDistance() const {
	return distance;
}
std::vector<int> const& Parcours::getRavitaillements() const {
	return ravitaillements;
}
std::vector<std::vector<int>> const& Parcours::getDeniveles() const {
	return deniveles;
}



/*--------------------*\
|*  Autres fonctions  *|
\*--------------------*/
void readFile(std::string const& path, std::vector<Coureur>& coureursTab, Parcours& parcours) {
	using namespace rapidjson;
	FILE* file;
	fopen_s(&file, "file.json", "rb");
	if (file == NULL) {
		std::cout << "Fichier \"file.json\" introuvable." << std::endl << std::endl;
		system("pause");
		exit(-1);
	}
	char buffer[15000];
	FileReadStream is(file, buffer, sizeof(buffer));
	Document doc;
	doc.ParseStream(is);
	if (doc.HasMember("Coureurs")) {
		Value const& coureurs = doc["Coureurs"];
		for (auto const& d : coureurs.GetArray()) {
			Coureur tmp;
			Value::ConstMemberIterator itr = d.MemberBegin();
			tmp.numero = itr++->value.GetInt();
			tmp.nom = itr++->value.GetString();
			tmp.poids = itr++->value.GetFloat();
			tmp.taille = itr++->value.GetFloat();
			tmp.poidsChaussure = itr++->value.GetFloat();
			tmp.vitesseMoyenne = itr++->value.GetFloat();
			tmp.preparation = itr->value.GetInt();
			tmp.hydratation = 0.5;
			tmp.distance.push_back(0);
			tmp.vitesse = 0;
			tmp.Ptmax = tmp.vitesseMoyenne * tmp.poids * 0.98f + 0.5f * 1.205f * 0.137f * tmp.taille * pow(tmp.vitesseMoyenne, 3.f);
			coureursTab.push_back(tmp);
		}
	}
	if (doc.HasMember("Parcours")) {
		if (doc["Parcours"].HasMember("Dénivelés")) {
			Value const& deniveles = doc["Parcours"]["Dénivelés"];
			for (auto const& d : deniveles.GetArray()) {
				std::vector<int> tmp;
				Value::ConstMemberIterator itr = d.MemberBegin();
				tmp.push_back(itr++->value.GetInt());
				tmp.push_back(itr->value.GetInt());
				parcours.deniveles.push_back(tmp);
			}
		}
		if (doc["Parcours"].HasMember("Ravitaillements")) {
			Value const& ravitaillements = doc["Parcours"]["Ravitaillements"];
			for (auto const& d : ravitaillements.GetArray()) {
				parcours.ravitaillements.push_back(d.GetInt());
			}
		}
	}
	fclose(file);
}

void saveFile(std::string const& path, std::vector<Coureur>& coureursTab) {
	using namespace rapidjson;

	Document doc;
	doc.SetObject();
	Document::AllocatorType& allocator = doc.GetAllocator();

	Value coureurs(kArrayType);

	for (auto const& c : coureursTab) {
		Value coureur(kObjectType);
		Value nom(c.nom.c_str(), static_cast<SizeType>(c.nom.size()), allocator);
		coureur.AddMember("Nom", nom, allocator);
		coureur.AddMember("Abandon", c.abandon, allocator);
		int second = 0;
		for (auto const& dist : c.distance) {
			Value s(std::to_string(second).c_str(), static_cast<SizeType>(std::to_string(second).size()), allocator);
			coureur.AddMember(s, dist, allocator);
			second++;
		}
		coureurs.PushBack(coureur, allocator);
	}

	doc.AddMember("Coureurs", coureurs, allocator);
	FILE* file;
	fopen_s(&file, path.c_str(), "wb");
	if (file == 0) {
		std::cout << "Impossible d'écrire les résultats dans un fichier \"result.json\"" << std::endl;
		system("pause");
		exit(-1);
	}
	char buffer[15000];

	FileWriteStream os(file, buffer, sizeof(buffer));
	PrettyWriter<FileWriteStream> writer(os);
	doc.Accept(writer);
	fclose(file);
}

template <typename T>
T getRandom(T const& min, T const& max) {
	T random;
	do random = (float)rand() / RAND_MAX * ((float)max - min) + min;
	while (random == max);
	return random;
}