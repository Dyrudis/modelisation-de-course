#include "affichage.h"

void readFile(std::string const& path, std::vector<Coureur>& coureursTab) {
	using namespace rapidjson;
	FILE* file;
	fopen_s(&file, "result.json", "rb");
	if (file == NULL) {
		std::cout << "Fichier \"result.json\" introuvable." << std::endl << std::endl;
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
			tmp.nom = itr++->value.GetString();
			tmp.abandon = itr++->value.GetBool();
			while (itr != d.MemberEnd()) {
				tmp.distance.push_back(itr++->value.GetFloat());
			}
			coureursTab.push_back(tmp);
		}
	}
	fclose(file);
}

void display(std::vector<Coureur>& coureurs) {
	using namespace sf;

	const int windowWidth = 1920 / 1.5f, windowHeight = 1080 / 1.5f, heightSpacing = 80, widthSpacing = 20, length = 750, leftMargin = windowWidth / 2.f - length / 2.f;

	RenderWindow window(VideoMode(windowWidth, windowHeight), "Marathon", Style::Titlebar | Style::Close | Style::Resize);

	Font DINNextRegular, DINNextBold, TungstenBold;
	DINNextRegular.loadFromFile("resources/DIN Next W01 Regular.ttf");
	DINNextBold.loadFromFile("resources/DIN Next W01 Bold.ttf");
	TungstenBold.loadFromFile("resources/Tungsten-Bold.ttf");

	Text titre("Marathon", TungstenBold, 80);
	titre.setPosition((windowWidth - titre.getLocalBounds().width) / 2, 15);
	titre.setFillColor(Color(15, 25, 35));
	titre.setLetterSpacing(1.5);

	RectangleShape titreBackground(Vector2f(windowWidth, 125));
	titreBackground.setPosition(0, 0);
	titreBackground.setFillColor(Color(236, 232, 225));

	RectangleShape timerBackground(Vector2f(windowWidth, 125));
	timerBackground.setPosition(0, windowHeight - timerBackground.getLocalBounds().height);
	timerBackground.setFillColor(Color(236, 232, 225));

	RectangleShape timerBarOuterRect(Vector2f(windowWidth / 3, 6));
	timerBarOuterRect.setPosition((windowWidth - timerBarOuterRect.getLocalBounds().width) / 2, windowHeight - 40);
	timerBarOuterRect.setFillColor(Color(15, 25, 35));

	Texture run;
	run.loadFromFile("resources/run.png");
	Texture fail;
	fail.loadFromFile("resources/fail.png");
	Texture win;
	win.loadFromFile("resources/win.png");
	Texture play;
	play.loadFromFile("resources/play.png");
	Texture pause;
	pause.loadFromFile("resources/pause.png");
	Texture forward;
	forward.loadFromFile("resources/forward.png");
	Texture backward;
	backward.loadFromFile("resources/backward.png");
	Texture end;
	end.loadFromFile("resources/end.png");
	Texture begin;
	begin.loadFromFile("resources/begin.png");

	Sprite play_pause;
	play_pause.setTexture(pause);
	play_pause.setColor(Color(15, 25, 35));
	play_pause.setScale(0.5, 0.5);
	play_pause.setPosition((windowWidth - play_pause.getGlobalBounds().width) / 2, windowHeight - 100);

	Sprite forwardSpr;
	forwardSpr.setTexture(forward);
	forwardSpr.setColor(Color(15, 25, 40));
	forwardSpr.setScale(0.5, 0.5);
	forwardSpr.setPosition(windowWidth / 2 + 40, windowHeight - 100);

	Sprite backwardSpr;
	backwardSpr.setTexture(backward);
	backwardSpr.setColor(Color(15, 25, 40));
	backwardSpr.setScale(0.5, 0.5);
	backwardSpr.setPosition(windowWidth / 2 - backwardSpr.getGlobalBounds().width - 40, windowHeight - 100);

	Sprite endSpr;
	endSpr.setTexture(end);
	endSpr.setColor(Color(15, 25, 40));
	endSpr.setScale(0.5, 0.5);
	endSpr.setPosition(windowWidth / 2 + 50 + forwardSpr.getGlobalBounds().width, windowHeight - 100);

	Sprite beginSpr;
	beginSpr.setTexture(begin);
	beginSpr.setColor(Color(15, 25, 40));
	beginSpr.setScale(0.5, 0.5);
	beginSpr.setPosition(windowWidth / 2 - beginSpr.getGlobalBounds().width - backwardSpr.getGlobalBounds().width - 50, windowHeight - 100);

	int totalTime = 0;
	for (Coureur const& c : coureurs)
		if (c.distance.size() > totalTime) totalTime = c.distance.size();

	Timer time(totalTime);
	int speed = 1, scroll = 0;
	float delta = 0;
	bool playing = true, clicking = false;
	while (window.isOpen()) {
		
		// Gestion des événements
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::MouseWheelScrolled && scroll + event.mouseWheelScroll.delta <= 0 && scroll + event.mouseWheelScroll.delta >= -94) {
				scroll += event.mouseWheelScroll.delta;
			}
			if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Add && speed < 4096 && time.getState()) {
					speed *= 2;
					time.setSpeed(speed);
				}
				if (event.key.code == Keyboard::Subtract && speed > 1 && time.getState()) {
					speed /= 2;
					time.setSpeed(speed);
				}
				if (event.key.code == Keyboard::Left) {
					time.addTime(-60);
				}
				if (event.key.code == Keyboard::Right) {
					time.addTime(60);
				}
				if (event.key.code == Keyboard::Space) {
					playing = !playing;
					if (playing) {
						play_pause.setTexture(pause);
						time.play();
					}
					else {
						play_pause.setTexture(play);
						time.pause();
					}
				}
			}
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == Mouse::Button::Left) {
				auto mouse = Mouse::getPosition(window);

				auto sprite = play_pause.getGlobalBounds();
				if (mouse.x > sprite.left && mouse.x < sprite.left + sprite.width &&
					mouse.y > sprite.top  && mouse.y < sprite.top  + sprite.height) {
					playing = !playing;
					if (playing) {
						play_pause.setTexture(pause);
						time.play();
					}
					else {
						play_pause.setTexture(play);
						time.pause();
					}
				}
				sprite = forwardSpr.getGlobalBounds();
				if (mouse.x > sprite.left && mouse.x < sprite.left + sprite.width &&
					mouse.y > sprite.top && mouse.y < sprite.top + sprite.height) {
					time.addTime(3600);
				}
				sprite = backwardSpr.getGlobalBounds();
				if (mouse.x > sprite.left && mouse.x < sprite.left + sprite.width &&
					mouse.y > sprite.top && mouse.y < sprite.top + sprite.height) {
					time.addTime(-3600);
				}
				sprite = endSpr.getGlobalBounds();
				if (mouse.x > sprite.left && mouse.x < sprite.left + sprite.width &&
					mouse.y > sprite.top && mouse.y < sprite.top + sprite.height) {
					time = Timer(totalTime);
					time.addTime(totalTime);
					time.pause();
					speed = 1;
					playing = false;
					play_pause.setTexture(play);
				}
				sprite = beginSpr.getGlobalBounds();
				if (mouse.x > sprite.left && mouse.x < sprite.left + sprite.width &&
					mouse.y > sprite.top && mouse.y < sprite.top + sprite.height) {
					time = Timer(totalTime);
					speed = 1;
					playing = true;
					play_pause.setTexture(pause);
				}
			}
		}

		window.clear(Color(236, 232, 225));

		int currentTime = time.getTime();
		// Dépassement du temps total
		if (currentTime >= totalTime) {
			currentTime = totalTime;
			time.pause();
			playing = false;
			play_pause.setTexture(play);
		}

		// Fonction de tri des coureurs
		std::sort(coureurs.begin(), coureurs.end(), [&](Coureur const& c1, Coureur const& c2) {
			// Les 2 coureurs sont arrivés (aucun abandon)
			if (!c1.abandon && !c2.abandon && currentTime >= c1.distance.size() && currentTime >= c2.distance.size()) {
				return c1.distance.size() < c2.distance.size();
			}
			// Les 2 coureurs ont abandonné (aucun ne cours)
			else if (c1.abandon && c2.abandon && currentTime >= c1.distance.size() && currentTime >= c2.distance.size()) {
				return c1.distance.back() > c2.distance.back();
			}
			// Le 1er coureur a abandonné
			else if (c1.abandon && currentTime >= c1.distance.size())
				return false;
			// Le 2eme coureur a abandonné
			else if (c2.abandon && currentTime >= c2.distance.size())
				return true;
			// Au moins 1 des 2 coureurs cours (aucun abandon)
			else {
				return c1.distance[min(currentTime, c1.distance.size() - 1)] > c2.distance[min(currentTime, c2.distance.size() - 1)];
			}
			});

		int height = 0, place = 0;
		for (auto const& c : coureurs) {
			place++;
			Text leftText;
			leftText.setFont(DINNextRegular);
			leftText.setCharacterSize(20);
			leftText.setString(c.nom + " - [" + std::to_string(place) + "]");
			leftText.setPosition(leftMargin - widthSpacing - leftText.getGlobalBounds().width, height + 150);
			leftText.setFillColor(Color(15, 25, 35));

			RectangleShape distanceOuterRect(Vector2f(length + 4, 44));
			distanceOuterRect.setPosition(leftMargin - 2, height + 141);
			distanceOuterRect.setFillColor(Color(15, 25, 35));

			RectangleShape distanceInnerRect;
			distanceInnerRect.setPosition(leftMargin, height + 143);

			Text rightText;
			rightText.setFont(DINNextRegular);
			rightText.setCharacterSize(20);
			rightText.setPosition(leftMargin + length + widthSpacing, height + 150);
			rightText.setFillColor(Color(15, 25, 35));

			Sprite image;
			image.setScale(0.5, 0.5);
			image.setColor(Color(236, 232, 225));

			// Cas où le coureur est arrivé
			if (currentTime >= c.distance.size() && !c.abandon) {
				distanceInnerRect.setSize(Vector2f(length, 40));
				distanceInnerRect.setFillColor(Color(255, 70, 85));

				rightText.setString(timeToString(c.distance.size()));

				image.setTexture(win, true);
				image.setPosition(leftMargin + length / 2.f - image.getLocalBounds().width / 4.f, height + 138);
			}
			// Cas où le coureur a adandonné
			else if (c.abandon && currentTime >= c.distance.size()) {
				distanceInnerRect.setSize(Vector2f((c.distance.back() / 42195.f) * length, 40));
				distanceInnerRect.setFillColor(Color(0, 70, 85));

				rightText.setString("Abandon : " + timeToString(c.distance.size()));

				image.setTexture(fail, true);
				image.setPosition(leftMargin + length / 2.f - image.getLocalBounds().width / 4.f, height + 138);
			}
			// Cas où le coureur continue sa course
			else {
				distanceInnerRect.setSize(Vector2f((c.distance.at(currentTime) / 42195.f) * length, 40));
				distanceInnerRect.setFillColor(Color(255, 70, 85));

				rightText.setString(std::to_string(c.distance.at(currentTime)) + "m");

				image.setTexture(run, true);
				image.setPosition(leftMargin + (c.distance.at(currentTime) / 42195.f) * length - image.getLocalBounds().width / 4.f + 20, height + 138);
			}

			int offsetY = scroll * heightSpacing;
			distanceOuterRect.move(0, offsetY);
			distanceInnerRect.move(0, offsetY);
			image.move(0, offsetY);
			leftText.move(0, offsetY);
			rightText.move(0, offsetY);

			window.draw(distanceOuterRect);
			window.draw(distanceInnerRect);
			window.draw(image);
			window.draw(leftText);
			window.draw(rightText);

			height += heightSpacing;
		}

		window.draw(titreBackground);
		window.draw(titre);

		Text timer(timeToString(currentTime), DINNextBold, 20);
		timer.setPosition((windowWidth - timer.getLocalBounds().width) / 2, windowHeight - 30);
		timer.setFillColor(Color(15, 25, 35));

		RectangleShape timerBarInnerRect(Vector2f(((float)currentTime / totalTime) * timerBarOuterRect.getLocalBounds().width - 2, 4));
		timerBarInnerRect.setPosition(timerBarOuterRect.getPosition().x + 1, windowHeight - 39);
		timerBarInnerRect.setFillColor(Color(236, 232, 225));

		int mycount = std::count_if(coureurs.begin(), coureurs.end(), [&](Coureur const& c) {
			return c.distance.at(min(currentTime, c.distance.size() - 1)) >= 42125 || (c.abandon && c.distance.size() <= currentTime);
			});
		Text compteur(std::to_string(mycount) + "/100", DINNextBold, 20);
		compteur.setPosition(windowWidth - compteur.getLocalBounds().width - 10, windowHeight - 30);
		compteur.setFillColor(Color(15, 25, 35));

		Text vitesse("Vitesse : x" + std::to_string(speed), DINNextBold, 20);
		vitesse.setPosition(10, windowHeight - 30);
		vitesse.setFillColor(Color(15, 25, 35));

		window.draw(timerBackground);
		window.draw(timer);
		window.draw(timerBarOuterRect);
		window.draw(timerBarInnerRect);
		window.draw(play_pause);
		window.draw(forwardSpr);
		window.draw(backwardSpr);
		window.draw(endSpr);
		window.draw(beginSpr);
		window.draw(compteur);
		window.draw(vitesse);

		window.display();
	}
}

std::string timeToString(int const& secondes) {
	std::stringstream ss;
	ss << std::setw(2) << std::setfill('0') << std::to_string(secondes / 3600) << "h";
	ss << std::setw(2) << std::setfill('0') << std::to_string(secondes / 60 % 60) << "m";
	ss << std::setw(2) << std::setfill('0') << std::to_string(secondes % 60) << "s";
	return ss.str();
}

float Timer::getTime() const {
	float currentPauseDuration = 0;
	if (!playing) currentPauseDuration += clock.getElapsedTime().asSeconds() - startPause;
	return (clock.getElapsedTime().asSeconds() - pauseDuration - currentPauseDuration) * speed + delta;
}

void Timer::pause() {
	if (playing) {
		startPause = clock.getElapsedTime().asSeconds();
		playing = false;
	}
}

void Timer::play() {
	if (!playing) {
		pauseDuration += clock.getElapsedTime().asSeconds() - startPause;
		playing = true;
	}
}

void Timer::setSpeed(int value) {
	if (playing) {
		delta = getTime();
		speed = value;
		clock = sf::Clock();
		pauseDuration = 0;
	}
}

void Timer::addTime(int delta) {
	if (getTime() + delta >= 0 && getTime() + delta < maxTime)
		this->delta += delta;
	else if (getTime() + delta >= maxTime)
		this->delta += maxTime - getTime();
	else
		this->delta += -getTime();
}

bool Timer::getState() {
	return playing;
}