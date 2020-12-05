#include <string>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <Cassert>
#include "windows.h"
#include <cmath> 


using namespace std;

struct Vector2f
{
	float x;
	float y;

	Vector2f()
	{
		x = 0;
		y = 0;
	}

	Vector2f(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	Vector2f operator+ (Vector2f v)
	{
		return Vector2f(x + v.x, y + v.y);
	}

	Vector2f operator- (Vector2f v)
	{
		return Vector2f(x - v.x, y - v.y);
	}

	Vector2f operator* (float n)
	{
		return Vector2f(n * x, n * y);
	}

	Vector2f operator/ (float n)
	{
		return Vector2f(x / n, y / n);
	}

	float scalar(Vector2f v)
	{
		return  x * v.x + y * v.y;
	}

	float length()
	{
		return sqrt(x * x + y * y);
	}

	Vector2f normalize()
	{
		float l = this->length();
		return Vector2f(x / l, y / l);
	}

	float angle(Vector2f u)
	{
		return((atan((u.y - y) / (u.x - x))) * 180 / 3.14);
	}

};

struct Sphere
{
public:

	Vector2f position;
	Vector2f velocity;
	Vector2f acceleration;
	int radius;
	int red;
	int green;
	int blue;
	int m;

	Sphere()
	{
		position = { 0, 0 };
		velocity = { 0, 0 };
		acceleration = { 0, 0 };
		radius = 0;
		red = 0;
		green = 0;
		blue = 0;
		m = 0;
	}

	Sphere(Vector2f position, Vector2f velocity, Vector2f acceleration, int radius, int red, int green, int blue, int m)
	{
		this->position = position;
		this->velocity = velocity;
		this->acceleration = acceleration;
		this->radius = radius;
		this->red = red;
		this->green = green;
		this->blue = blue;
		this->m = m;
	}


	void drawSphere(sf::RenderWindow* myWindow)
	{
		int numberofCircles = 10;
		float x_centerofCircles = position.x;
		float y_centerofCircles = position.y;
		sf::CircleShape circle(0, 30);
		for (int i = 0; i < numberofCircles; i++)
		{
			circle.setRadius(radius - i * radius / numberofCircles);
			circle.setFillColor(sf::Color(i * red / numberofCircles, i * green / numberofCircles, i * blue / numberofCircles));
			circle.setPosition(position.x + (position.x - x_centerofCircles) * i / numberofCircles - radius + radius * i / numberofCircles, position.y + (position.y - y_centerofCircles) * i / numberofCircles - radius + radius * i / numberofCircles);
			myWindow->draw(circle);
		}
	}


	void moveSphere(float t)
	{
		position = position + velocity * t;
	}


	bool ballsDangerouslyClose(float x, float y)
	{
		if (sqrt(pow((x - position.x), 2) + pow((y - position.y), 2)) <= 20)
		{
			return true;
		};
		return false;
	};
};

struct Wall
{
public:

	Vector2f position;
	float x;
	float y;

	Wall()
	{
		position = { 0, 0 };
		x = 0;
		y = 0;
	}

	Wall(Vector2f position, float x, float y)
	{
		this->position = position;
		this->x = x;
		this->y = y;
	}

	void drawWall(sf::RenderWindow* myWindow)
	{
		sf::RectangleShape rectangle(sf::Vector2f(x, y));
		rectangle.setFillColor(sf::Color(175, 180, 240));
		rectangle.setPosition(position.x, position.y);
		myWindow->draw(rectangle);
	};

	void collideBulletWithWall(Sphere* sphere)
	{
		if (sphere->position.x >= position.x  and sphere->position.x <= position.x + x)
		{
			if (sphere->position.y >= position.y and sphere->position.y <= position.y + y)
			{
				*sphere = { {-100.f, -100.f} , {0.f, 0.f},  {0, 0}, 10, 0, 0, 255, 10 };
			};
		};
	};

	void collidePlayerWithWall(sf::CircleShape * myPlayer, sf::Vector2f * movement)
	{
		sf::Vector2f pos = myPlayer->getPosition();
		float r = 15;
		if (pos.x + 2 * r >= position.x and pos.x <= position.x + x)
		{
			if (pos.y + 2 * r >= position.y and pos.y <= position.y + y)
			{
				if (DangerouslyClose(pos.x + 2*r, position.x, 15) and movement->x > 0)
				{
					movement->x = 0;
				};

				if (DangerouslyClose(pos.x, position.x + x, 15) and movement->x < 0)
				{
					movement->x = 0;
				};

				if (DangerouslyClose(pos.y + 2*r, position.y, 15) and movement->y > 0)
				{
					movement->y = 0;
				};

				if (DangerouslyClose(pos.y, position.y + y, 15) and movement->y < 0)
				{
					movement->y = 0;
				};
			};
		};
	};

	bool DangerouslyClose(float x, float y, float r)
	{
		if (sqrt(pow((x - y), 2)) < r)
		{
			return true;
		};

		return false;
	};

	float normalize(float n)
	{
		if (n == 0)
		{
			return 0;
		};

		return n / n;
	};
};



class Game
{
public:
	Game();
	void run();

private:
	void ProcessEvents();
	void update();
	void updateMap();
	void render();
	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
	void gameOver(int one);
	void repeat();
	void health(int, int, int, int, int);
	void updateBullet();
	void poolBullet();
private:
	sf::RenderWindow myWindow;
	sf::CircleShape myPlayer1; sf::CircleShape myPlayer2;
	bool movingLeft1 = 0, movingUp1 = 0, movingRight1 = 0, movingDown1 = 0, shot1 = 0;
	bool movingLeft2 = 0, movingUp2 = 0, movingRight2 = 0, movingDown2 = 0, shot2 = 0;

	sf::Vector2f movement1; sf::Vector2f movement2;
	Sphere spheres1[10]; Sphere spheres2[10];

	int i = 0; int g = 0; float t = 0.1; int gv = 1; int sleepgun1 = 0; int sleepgun2 = 0; int numberwall = 22; int numberbullet = 10; int health1 = 3; int health2 = 3;

	string direction1 = { 0,0 }; string direction2 = { 0,0 };
	
	Wall edgemap[22];
	int edge[22][4] = { {0, 0, 1920, 20}, {1900, 0, 20, 1080}, {0, 1060, 1920, 20}, {0, 0, 20, 1080}, {20, 920, 80, 20}, {140, 980, 20, 80 }, {70, 820, 170, 20}, {240, 820, 20, 180},{1800, 150, 100, 20}, {1750, 20, 20, 90 }, {1670, 250, 150, 20}, {1650, 80, 20, 190}, {660, 360, 20, 360}, {1260, 360, 20, 360}, {660, 340, 170, 20}, {1110, 340, 170, 20}, {660, 720, 170, 20}, {1110, 720, 170, 20}, {200, 140, 500, 20}, {200, 140, 20, 400}, {1200, 900, 500, 20}, {1700, 500, 20, 420}};

};

Game::Game() : myWindow(sf::VideoMode(1920, 1080), "myWindow"), myPlayer1(), myPlayer2()
{
	myPlayer1.setRadius(15.f);
	myPlayer1.setFillColor(sf::Color::Red);
	myPlayer1.setPosition(60.f, 990.f);

	myPlayer2.setRadius(15.f);
	myPlayer2.setFillColor(sf::Color::Blue);
	myPlayer2.setPosition(1840.f, 50.f);

	poolBullet();

	for (int m = 0; m < numberwall; m++)
	{
		edgemap[m] = {{float(edge[m][0]), float(edge[m][1]) }, float(edge[m][2]), float(edge[m][3])};
    };

};

void Game::run()
{

	while (myWindow.isOpen())
	{
		ProcessEvents();

		if (gv)
		{
			update();
			render();
		};
	}
}

void Game::ProcessEvents()
{
	sf::Event event;
	while (myWindow.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::KeyPressed: handlePlayerInput(event.key.code, true); break;
		case sf::Event::KeyReleased: handlePlayerInput(event.key.code, false); break;
		case sf::Event::Closed: myWindow.close(); break;
		}
	}
}


void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
	if (key == sf::Keyboard::W) movingUp1 = isPressed, direction1[1] = -1, direction1[0] = 0;
	else if (key == sf::Keyboard::S) movingDown1 = isPressed, direction1[1] = 1, direction1[0] = 0;
	else if (key == sf::Keyboard::A) movingLeft1 = isPressed, direction1[0] = -1, direction1[1] = 0;
	else if (key == sf::Keyboard::D) movingRight1 = isPressed, direction1[0] = 1, direction1[1] = 0;
	else if (key == sf::Keyboard::X) shot1 = isPressed;

	if (key == sf::Keyboard::Up) movingUp2 = isPressed, direction2[1] = -1, direction2[0] = 0;
	else if (key == sf::Keyboard::Down) movingDown2 = isPressed, direction2[1] = 1, direction2[0] = 0;
	else if (key == sf::Keyboard::Left) movingLeft2 = isPressed, direction2[0] = -1, direction2[1] = 0;
	else if (key == sf::Keyboard::Right) movingRight2 = isPressed, direction2[0] = 1, direction2[1] = 0;
	else if (key == sf::Keyboard::RShift) shot2 = isPressed;
	
	if (key == sf::Keyboard::F and gv == 0) repeat();
	
}


void Game::update()
{
	movement1 = { 0.f, 0.f };
	sf::Vector2f pos1(0.f, 0.f);
	if (movingUp1) movement1.y -= 5.f;
	if (movingDown1) movement1.y += 5.f;
	if (movingLeft1) movement1.x -= 5.f;
	if (movingRight1) movement1.x += 5.f;
	if (shot1 and (direction1[0] != 0 or direction1[1] != 0) and sleepgun1 <= 0) { pos1 = myPlayer1.getPosition(); spheres1[i] = { {pos1.x + myPlayer1.getRadius(), pos1.y + myPlayer1.getRadius()} , {float(180 * direction1[0]), float(180 * direction1[1])},  {0, 0}, 10, 255, 0, 0, 10 }; i += 1; sleepgun1 = 30; };
	if (i == numberbullet) i = 0;
	if (sleepgun1 > -1) sleepgun1 -= 1;

	movement2 = { 0.f, 0.f };
	sf::Vector2f pos2(0.f, 0.f);
	if (movingUp2) movement2.y -= 5.f;
	if (movingDown2) movement2.y += 5.f;
	if (movingLeft2) movement2.x -= 5.f;
	if (movingRight2) movement2.x += 5.f;
	if (shot2 and (direction2[0] != 0 or direction2[1] != 0) and sleepgun2 <= 0) { pos2 = myPlayer2.getPosition(); spheres2[g] = { {pos2.x + myPlayer2.getRadius(), pos2.y + myPlayer2.getRadius()} , {float(180 * direction2[0]), float(180 * direction2[1])},  {0, 0}, 10, 0, 0, 255, 10 }; g += 1; sleepgun2 = 30; };
	if (g == numberbullet) g = 0;
    if (sleepgun2 > -1) sleepgun2 -= 1;

	updateBullet();
	updateMap();

	myPlayer1.move(movement1);
	myPlayer2.move(movement2);
}


void Game::updateMap()
{
	for (int i = 0; i < numberwall; i++)
	{
		edgemap[i].collidePlayerWithWall(&myPlayer1, &movement1);
		edgemap[i].collidePlayerWithWall(&myPlayer2, &movement2);

		for (int j = 0; j < numberbullet; j++)
		{
			edgemap[i].collideBulletWithWall(&spheres1[j]);
			edgemap[i].collideBulletWithWall(&spheres2[j]);
		};
	};
};

void Game::updateBullet()
{
	for (int j = 0; j < numberbullet; j++)
	{
		spheres1[j].moveSphere(t);
		spheres2[j].moveSphere(t);

		sf::Vector2f poss2(0.f, 0.f);
		poss2 = myPlayer2.getPosition();
		if (spheres1[j].ballsDangerouslyClose(poss2.x + 15, poss2.y + 15)) { health2 -= 1;  spheres1[j] = { {-100.f, -100.f} , {0.f, 0.f},  {0, 0}, 10, 0, 0, 255, 10 }; };

		sf::Vector2f poss1(0.f, 0.f);
		poss1 = myPlayer1.getPosition();
		if (spheres2[j].ballsDangerouslyClose(poss1.x + 15, poss1.y + 15)) { health1 -= 1; spheres2[j] = { {-100.f, -100.f} , {0.f, 0.f},  {0, 0}, 10, 0, 0, 255, 10 }; };
	};
};


void Game::render()
{
	myWindow.clear();

	sf::Vector2f pos1(0.f, 0.f);
	pos1 = myPlayer1.getPosition();

	sf::Vector2f pos2(0.f, 0.f);
	pos2 = myPlayer2.getPosition();

	for (int a = 0; a < numberwall; a++)
	{
		edgemap[a].drawWall(&myWindow);
	};
    
	health(pos1.x, pos1.y, health1, 0, sleepgun1);
	health(pos2.x, pos2.y, health2, 1, sleepgun2);

	for (int j = 0; j < numberbullet; j++)
	{
		spheres1[j].drawSphere(&myWindow);
		spheres2[j].drawSphere(&myWindow);
	};

	if (gv)
	{
		myWindow.draw(myPlayer1);
		myWindow.draw(myPlayer2);
		myWindow.display();
	};
}


void Game::gameOver(int one)
{
	myWindow.clear();
	sf::Font font;
	font.loadFromFile("CyrilicOld.ttf");
	sf::Text text("Win", font, 30);
	text.setFillColor(sf::Color::Red);
	text.setPosition(800, 300);
	text.setStyle(sf::Text::Bold);

	if (one == 1)
	{
		text.setString("         Win 1 \nPress F to pay respect");
	};

	if (one == 0)
	{
		text.setString("         Win 2 \nPress F to pay respect");
	};

	myWindow.draw(text);
	myWindow.display();

	gv = 0;
};


void Game::repeat()
{
	poolBullet();

	myPlayer1.setPosition(60.f, 990.f);
	myPlayer2.setPosition(1840.f, 50.f);

	health1 = 3; health2 = 3;
	gv = 1;
};


void Game::health(int x, int y, int health, int player, int sleep)
{
	sf::RectangleShape rectangle1(sf::Vector2f(8, 5));
	sf::RectangleShape rectangle2(sf::Vector2f(8, 5));
	sf::RectangleShape rectangle3(sf::Vector2f(8, 5));

	sf::RectangleShape rectangle4(sf::Vector2f(30 - sleep, 5));

	rectangle1.setFillColor(sf::Color(0, 255, 0));
	rectangle2.setFillColor(sf::Color(0, 255, 0));
	rectangle3.setFillColor(sf::Color(0, 255, 0));

	rectangle4.setFillColor(sf::Color(101, 67, 33));

	if (health == 2)
	{
		rectangle1.setFillColor(sf::Color(255, 0, 0));
	};

	if (health == 1)
	{
		rectangle1.setFillColor(sf::Color(255, 0, 0));
		rectangle2.setFillColor(sf::Color(255, 0, 0));
	};

	if (health == 0)
	{
		if (player == 0)
		{
			gameOver(0);
		}

		if (player == 1)
		{
			gameOver(1);
		};
	};

	rectangle1.setPosition(x - 1, y - 10);
	rectangle2.setPosition(x + 10, y - 10);
	rectangle3.setPosition(x + 21, y - 10);

	rectangle4.setPosition(x - 1, y - 20);

	myWindow.draw(rectangle1);
	myWindow.draw(rectangle2);
	myWindow.draw(rectangle3);

	myWindow.draw(rectangle4);
};

void Game::poolBullet()
{
	for (int s = 0; s < numberbullet; s++)
	{
		spheres1[s] = { {-100.f, -100.f} , {0.f, 0.f},  {0, 0}, 10, 0, 0, 255, 10 };
		spheres2[s] = { {-100.f, -100.f} , {0.f, 0.f},  {0, 0}, 10, 0, 0, 255, 10 };
	};
};


int main()
{
	Game game;
	game.run();
	return 0;
}
