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

	Sphere()
	{
		position = { 0, 0 };
		velocity = { 0, 0 };
		acceleration = { 0, 0 };
		radius = 0;
		red = 0;
		green = 0;
		blue = 0;
	}

	Sphere(Vector2f position, Vector2f velocity, Vector2f acceleration, int radius, int red, int green, int blue)
	{
		this->position = position;
		this->velocity = velocity;
		this->acceleration = acceleration;
		this->radius = radius;
		this->red = red;
		this->green = green;
		this->blue = blue;
	}


	void drawSphere(sf::RenderWindow* myWindow)
	{
		sf::CircleShape circle(0, 30);
		circle.setRadius(radius);
		circle.setFillColor(sf::Color(red, green, blue));
		circle.setPosition(position.x - radius, position.y - radius);
		myWindow->draw(circle);
	}

	void drawBullet(sf::RenderWindow* myWindow)
	{
		sf::CircleShape circle(0, 10);
		circle.setRadius(radius);
		circle.setFillColor(sf::Color(red, green, blue));

		for (int i = 0; i < 10; i++)
		{
			circle.setRadius(i * radius/10);
		    circle.setPosition(position.x - i * radius / 10 - i * velocity.x/100, position.y - i * radius / 10 - i * velocity.y/100);
			myWindow->draw(circle);
		};
	};

	void moveSphere(float t)
	{
		position = position + velocity * t;
	}

	void move(Vector2f delta)
	{
		position = position + delta;
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
				*sphere = { {-100.f, -100.f} , {0.f, 0.f},  {0, 0}, 5, 0, 0, 255 };
			};
		};
	};

	void collidePlayerWithWall(Sphere *myPlayer, Vector2f * movement)
	{

		if (myPlayer->position.x + myPlayer->radius >= position.x and myPlayer->position.x - myPlayer->radius <= position.x + x)
		{
			if (myPlayer->position.y >= position.y and myPlayer->position.y <= position.y + y)
			{
				if (DangerouslyClose(myPlayer->position.x + myPlayer->radius, position.x, myPlayer->radius) and movement->x > 0)
				{
					movement->x = 0;
				};

				if (DangerouslyClose(myPlayer->position.x - myPlayer->radius, position.x + x, myPlayer->radius) and movement->x < 0)
				{
					movement->x = 0;
				};

			};
		};
		
		if (myPlayer->position.x >= position.x and myPlayer->position.x <= position.x + x)
		{
			if (myPlayer->position.y + myPlayer->radius >= position.y and myPlayer->position.y - myPlayer->radius <= position.y + y)
			{
				if (DangerouslyClose(myPlayer->position.y + myPlayer->radius, position.y, myPlayer->radius) and movement->y > 0)
				{
					movement->y = 0;
				};

				if (DangerouslyClose(myPlayer->position.y - myPlayer->radius, position.y + y, myPlayer->radius) and movement->y < 0)
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

struct Health
{
public:

	int health;

	Health()
	{
		health = 0;
	}

	Health(int health)
	{
		this->health = health;
	}

	void drawHealth(sf::RenderWindow* myWindow, int x, int y, int sleep)
	{
		sf::RectangleShape rectangle1(sf::Vector2f(8, 5));
		sf::RectangleShape rectangle2(sf::Vector2f(8, 5));
		sf::RectangleShape rectangle3(sf::Vector2f(8, 5));

		sf::RectangleShape rectangle4(sf::Vector2f(30 - sleep * 30 / 70, 5));

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

		rectangle1.setPosition(x - 1, y - 10);
		rectangle2.setPosition(x + 10, y - 10);
		rectangle3.setPosition(x + 21, y - 10);

		rectangle4.setPosition(x - 1, y - 20);

		myWindow->draw(rectangle1);
		myWindow->draw(rectangle2);
		myWindow->draw(rectangle3);

		myWindow->draw(rectangle4);
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
	void handlePlayerInput(sf::Keyboard::Key, bool);
	void gameOver(int);
	void repeat();
	void myHealth(int, int, Health, int, int);
	void updateBullet();
	void poolBullet();
private:
	sf::RenderWindow myWindow;
	Sphere myPlayer1; Sphere myPlayer2;
	bool movingLeft1 = 0, movingUp1 = 0, movingRight1 = 0, movingDown1 = 0, shot1 = 0;
	bool movingLeft2 = 0, movingUp2 = 0, movingRight2 = 0, movingDown2 = 0, shot2 = 0;

	Vector2f movement1; Vector2f movement2;
	Sphere spheres1[10]; Sphere spheres2[10];
	Health healthplayer1; Health healthplayer2;

	int i = 0; int g = 0; float t = 0.1; int gv = 1; int sleepgun1 = 0; int sleepgun2 = 0; int numberwall = 22; int numberbullet = 10;

	string direction1 = { 0,0 }; string direction2 = { 0,0 };
	
	Wall edgemap[22];
	int edge[22][4] = { {0, 0, 1920, 20}, {1900, 0, 20, 1080}, {0, 1060, 1920, 20}, {0, 0, 20, 1080}, {20, 920, 80, 20}, {140, 980, 20, 80 }, {70, 820, 170, 20}, {240, 820, 20, 180},{1800, 150, 100, 20}, {1750, 20, 20, 90 }, {1670, 250, 150, 20}, {1650, 80, 20, 190}, {660, 360, 20, 360}, {1260, 360, 20, 360}, {660, 340, 170, 20}, {1110, 340, 170, 20}, {660, 720, 170, 20}, {1110, 720, 170, 20}, {200, 140, 500, 20}, {200, 140, 20, 400}, {1200, 900, 500, 20}, {1700, 500, 20, 420}};

};

Game::Game() : myWindow(sf::VideoMode(1920, 1080), "myWindow"), myPlayer1(), myPlayer2()
{
	myPlayer1 = { {60, 990} , {0, 0},  {0, 0}, 15, 255, 0, 0 };
	myPlayer2 = { {1840, 50} , {0, 0},  {0, 0}, 15, 0, 0, 255 };;

	poolBullet();

	healthplayer1.health = 3;
	healthplayer2.health = 3;

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
	if (movingUp1) movement1.y -= 3.f;
	if (movingDown1) movement1.y += 3.f;
	if (movingLeft1) movement1.x -= 3.f;
	if (movingRight1) movement1.x += 3.f;
	if (shot1 and (direction1[0] != 0 or direction1[1] != 0) and sleepgun1 <= 0) { spheres1[i] = { {myPlayer1.position.x, myPlayer1.position.y} , {float(120 * direction1[0]), float(120 * direction1[1])},  {0, 0}, 5, 255, 0, 0 }; i += 1; sleepgun1 = 70; };
	if (i == numberbullet) i = 0;
	if (sleepgun1 > -1) sleepgun1 -= 1;

	movement2 = { 0.f, 0.f };
	if (movingUp2) movement2.y -= 3.f;
	if (movingDown2) movement2.y += 3.f;
	if (movingLeft2) movement2.x -= 3.f;
	if (movingRight2) movement2.x += 3.f;
	if (shot2 and (direction2[0] != 0 or direction2[1] != 0) and sleepgun2 <= 0) { spheres2[g] = { {myPlayer2.position.x, myPlayer2.position.y} , {float(120 * direction2[0]), float(120 * direction2[1])},  {0, 0}, 5, 0, 0, 255 }; g += 1; sleepgun2 = 70; };
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

		if (spheres1[j].ballsDangerouslyClose(myPlayer2.position.x, myPlayer2.position.y)) { healthplayer2.health -= 1;  spheres1[j] = { {-100.f, -100.f} , {0.f, 0.f},  {0, 0}, 10, 0, 0, 255 }; };
		if (spheres2[j].ballsDangerouslyClose(myPlayer1.position.x, myPlayer1.position.y)) { healthplayer1.health -= 1; spheres2[j] = { {-100.f, -100.f} , {0.f, 0.f},  {0, 0}, 10, 0, 0, 255 }; };
	};
};


void Game::render()
{
	myWindow.clear();

	for (int a = 0; a < numberwall; a++)
	{
		edgemap[a].drawWall(&myWindow);
	};
    
	myHealth(myPlayer1.position.x - myPlayer1.radius, myPlayer1.position.y - myPlayer1.radius, healthplayer1, sleepgun1, 0);
	myHealth(myPlayer2.position.x - myPlayer2.radius, myPlayer2.position.y- myPlayer2.radius, healthplayer2, sleepgun2, 1);

	for (int j = 0; j < numberbullet; j++)
	{
		spheres1[j].drawBullet(&myWindow);
		spheres2[j].drawBullet(&myWindow);
	};

	if (gv)
	{
	    myPlayer1.drawSphere(&myWindow);
		myPlayer2.drawSphere(&myWindow);
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

	myPlayer1.position = { 60.f, 990.f };
	myPlayer2.position = { 1840.f, 50.f };

	healthplayer1.health = 3; healthplayer2.health = 3;
	gv = 1;
};


void Game::myHealth(int x, int y, Health health, int sleep, int player)
{
	health.drawHealth(&myWindow, x, y, sleep);

	if (health.health == 0)
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
};

void Game::poolBullet()
{
	for (int s = 0; s < numberbullet; s++)
	{
		spheres1[s] = { {-100.f, -100.f} , {0.f, 0.f},  {0, 0}, 5, 0, 0, 255 };
		spheres2[s] = { {-100.f, -100.f} , {0.f, 0.f},  {0, 0}, 5, 0, 0, 255 };
	};
};


int main()
{
	Game game;
	game.run();
	return 0;
}
