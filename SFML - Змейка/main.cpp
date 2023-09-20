#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <windows.h>
#include <random>
#include <fstream>
#include <math.h>

using namespace sf;

int r_num_gen(int minN, int maxN);

void gameMenu(RenderWindow& mainWindow);
int gameProcess(RenderWindow& mainWindow);
bool gameOver(RenderWindow& mainWindow, int snakeLength);

std::string versionStr = "1.2";

class Player {
private:
	Texture* textures = new Texture[100];
	Sprite* body = new Sprite[100];

	Vector2i position[100];
	Vector2i oldPosition[100];

	int snakeLength;
	int speed;
	int actualDirection;

public:
	Player() {
		textures[0].loadFromFile("Resources/textures/snake/head.png");
		position[0] = { 510, 140 };
		snakeLength = 0;
		speed = 100;
		actualDirection = 3;

		body[0].setTexture(textures[0]);
		body[0].setPosition((Vector2f)position[0]);
		body[0].setScale(10, 10);
		body[0].setRotation(270);
		body[0].setOrigin(5, 5);
	}

	void setPosition(int num, Vector2i newPosition) {
		position[num] = newPosition;
	}

	Vector2i getPosition(int num) {
		return position[num];
	}

	int getSnakeLength() {
		return snakeLength;
	}

	void addLength() {
		snakeLength++;
		position[snakeLength] = position[snakeLength - 1];
		textures[snakeLength].loadFromFile("Resources/textures/snake/tail.png");
		body[snakeLength].setTexture(textures[snakeLength]);
		body[snakeLength].setScale(10, 10);
		body[snakeLength].setRotation(body[snakeLength - 1].getRotation());
		body[snakeLength].setPosition((Vector2f)position[snakeLength]);
		body[snakeLength].setOrigin(5, 5);

		if (snakeLength > 1)
			textures[snakeLength - 1].loadFromFile("Resources/textures/snake/tail.png");
		else if (snakeLength == 1) {
			if (actualDirection == 0)
				position[1].y += speed;
			else if (actualDirection == 1)
				position[1].x += speed;
			else if (actualDirection == 2)
				position[1].y -= speed;
			else if (actualDirection == 3)
				position[1].x -= speed;

			body[1].setPosition((Vector2f)position[1]);
		}
	}
	void move() {
		for (int i = 0; i < snakeLength; i++) {
			oldPosition[i] = position[i];
		}

		float counter = 0;
		int nextActualDirection = actualDirection;

		Clock clock;
		Time dt = clock.getElapsedTime();
		float dtAsSeconds = dt.asSeconds();

		while (dtAsSeconds < 0.3) {
			if ((position[0].x - 510) % 100 == 0 && (position[0].y - 140) % 100 == 0) {
				if ((Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up)) && actualDirection != 2)
					nextActualDirection = 0;
				if ((Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left)) && actualDirection != 3)
					nextActualDirection = 1;
				if ((Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down)) && actualDirection != 0)
					nextActualDirection = 2;
				if ((Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)) && actualDirection != 1)
					nextActualDirection = 3;
			}

			dt = clock.getElapsedTime();
			dtAsSeconds = dt.asSeconds();
		}
		actualDirection = nextActualDirection;

		if (actualDirection == 0) {
			body[0].setRotation(0);
			position[0].y -= speed;
		}
		else if (actualDirection == 1) {
			body[0].setRotation(270);
			position[0].x -= speed;
		}
		else if (actualDirection == 2) {
			body[0].setRotation(180);
			position[0].y += speed;
		}
		else if (actualDirection == 3) {
			body[0].setRotation(90);
			position[0].x += speed;
		}

		if (position[0].x == 1510)
			position[0].x = 510;
		if (position[0].x == 410)
			position[0].x = 1410;
		if (position[0].y == 1040)
			position[0].y = 140;
		if (position[0].y == 40)
			position[0].y = 940;

		body[0].setPosition((Vector2f)position[0]);
		if (snakeLength > 0) {
			for (int i = 1; i < snakeLength + 1; i++) {
				position[i] = oldPosition[i - 1];
				body[i].setPosition((Vector2f)position[i]);
			}
			for (int i = 1; i < snakeLength + 1; i++) {
				if (position[i].x != position[i - 1].x) {
					if (position[i].x > position[i - 1].x)
						body[i].setRotation(270);
					if (position[i].x < position[i - 1].x)
						body[i].setRotation(90);
					textures[i].loadFromFile("Resources/textures/snake/body_1.png");
				}
				if (position[i].y != position[i - 1].y) {
					if (position[i].y < position[i - 1].y)
						body[i].setRotation(180);
					if (position[i].y > position[i - 1].y)
						body[i].setRotation(0);
					textures[i].loadFromFile("Resources/textures/snake/body_1.png");
				}

				bool isClockwise = true;
				if (position[i - 1].x != position[i + 1].x && position[i - 1].y != position[i + 1].y) {
					textures[i].loadFromFile("Resources/textures/snake/body_2.png");
					int rtA = 0;
					if (position[i - 1].x < position[i + 1].x && position[i - 1].y > position[i + 1].y) {
						if ((position[i].x == position[i - 1].x && position[i].x < position[i + 1].x) && (position[i].y < position[i - 1].y && position[i].y == position[i + 1].y)) {
							body[i].setRotation(0);
							rtA += 270;
						}
						if ((position[i].x > position[i - 1].x && position[i].x == position[i + 1].x) && (position[i].y == position[i - 1].y && position[i].y > position[i + 1].y)) {
							body[i].setRotation(180);
							rtA += 90;
							isClockwise = false;
						}
					}
					if (position[i - 1].x < position[i + 1].x && position[i - 1].y < position[i + 1].y) {
						if ((position[i].x == position[i - 1].x && position[i].x < position[i + 1].x) && (position[i].y > position[i - 1].y && position[i].y == position[i + 1].y)) {
							body[i].setRotation(270);
							rtA += 90;
						}
						if ((position[i].x > position[i - 1].x && position[i].x == position[i + 1].x) && (position[i].y == position[i - 1].y && position[i].y < position[i + 1].y)) {
							body[i].setRotation(90);
							rtA -= 90;
							isClockwise = false;
						}
					}
					if (position[i - 1].x > position[i + 1].x && position[i - 1].y < position[i + 1].y) {
						if ((position[i].x < position[i - 1].x && position[i].x == position[i + 1].x) && (position[i].y == position[i - 1].y && position[i].y < position[i + 1].y)) {
							body[i].setRotation(0);
							rtA += 90;
							isClockwise = false;
						}
						if ((position[i].x == position[i - 1].x && position[i].x > position[i + 1].x) && (position[i].y > position[i - 1].y && position[i].y == position[i + 1].y)) {
							body[i].setRotation(180);
							rtA -= 90;
						}
					}
					if (position[i - 1].x > position[i + 1].x && position[i - 1].y > position[i + 1].y) {
						if ((position[i].x < position[i - 1].x && position[i].x == position[i + 1].x) && (position[i].y == position[i - 1].y && position[i].y > position[i + 1].y)) {
							body[i].setRotation(270);
							rtA -= 90;
							isClockwise = false;
						}
						if ((position[i].x == position[i - 1].x && position[i].x > position[i + 1].x) && (position[i].y < position[i - 1].y && position[i].y == position[i + 1].y)) {
							body[i].setRotation(90);
							rtA += 90;
						}
					}

					bool isAfterTeleport1 = false;
					bool isBeforeTeleport1 = false;

					int rt = body[i].getRotation();
					if (position[i].x == position[i - 1].x && std::fabs(position[i].y - position[i - 1].y) != 100) {
						rt += rtA;
						isAfterTeleport1 = true;
					}
					if (position[i].y == position[i - 1].y && std::fabs(position[i].x - position[i - 1].x) != 100) {
						rt += rtA;
						isAfterTeleport1 = true;
					}

					if (position[i].x == position[i + 1].x && std::fabs(position[i].y - position[i + 1].y) != 100) {
						rt += rtA * -1;
						isBeforeTeleport1 = true;
					}
					if (position[i].y == position[i + 1].y && std::fabs(position[i].x - position[i + 1].x) != 100) {
						rt += rtA * -1;
						isBeforeTeleport1 = true;
					}

					if (isAfterTeleport1 && isBeforeTeleport1) {
						rt += 180;
					}
					body[i].setRotation(rt);
				}

				if (i == snakeLength) {
					textures[i].loadFromFile("Resources/textures/snake/tail.png");
					if ((position[i].x == position[i - 1].x && std::fabs(position[i].y - position[i - 1].y) != 100) ||
						(position[i].y == position[i - 1].y && std::fabs(position[i].x - position[i - 1].x) != 100)) {
						int rt = body[i].getRotation();
						rt += 180;
						body[i].setRotation(rt);
					}
				}
			}
		}
	}
	int getRotationOfBlock(int num) {
		return body[num].getRotation();
	}

	void draw(RenderWindow& mainWindow) {
		for (int i = 0; i < snakeLength + 1; i++) {
			mainWindow.draw(body[i]);
		}
	}
};

class Apple {
private:
	Texture texture;
	Sprite body;
	Vector2i position;

	std::string fileNamesOfTextures[2];

public:
	Apple() {
		fileNamesOfTextures[0] = "Resources/textures/items/apple_1.png";
		fileNamesOfTextures[1] = "Resources/textures/items/apple_2.png";

		texture.loadFromFile(fileNamesOfTextures[r_num_gen(0, 2)]);
		body.setTexture(texture);
		body.setOrigin(5, 5);
		body.setScale(10, 10);

		Vector2i proposedPosition;
		bool notGoodPosition = true;

		while (notGoodPosition) {
			proposedPosition = { r_num_gen(5, 15) * 100 + 10, r_num_gen(1, 10) * 100 + 40 };
			if (proposedPosition.x == 510 && proposedPosition.y == 140)
				notGoodPosition = true;
			else {
				position = proposedPosition;
				notGoodPosition = false;
			}
		}

		body.setPosition((Vector2f)position);
	}

	Vector2i getPosition() {
		return position;
	}

	void newApple(Vector2i* positionsOfSnakeParts, int snakeLength) {
		Vector2i proposedPosition;
		bool goodPosition = false;
		int equalPositionsCounter = 0;

		texture.loadFromFile(fileNamesOfTextures[r_num_gen(0, 2)]);

		while (!goodPosition) {
			equalPositionsCounter = 0;
			proposedPosition = { r_num_gen(5, 15) * 100 + 10, r_num_gen(1, 10) * 100 + 40 };
			for (int i = 0; i < snakeLength + 1; i++) {
				if (proposedPosition == positionsOfSnakeParts[i])
					equalPositionsCounter++;
			}
			if (equalPositionsCounter == 0)
				goodPosition = true;
		}

		position = proposedPosition;
		body.setPosition((Vector2f)position);
	}
	void draw(RenderWindow& mainWindow) {
		mainWindow.draw(body);
	}
};

class TextMessage {
private:
	std::string textStr;

	Text text;
	Vector2f position;
	Font font;

public:
	TextMessage() {
		textStr = "MESSAGE";

		font.loadFromFile("Resources/font/minecraft.ttf");
		text.setCharacterSize(50);
		text.setFont(font);
		text.setString(textStr);
		text.setPosition({ 0, 0 });
		text.setFillColor(Color::White);
	}

	void setPosition(Vector2i newPosition, bool isCenterTheFieldByX, bool isCenterTheFieldByY) {
		Vector2f centerOfTextMessage = { text.getLocalBounds().width / 2.f, text.getLocalBounds().height / 2.f };
		text.setOrigin(centerOfTextMessage);

		if (isCenterTheFieldByX || isCenterTheFieldByY) {
			Vector2f centerOfTextMessage = { text.getLocalBounds().width / 2.f, text.getLocalBounds().height / 2.f };
			text.setOrigin(centerOfTextMessage);

			if (isCenterTheFieldByX) {
				newPosition.x = VideoMode::getDesktopMode().width / 2;
			}
			if (isCenterTheFieldByY) {
				newPosition.y = VideoMode::getDesktopMode().height / 2;
			}
		}

		text.setPosition((Vector2f)newPosition);
	}
	void setText(std::string newTextStr) {
		textStr = newTextStr;
		text.setString(textStr);
	}
	void setCharacterSize(int newSize) {
		text.setCharacterSize(newSize);
	}
	void draw(RenderWindow& mainWindow) {
		mainWindow.draw(text);
	}
};

class MyCursor {
private:
	Texture txtr;
	Sprite sprt;
	Vector2f position;

public:
	MyCursor() {
		position = (Vector2f)Mouse::getPosition();
		txtr.loadFromFile("Resources/textures/interface/cursor.png");
		sprt.setTexture(txtr);
		sprt.setScale(5, 5);
		sprt.setPosition(position);
	}

	void draw(RenderWindow& mainWindow) {
		position = (Vector2f)Mouse::getPosition();
		sprt.setPosition(position);
		mainWindow.draw(sprt);
	}
};

class Button {
private:
	Vector2f position;
	RectangleShape body;
	Text text;
	Font font;
	std::string textStr;
	int characterSize;
	Vector2f sizeOfBody;

	bool isHovered;

public:
	Button(float setablePosition_y, std::string setableTextStr) {
		// this button is always centered
		isHovered = false;
		position.y = setablePosition_y; // text [y] is here
		textStr = setableTextStr;
		characterSize = 60;
		font.loadFromFile("Resources/font/minecraft.ttf");

		text.setFillColor(Color::White);
		text.setFont(font);
		text.setCharacterSize(characterSize);
		text.setString(textStr);
		text.setOrigin({ text.getLocalBounds().width / 2, text.getLocalBounds().height / 2 });
		position.x = VideoMode::getDesktopMode().width / 2; // text [x] is here
		text.setPosition(position);

		body.setFillColor(Color::Black);
		body.setOutlineThickness(5);
		body.setOutlineColor(Color::White);
		sizeOfBody.x = text.getLocalBounds().width + 10 + 20; // 10 - outline thickness * 2, 20 - standart indent * 2
		sizeOfBody.y = text.getLocalBounds().height + 10 + 10;
		body.setSize(sizeOfBody);
		body.setOrigin({ sizeOfBody.x / 2, sizeOfBody.y / 2 - 3 });
		body.setPosition(position);
	}

	bool isClicked() {
		if (isHovered && Mouse::isButtonPressed(Mouse::Left)) {
			return 1;
		}
		else {
			return 0;
		}
	}

	void draw(RenderWindow& mainWindow) {
		Vector2f mousePos = (Vector2f)Mouse::getPosition();
		Vector2f upperLeftCornerPos = { position.x - sizeOfBody.x / 2, position.y - sizeOfBody.y / 2 };

		if (
			(mousePos.x >= upperLeftCornerPos.x && mousePos.x <= upperLeftCornerPos.x + sizeOfBody.x)
			&&
			((mousePos.y >= upperLeftCornerPos.y && mousePos.y <= upperLeftCornerPos.y + sizeOfBody.y))
			) {
			isHovered = true;
		}
		else {
			isHovered = false;
		}

		if (isHovered) {
			body.setFillColor(Color::White);
			body.setOutlineColor(Color::Black);
			text.setFillColor(Color::Black);
		}
		else {
			body.setFillColor(Color::Black);
			body.setOutlineColor(Color::White);
			text.setFillColor(Color::White);
		}

		mainWindow.draw(body);
		mainWindow.draw(text);
	}
};

class Background {
private:
	//Vector2i positions[90];
	//Texture textures[90];
	//Sprite sprites[90];

	Vector2i* positions = new Vector2i[90];
	Texture* textures = new Texture[90];
	Sprite* sprites = new Sprite[90];

	std::string fileNamesOfTextures[6];

public:
	Background() {
		fileNamesOfTextures[0] = "Resources/textures/backgrounds/bg_flower_1.png";
		fileNamesOfTextures[1] = "Resources/textures/backgrounds/bg_flower_2.png";
		fileNamesOfTextures[2] = "Resources/textures/backgrounds/bg_grass_1.png";
		fileNamesOfTextures[3] = "Resources/textures/backgrounds/bg_rock_1.png";
		fileNamesOfTextures[4] = "Resources/textures/backgrounds/bg_rock_2.png";
		fileNamesOfTextures[5] = "Resources/textures/backgrounds/bg_rock_3.png";

		/////////////////////////////
		// GENERATE BACKGROUND POSITIONS
		/////////////////////////////

		int counterX = 0;
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 10; j++) {
				positions[counterX].x = j * 100 + 510;
				counterX++;
			}
		}

		int counterY = 0;
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 10; j++) {
				positions[counterY].y = i * 100 + 140;
				counterY++;
			}
		}

		/////////////////////////////

		for (int i = 0; i < 90; i++) {
			int randomChoice = r_num_gen(0, 7);

			if (randomChoice == 6)
				randomChoice = 2;

			textures[i].loadFromFile(fileNamesOfTextures[randomChoice]);
			sprites[i].setTexture(textures[i]);
			sprites[i].setPosition((Vector2f)positions[i]);
			sprites[i].setOrigin(5, 5);
			sprites[i].setScale(10, 10);

			int numOfRotation = r_num_gen(0, 4);
			switch (numOfRotation) {
			case 0:
				sprites[i].setRotation(0);
				break;
			case 1:
				sprites[i].setRotation(90);
				break;
			case 2:
				sprites[i].setRotation(180);
				break;
			case 3:
				sprites[i].setRotation(270);
				break;
			}
		}
	}

	void draw(RenderWindow& mainWindow) {
		for (int i = 0; i < 90; i++) {
			mainWindow.draw(sprites[i]);
		}
	}
};

//////////////////////// EVENT ////////////////////////
Event event;
////////////////////////

//////////////////////// SOUND`s ////////////////////////
Music music;

SoundBuffer soundBuffer;
Sound sound;
////////////////////////

int main() {
	RenderWindow mainWindow;
	mainWindow.create(VideoMode(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height), "Snake Game", Style::Fullscreen);
	mainWindow.setMouseCursorVisible(false);

	sound.setBuffer(soundBuffer);
	sound.setVolume(100.f);

	int snakeLength;
	bool answerFromGameOver = false;

	gameMenu(mainWindow);
	while (mainWindow.isOpen()) {
		snakeLength = gameProcess(mainWindow);
		if (snakeLength == -1) {
			break;
		}
		answerFromGameOver = gameOver(mainWindow, snakeLength);
		if (answerFromGameOver) {
			break;
		}
	}
	return 0;
}

int r_num_gen(int minN, int maxN) {
	std::random_device rd;
	std::mt19937 mersenne(rd());
	return ((mersenne() % (maxN - minN)) + minN);
}

void gameMenu(RenderWindow& mainWindow) {
	//////////////////////// MAIN MENU ////////////////////////
	music.openFromFile("Resources/sounds/game process/gameProcessMusic_1.ogg");
	music.setLoop(true);
	music.play();

	bool inMenu = true;

	MyCursor cursor;

	Vector2f playButtonPos = { 750, 700 };

	Texture txtrMenuBg;
	Texture txtrPlayButton_off;
	Texture txtrPlayButton_on;

	txtrMenuBg.loadFromFile("Resources/textures/mainMenu/mainMenu.png");
	txtrPlayButton_off.loadFromFile("Resources/textures/mainMenu/mainMenuPlayButton_off.png");
	txtrPlayButton_on.loadFromFile("Resources/textures/mainMenu/mainMenuPlayButton_on.png");

	Sprite sprtMenuBg;
	Sprite sprtPlayButton_off;
	Sprite sprtPlayButton_on;

	sprtMenuBg.setTexture(txtrMenuBg);
	sprtPlayButton_off.setTexture(txtrPlayButton_off);
	sprtPlayButton_on.setTexture(txtrPlayButton_on);

	sprtMenuBg.setScale(10, 10);
	sprtPlayButton_off.setScale(10, 10);
	sprtPlayButton_on.setScale(10, 10);

	sprtMenuBg.setPosition(0, 0);
	sprtPlayButton_off.setPosition(playButtonPos);
	sprtPlayButton_on.setPosition(playButtonPos);

	Text verT;
	Font verF;

	verF.loadFromFile("Resources/font/minecraft.ttf");

	verT.setCharacterSize(30);
	verT.setFillColor(Color::Red);
	verT.setPosition(10, 1000);
	verT.setFont(verF);
	verT.setString("v" + versionStr);

	while (inMenu) {
		mainWindow.clear(Color::Black);

		mainWindow.draw(sprtMenuBg);


		if (
			(Mouse::getPosition().x >= playButtonPos.x && Mouse::getPosition().x <= playButtonPos.x + 420)
			&&
			(Mouse::getPosition().y >= playButtonPos.y && Mouse::getPosition().y <= playButtonPos.y + 200)
			) {
			mainWindow.draw(sprtPlayButton_on);

			while (mainWindow.pollEvent(event)) {
				if (event.key.code == Mouse::Left) {
					if (Mouse::isButtonPressed(Mouse::Left)) {
						music.stop();
						inMenu = false;
					}
				}
			}
		}
		else {
			mainWindow.draw(sprtPlayButton_off);
		}
		mainWindow.draw(verT);
		cursor.draw(mainWindow);

		mainWindow.display();

		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			music.stop();
			mainWindow.close();
			return;
		}
	}
}

int gameProcess(RenderWindow &mainWindow) {

	Player player;
	Apple apple;
	Background bgGame;

	bool b_gameOver = false;
	std::string dStr;

	Texture bgTxtr;
	Sprite bgSide;

	bgTxtr.loadFromFile("Resources/textures/backgrounds/bg_side.png");
	bgSide.setTexture(bgTxtr);
	bgSide.setScale(10, 10);
	bgSide.setPosition(0, 0);

	Texture integerTxtr[2];
	Sprite score[2];
	std::string nameOfFiles[2];

	score[0].setScale(10, 10);
	score[1].setScale(10, 10);
	score[0].setPosition({ 780, 20 });
	score[1].setPosition({ 820, 20 });

	while (mainWindow.isOpen()) {
		player.move(); // just move player

		////////////////////////////////////////// take me to class Background!

		if (player.getSnakeLength() < 10) {
			nameOfFiles[0] = "Resources/textures/integers/u_0.png";
			nameOfFiles[1] = "Resources/textures/integers/" + std::to_string(player.getSnakeLength()) + ".png";
		}
		else {
			nameOfFiles[0] = "Resources/textures/integers/" + std::to_string(player.getSnakeLength() / 10) + ".png";
			nameOfFiles[1] = "Resources/textures/integers/" + std::to_string(player.getSnakeLength() % 10) + ".png";
		}

		integerTxtr[0].loadFromFile(nameOfFiles[0]);
		integerTxtr[1].loadFromFile(nameOfFiles[1]);

		score[0].setTexture(integerTxtr[0]);
		score[1].setTexture(integerTxtr[1]);

		//////////////////////////////////////////

		if (apple.getPosition() == player.getPosition(0)) { // if eat an apple
			player.addLength();
			std::string nameOfSoundFile = "Resources/sounds/eat apple/eatApple_sound_1.ogg";
			soundBuffer.loadFromFile(nameOfSoundFile);
			sound.play();

			Vector2i* positionsOfSnakeParts = new Vector2i[100];
			for (int i = 0; i < player.getSnakeLength() + 1; i++) {
				positionsOfSnakeParts[i] = player.getPosition(i);
			}
			apple.newApple(positionsOfSnakeParts, player.getSnakeLength());
		}

		for (int i = 1; i < player.getSnakeLength() + 1; i++) {
			if (player.getPosition(0) == player.getPosition(i)) { // if player head crushed into the body
				b_gameOver = true;
			}
		}

		mainWindow.clear(Color::Black);

		bgGame.draw(mainWindow); // draw background
		apple.draw(mainWindow); // draw apple
		if (!b_gameOver)
			player.draw(mainWindow); // draw player (all snake)
		mainWindow.draw(bgSide); // del me, or not
		mainWindow.draw(score[0]);
		mainWindow.draw(score[1]);

		mainWindow.display();

		if (Keyboard::isKeyPressed(Keyboard::Escape)) { // to exit
			mainWindow.close();
			return -1;
		}

		if (b_gameOver) {
			std::string nameOfSoundFile = "Resources/sounds/game over/gameOver_sound_1.ogg";
			soundBuffer.loadFromFile(nameOfSoundFile);
			sound.play();

			int blackoutColorAlpha = 0;

			Texture deadTxtr;
			Sprite deadSprt;

			deadTxtr.loadFromFile("Resources/textures/interface/dead.png");
			deadSprt.setTexture(deadTxtr);
			deadSprt.setOrigin(5, 5);
			deadSprt.setPosition((Vector2f)player.getPosition(1));
			deadSprt.setScale(15, 15);

			RectangleShape blackout;
			blackout.setPosition(0, 0);
			blackout.setSize({ (float)VideoMode::getDesktopMode().width, (float)VideoMode::getDesktopMode().height });
			blackout.setFillColor(Color(0, 0, 0, 0));

			for (int i = 0; i < 255; i++) {
				Clock clock;
				Time dt = clock.getElapsedTime();
				float dtAsSeconds = dt.asSeconds();

				blackout.setFillColor(Color(0, 0, 0, i));

				mainWindow.draw(deadSprt);
				mainWindow.draw(blackout);
				mainWindow.display();

				while (dtAsSeconds < 0.02) {
					dt = clock.getElapsedTime();
					dtAsSeconds = dt.asSeconds();
				}
			}

			return player.getSnakeLength(); // when game over
		}
	}
}

bool gameOver(RenderWindow& mainWindow, int snakeLength) {

	MyCursor cursor;

	Button button_restart(700, "RESTART"); // change me!
	Button button_exit(820, "EXIT"); // change me!
	Button button_reset(940, "RESET HIGH SCORE"); // change me!

	//////////////////////// TAKE A HIGTH SCORE ////////////////////////
	std::string hightScore;
	std::ifstream in("Resources/data/highScore.txt");
	std::getline(in, hightScore);
	in.close();
	////////////////////////

	//////////////////////// CREATING A BACKGROUND ////////////////////////
	RectangleShape bg;
	bg.setPosition({ 0, 0 });
	bg.setFillColor(Color::Black);
	bg.setSize({ (float)VideoMode::getDesktopMode().width, (float)VideoMode::getDesktopMode().height });
	////////////////////////

	//////////////////////// MESSAGE ////////////////////////
	TextMessage gameOverMessage_title;
	TextMessage gameOverMessage_score;
	TextMessage gameOverMessage_congratulation;

	gameOverMessage_title.setCharacterSize(100);
	gameOverMessage_score.setCharacterSize(50);
	gameOverMessage_congratulation.setCharacterSize(80);

	gameOverMessage_title.setText("GAME OVER!");
	gameOverMessage_score.setText("Score: " + std::to_string(snakeLength) + "\n\nHigh score: " + hightScore);
	gameOverMessage_congratulation.setText("NEW HIGH SCORE!");

	gameOverMessage_title.setPosition({ 0, 100 }, true, false);
	gameOverMessage_score.setPosition({ 0, 300 }, true, false);
	gameOverMessage_congratulation.setPosition({ 0, 500 }, true, false);

	if (snakeLength > std::stoi(hightScore)) {
		std::ofstream out;
		out.open("Resources/data/highScore.txt");
		out << snakeLength;
		out.close();
	}
	////////////////////////

	//////////////////////// CYCLE ////////////////////////
	while (1) {
		mainWindow.clear(Color::Black);

		mainWindow.draw(bg);
		gameOverMessage_title.draw(mainWindow);
		gameOverMessage_score.draw(mainWindow);
		if (snakeLength > std::stoi(hightScore)) {
			if (sound.getStatus() != 2) {
				std::string nameOfSoundFile = "Resources/sounds/new higth score/newHigthScore_sound_1.ogg";
				soundBuffer.loadFromFile(nameOfSoundFile);
				sound.play();
			}
			gameOverMessage_congratulation.draw(mainWindow);
		}
		button_restart.draw(mainWindow);
		button_exit.draw(mainWindow);
		button_reset.draw(mainWindow);

		cursor.draw(mainWindow);
		mainWindow.display();



		while (mainWindow.pollEvent(event)) {
			if (event.key.code == Mouse::Left) {
				if (button_restart.isClicked()) {
					sound.stop();
					return 0;
				}
				if (button_exit.isClicked()) {
					sound.stop();
					return 1;
				}
				if (button_reset.isClicked()) {
					std::ofstream out;
					out.open("Resources/data/highScore.txt");
					out << "0";
					out.close();
				}
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Enter)) {
			sound.stop();
			return 0;
		}
	}
	////////////////////////
}