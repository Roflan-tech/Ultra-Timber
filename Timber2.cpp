#include <SFML/graphics.hpp>
#include <sstream>
#include <SFML/Audio.hpp>

using namespace sf;

const int NUM_LOGS = 6;
const int NUM_BRANCHES = 6;

Sprite branches[NUM_BRANCHES];
Sprite logs[NUM_LOGS];

enum class side {LEFT, RIGHT, NONE};
side branchPositions[NUM_BRANCHES];

enum class kind {RAINBOW, ANCIENT, SCIFI};
kind logKind[NUM_LOGS];

enum class character{DEMON, ELF, ROBOT};

void updateBranches(int seed);
void updateLogs(int seed);

int main()
{
	VideoMode vm(1920, 1080);
	RenderWindow window(vm, "Timber", Style::Fullscreen);

	Texture textureBackground;
	textureBackground.loadFromFile("graphics/background.png");
	Sprite spriteBackground;
	spriteBackground.setTexture(textureBackground);
	spriteBackground.setPosition(0, 0);

	Texture textureTutorial;
	textureTutorial.loadFromFile("graphics/tutorial.png");
	Sprite spriteTutorial;
	spriteTutorial.setTexture(textureTutorial);
	FloatRect tutRect = spriteTutorial.getLocalBounds();
	spriteTutorial.setOrigin(tutRect.left + tutRect.width / 2.0f,
		tutRect.top + tutRect.height / 2.0f);
	spriteTutorial.setPosition(1920 / 2.0f, 1080 / 2.0f);

	Texture textureplayerElf;
	textureplayerElf.loadFromFile("graphics/Whitty.png");
	Sprite spriteElf;
	spriteElf.setTexture(textureplayerElf);
	spriteElf.setPosition(2000, 720);

	Texture textureplayerDemon;
	textureplayerDemon.loadFromFile("graphics/michale.png");
	Sprite spriteDemon;
	spriteDemon.setTexture(textureplayerDemon);
	spriteDemon.setPosition(2000, 720);

	Texture textureplayerRobot;
	textureplayerRobot.loadFromFile("graphics/robodude.png");
	Sprite spriteRobot;
	spriteRobot.setTexture(textureplayerRobot);
	spriteRobot.setPosition(2000, 720);
	
	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(7000, 830);

	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);
	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	Clock clock;

	bool paused = true;
	int score = 0;

	Text messageText;
	Text scoreText;

	Font font;
	font.loadFromFile("fonts/Roboto-Regular.ttf");

	messageText.setFont(font);
	scoreText.setFont(font);

	messageText.setString("Press Enter To Start!");
	scoreText.setString("Score = 0");

	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);

	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);

	// Position the text
	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left +
		textRect.width / 2.0f,
		textRect.top +
		textRect.height / 2.0f);
	messageText.setPosition(20000, 20000);
	scoreText.setPosition(20, 20);

	// Background rectangle
	RectangleShape scoreShape;
	scoreShape.setFillColor(Color(0, 0, 0, 128));
	scoreShape.setOutlineColor(Color(240, 247, 25, 128));
	scoreShape.setOutlineThickness(2);
	scoreShape.setPosition(1920 / 2.0f, 1080 / 2.0f);
	scoreShape.setPosition(20, 35);
	scoreShape.setSize(Vector2f(550, 100));

	// Death rectangle
	RectangleShape deathShape;
	deathShape.setFillColor(Color(0, 0, 0, 175));
	deathShape.setOutlineColor(Color(240, 247, 25, 128));
	deathShape.setOutlineThickness(2);
	deathShape.setPosition(20000, 2000);
	deathShape.setSize(Vector2f(800, 600));
	FloatRect deathRect = deathShape.getLocalBounds();
	deathShape.setOrigin(deathRect.left + deathRect.width / 2.0f,
		deathRect.top + deathRect.height / 2.0f);

	// Prepare 6 branches
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");

	// Set the texture for each branch sprite 
	for (int i = 0; i < NUM_BRANCHES; i++) {
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);

		// Set the sprite's origin to dead center

		// We can spin it round without changing its position
		branches[i].setOrigin(220, 20);
	}

	side playerSide = side::LEFT;

	// Prepare the gravestone
	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(6000, 860);

	Texture textureRainbow;
	textureRainbow.loadFromFile("graphics/rainbow.png");
	Sprite spriteRainbow; 
	spriteRainbow.setTexture(textureRainbow);

	Texture textureAncient;
	textureAncient.loadFromFile("graphics/ancient.png");
	Sprite spriteAncient;
	spriteAncient.setTexture(textureAncient);

	Texture textureScifi;
	textureScifi.loadFromFile("graphics/scifi.png");
	Sprite spriteScifi;
	spriteScifi.setTexture(textureScifi);

	Sprite flyingAncientLog;
	flyingAncientLog.setTexture(textureAncient);
	flyingAncientLog.setScale(0.85, 0.85); 
	flyingAncientLog.setPosition(2000, 720);

	Sprite flyingRainbowLog;
	flyingRainbowLog.setTexture(textureRainbow);
	flyingRainbowLog.setScale(0.85, 0.85);
	flyingRainbowLog.setPosition(2000, 720);

	Sprite flyingScifiLog;
	flyingScifiLog.setTexture(textureScifi);
	flyingScifiLog.setScale(0.85, 0.85);
	flyingScifiLog.setPosition(2000, 720);

	// Prepare the sounds
	// The player chopping sound
	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);

	// Prepare the flying log
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);

	// Line the axe up with the tree
	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;

	// Some other useful log related variables
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	// Control the player input
	bool acceptInput = false;

	character player = character::ELF;

	enum class logtmp {RAI, ANC, SCI};

	logtmp logsave;

	while (window.isOpen())
	{
		Event event;

		while (window.pollEvent(event)) 
		{
			if (event.type == Event::KeyReleased && !paused)
			{
				acceptInput = true;
				spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape)) 
		{
			window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			paused = false;
			score = 0;
			timeRemaining = 8;

			// hide death rectangle
			deathShape.setPosition(20000, 2000);

			// hide tutorial 
			spriteTutorial.setPosition(20000, 10000);

			for (int i = 1; i < NUM_BRANCHES; i++)
			{
				branchPositions[i] = side::NONE;
			}

			for (int i = 1; i < NUM_LOGS; i++)
			{
				logKind[i] = kind::RAINBOW;
			}
			
			spriteRIP.setPosition(675, 2000);

			player = character::ELF;
			spriteElf.setPosition(580, 720);
			spriteDemon.setPosition(2000, 720);
			spriteRobot.setPosition(2000, 720);

			acceptInput = true;
		}

		if (acceptInput)
		{
			if (Keyboard::isKeyPressed(Keyboard::Q))
			{
				spriteDemon.setPosition(2000, 720);
				spriteRobot.setPosition(2000, 720);

				if (playerSide == side::RIGHT)
				{
					player = character::ELF;
					spriteElf.setPosition(1200, 720);
				}
				else if (playerSide == side::LEFT)
				{
					player = character::ELF;
					spriteElf.setPosition(580, 720);
				}
			}
			if (Keyboard::isKeyPressed(Keyboard::W))
			{
				spriteElf.setPosition(2000, 720);
				spriteRobot.setPosition(2000, 720);

				if (playerSide == side::RIGHT)
				{
					player = character::DEMON;
					spriteDemon.setPosition(1200, 720);
				}
				else if (playerSide == side::LEFT)
				{
					player = character::DEMON;
					spriteDemon.setPosition(580, 720);
				}
			}
			if (Keyboard::isKeyPressed(Keyboard::E))
			{
				spriteElf.setPosition(2000, 720);
				spriteDemon.setPosition(2000, 720);

				if (playerSide == side::RIGHT)
				{
					player = character::ROBOT;
					spriteRobot.setPosition(1200, 720);
				}
				else if (playerSide == side::LEFT)
				{
					player = character::ROBOT;
					spriteRobot.setPosition(580, 720);
				}
			}
			if (Keyboard::isKeyPressed(Keyboard::Right)) 
			{
				playerSide = side::RIGHT;

				// has the player chopped log with wrong character
				if ((player == character::ELF) && ((logKind[5] == kind::ANCIENT) || (logKind[5] == kind::SCIFI)))
				{
					// death
					paused = true;
					acceptInput = false;

					// hide the player
					spriteElf.setPosition(2000, 660);
					// hide the player
					spriteDemon.setPosition(2000, 660);
					// hide the player
					spriteRobot.setPosition(2000, 660);

					// hide flying logs
					logActive = false;
					flyingAncientLog.setPosition(2000, 720);
					flyingRainbowLog.setPosition(2000, 720);

					// hide the axe
					spriteAxe.setPosition(2000, 830);

					// draw death rect and set reset its origin
					deathShape.setSize(Vector2f(1200, 600));
					FloatRect deathRect = deathShape.getLocalBounds();
					deathShape.setOrigin(deathRect.left + deathRect.width / 2.0f,
						deathRect.top + deathRect.height / 2.0f);
					deathShape.setPosition(1920 / 2.0f, 1080 / 2.0f);

					// Change the text of the message
					messageText.setFillColor(Color::Red);
					messageText.setString("WRONG CHARACTER!");

					// Center it on the screen
					FloatRect textRect = messageText.getLocalBounds();

					messageText.setOrigin(textRect.left + textRect.width / 2.0f,
						textRect.top + textRect.height / 2.0f);

					messageText.setPosition(1920 / 2.0f,
						1080 / 2.0f);
				}
				else if ((player == character::DEMON) && ((logKind[5] == kind::RAINBOW) || (logKind[5] == kind::SCIFI)))
				{
					// death
					paused = true;
					acceptInput = false;

					// hide the player
					spriteElf.setPosition(2000, 660);
					// hide the player
					spriteDemon.setPosition(2000, 660);
					// hide the player
					spriteRobot.setPosition(2000, 660);

					// hide flying logs
					logActive = false;
					flyingAncientLog.setPosition(2000, 720);
					flyingRainbowLog.setPosition(2000, 720);

					// hide the axe
					spriteAxe.setPosition(2000, 830);

					// draw death rect and set reset its origin
					deathShape.setSize(Vector2f(1200, 600));
					FloatRect deathRect = deathShape.getLocalBounds();
					deathShape.setOrigin(deathRect.left + deathRect.width / 2.0f,
						deathRect.top + deathRect.height / 2.0f);
					deathShape.setPosition(1920 / 2.0f, 1080 / 2.0f);

					// Change the text of the message
					messageText.setFillColor(Color::Red);
					messageText.setString("WRONG CHARACTER!");

					// Center it on the screen
					FloatRect textRect = messageText.getLocalBounds();

					messageText.setOrigin(textRect.left + textRect.width / 2.0f,
						textRect.top + textRect.height / 2.0f);

					messageText.setPosition(1920 / 2.0f,
						1080 / 2.0f);
				}
				else if ((player == character::ROBOT) && ((logKind[5] == kind::RAINBOW) || (logKind[5] == kind::ANCIENT)))
				{
					// death
					paused = true;
					acceptInput = false;

					// hide the player
					spriteElf.setPosition(2000, 660);
					// hide the player
					spriteDemon.setPosition(2000, 660);
					//
					spriteRobot.setPosition(2000, 660);

					// hide flying logs
					logActive = false;
					flyingAncientLog.setPosition(2000, 720);
					flyingRainbowLog.setPosition(2000, 720);

					// hide the axe
					spriteAxe.setPosition(2000, 830);

					// draw death rect and set reset its origin
					deathShape.setSize(Vector2f(1200, 600));
					FloatRect deathRect = deathShape.getLocalBounds();
					deathShape.setOrigin(deathRect.left + deathRect.width / 2.0f,
						deathRect.top + deathRect.height / 2.0f);
					deathShape.setPosition(1920 / 2.0f, 1080 / 2.0f);

					// Change the text of the message
					messageText.setFillColor(Color::Red);
					messageText.setString("WRONG CHARACTER!");

					// Center it on the screen
					FloatRect textRect = messageText.getLocalBounds();

					messageText.setOrigin(textRect.left + textRect.width / 2.0f,
						textRect.top + textRect.height / 2.0f);

					messageText.setPosition(1920 / 2.0f,
						1080 / 2.0f);
				}

				score++;

				timeRemaining += (2 / score) + .35;

				if (!paused)
				{
					spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
				}

				if (player == character::ELF && !paused)
				{
					spriteElf.setPosition(1200, 720);
				}
				else if (player == character::DEMON && !paused)
				{
					spriteDemon.setPosition(1200, 720);
				}
				else if (player == character::ROBOT && !paused)
				{
					spriteRobot.setPosition(1200, 720);
				}
				
				if (logKind[5] == kind::RAINBOW && !paused)
				{
					flyingRainbowLog.setPosition(810, 740);
					logsave = logtmp::RAI;

				}
				else if (logKind[5] == kind::ANCIENT && !paused)
				{
					flyingAncientLog.setPosition(800, 740);
					logsave = logtmp::ANC;
				}
				else if (logKind[5] == kind::SCIFI && !paused)
				{
					flyingScifiLog.setPosition(800, 740);
					logsave = logtmp::SCI;
				}
				updateBranches(score);

				updateLogs(score+69);

				logSpeedX = -5000;
				logActive = true;
				acceptInput = false;

					chop.play();
				}
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				playerSide = side::LEFT;

				// has the player chopped log with wrong character
				if ((player == character::ELF) && ((logKind[5] == kind::ANCIENT) || (logKind[5] == kind::SCIFI)))
				{
					// death
					paused = true;
					acceptInput = false;

					// hide the player
					spriteElf.setPosition(2000, 660);
					// hide the player
					spriteDemon.setPosition(2000, 660);
					// hide the player
					spriteRobot.setPosition(2000, 660);

					// hide flying logs
					logActive = false;
					flyingAncientLog.setPosition(2000, 720);
					flyingRainbowLog.setPosition(2000, 720);

					// hide the axe
					spriteAxe.setPosition(2000, 830);

					// draw death rect and set reset its origin
					deathShape.setSize(Vector2f(1200, 600));
					FloatRect deathRect = deathShape.getLocalBounds();
					deathShape.setOrigin(deathRect.left + deathRect.width / 2.0f,
						deathRect.top + deathRect.height / 2.0f);
					deathShape.setPosition(1920 / 2.0f, 1080 / 2.0f);

					// Change the text of the message
					messageText.setFillColor(Color::Red);
					messageText.setString("WRONG CHARACTER!");

					// Center it on the screen
					FloatRect textRect = messageText.getLocalBounds();

					messageText.setOrigin(textRect.left + textRect.width / 2.0f,
						textRect.top + textRect.height / 2.0f);

					messageText.setPosition(1920 / 2.0f,
						1080 / 2.0f);
				}
				else if ((player == character::DEMON) && ((logKind[5] == kind::RAINBOW) || (logKind[5] == kind::SCIFI)))
				{
					// death
					paused = true;
					acceptInput = false;

					// hide the player
					spriteElf.setPosition(2000, 660);
					// hide the player
					spriteDemon.setPosition(2000, 660);
					spriteRobot.setPosition(2000, 660);


					// hide flying logs
					logActive = false;
					flyingAncientLog.setPosition(2000, 720);
					flyingRainbowLog.setPosition(2000, 720);

					// hide the axe
					spriteAxe.setPosition(2000, 830);

					// draw death rect and set reset its origin
					deathShape.setSize(Vector2f(1200, 600));
					FloatRect deathRect = deathShape.getLocalBounds();
					deathShape.setOrigin(deathRect.left + deathRect.width / 2.0f,
						deathRect.top + deathRect.height / 2.0f);
					deathShape.setPosition(1920 / 2.0f, 1080 / 2.0f);

					// Change the text of the message
					messageText.setFillColor(Color::Red);
					messageText.setString("WRONG CHARACTER");

					// Center it on the screen
					FloatRect textRect = messageText.getLocalBounds();

					messageText.setOrigin(textRect.left + textRect.width / 2.0f,
						textRect.top + textRect.height / 2.0f);

					messageText.setPosition(1920 / 2.0f,
						1080 / 2.0f);
				}
				else if ((player == character::ROBOT) && ((logKind[5] == kind::RAINBOW) || (logKind[5] == kind::ANCIENT)))
				{
					// death
					paused = true;
					acceptInput = false;

					// hide the player
					spriteElf.setPosition(2000, 660);
					// hide the player
					spriteDemon.setPosition(2000, 660);
					//
					spriteRobot.setPosition(2000, 660);

					// hide flying logs
					logActive = false;
					flyingAncientLog.setPosition(2000, 720);
					flyingRainbowLog.setPosition(2000, 720);

					// hide the axe
					spriteAxe.setPosition(2000, 830);

					// draw death rect and set reset its origin
					deathShape.setSize(Vector2f(1200, 600));
					FloatRect deathRect = deathShape.getLocalBounds();
					deathShape.setOrigin(deathRect.left + deathRect.width / 2.0f,
						deathRect.top + deathRect.height / 2.0f);
					deathShape.setPosition(1920 / 2.0f, 1080 / 2.0f);

					// Change the text of the message
					messageText.setFillColor(Color::Red);
					messageText.setString("WRONG CHARACTER!");

					// Center it on the screen
					FloatRect textRect = messageText.getLocalBounds();

					messageText.setOrigin(textRect.left + textRect.width / 2.0f,
						textRect.top + textRect.height / 2.0f);

					messageText.setPosition(1920 / 2.0f,
						1080 / 2.0f);
				}


				score++;

				timeRemaining += (2 / score) + .35;

				if (!paused)
				{
					spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
				}

				if (player == character::ELF && !paused)
				{
					spriteElf.setPosition(580, 720);
				}
				else if (player == character::DEMON && !paused)
				{
					spriteDemon.setPosition(580, 720);
				}
				else if (player == character::ROBOT && !paused)
				{
					spriteRobot.setPosition(580, 720);
				}

				if (logKind[5] == kind::RAINBOW && !paused)
				{
					flyingRainbowLog.setPosition(810, 740);
					logsave = logtmp::RAI;
				}
				else if (logKind[5] == kind::ANCIENT && !paused)
				{
					flyingAncientLog.setPosition(800, 740);
					logsave = logtmp::ANC;
				}
				else if (logKind[5] == kind::SCIFI && !paused)
				{
					flyingScifiLog.setPosition(800, 740);
					logsave = logtmp::SCI;
				}

				updateBranches(score);

				updateLogs(score+69);

				logSpeedX = 5000;
				logActive = true;

				acceptInput = false;

				chop.play();
			}
		}

		if (!paused)
		{
			Time dt = clock.restart();

			timeRemaining -= dt.asSeconds();
			timeBar.setSize(Vector2f(timeBarWidthPerSecond* timeRemaining, timeBarHeight));

			if (timeRemaining <= 0.0f) 
			{
				paused = true;
				messageText.setString("Out of time!!");
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top +
					textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
			}

			std::stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());

			if (logActive)
			{
				if (logsave == logtmp::RAI)
				{
					flyingRainbowLog.setPosition(
						flyingRainbowLog.getPosition().x + (logSpeedX * dt.asSeconds()),
						flyingRainbowLog.getPosition().y + (logSpeedY * dt.asSeconds()));

					// Has the log reached the right hand edge?
					if (flyingRainbowLog.getPosition().x < -100 ||
						flyingRainbowLog.getPosition().x > 2000)
					{
						// Set it up ready to be a whole new log next frame
						logActive = false;
						flyingRainbowLog.setPosition(810, 720);
					}
				}
				if (logsave == logtmp::ANC)
				{

					flyingAncientLog.setPosition(
						flyingAncientLog.getPosition().x + (logSpeedX * dt.asSeconds()),
						flyingAncientLog.getPosition().y + (logSpeedY * dt.asSeconds()));

					// Has the log reached the right hand edge?
					if (flyingAncientLog.getPosition().x < -100 ||
						flyingAncientLog.getPosition().x > 2000)
					{
						// Set it up ready to be a whole new log next frame
						logActive = false;
						flyingAncientLog.setPosition(810, 720);
					}
				}
				if (logsave == logtmp::SCI)
				{

					flyingScifiLog.setPosition(
						flyingScifiLog.getPosition().x + (logSpeedX * dt.asSeconds()),
						flyingScifiLog.getPosition().y + (logSpeedY * dt.asSeconds()));

					// Has the log reached the right hand edge?
					if (flyingScifiLog.getPosition().x < -100 ||
						flyingScifiLog.getPosition().x > 2000)
					{
						// Set it up ready to be a whole new log next frame
						logActive = false;
						flyingScifiLog.setPosition(810, 720);
					}
				}
			}
			

			// update the branch sprites
			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				float height = i * 150;

				if (branchPositions[i] == side::LEFT)
				{
					// Move the sprite to the left side
					branches[i].setPosition(610, height);

					// Flip the sprite round the other way
					
					// doesn't work
					//branches[i].scale(-1, 1);
					branches[i].setTextureRect(IntRect(440, 0, -440, height));
				}
				else if (branchPositions[i] == side::RIGHT)
				{
					// Move the sprite to the right side
					branches[i].setPosition(1330, height);

					// Set the sprite rotation to normal
					//branches[i].setRotation(0);
					branches[i].setTextureRect(IntRect(0, 0, 440, height));
				}
				else 
				{
					// Hide the branch
					branches[i].setPosition(3000, height);
				}
			}

			for (int i = 0; i < NUM_LOGS; i++)
			{
				float height = i * 150;

				if (logKind[i] == kind::RAINBOW)
				{
					logs[i].setPosition(810, height);
					logs[i].setTexture(textureRainbow, true);
				}
				if (logKind[i] == kind::ANCIENT)
				{
					logs[i].setPosition(800, height);
					logs[i].setTexture(textureAncient, true);
				}
				if (logKind[i] == kind::SCIFI)
				{
					logs[i].setPosition(800, height);
					logs[i].setTexture(textureScifi, true);
				}
			}

			// has the player been squished by a branch?
			if (branchPositions[5] == playerSide)
			{
				// death
				paused = true;
				acceptInput = false;

				// draw tombstone deppended on player death position
				if (branchPositions[5] == side::RIGHT)
				{
					spriteRIP.setPosition(1200, 760);
				}
				else if (branchPositions[5] == side::LEFT)
				{
					// Draw the gravestone
					spriteRIP.setPosition(580, 760);
				}

				// hide the player
				spriteElf.setPosition(20000, 660);
				spriteDemon.setPosition(20000, 720);
				spriteRobot.setPosition(20000, 720);

				// hide flying logs
				flyingAncientLog.setPosition(2000, 720);
				flyingRainbowLog.setPosition(2000, 720);
				flyingScifiLog.setPosition(2000, 720);

				// hide the axe
				spriteAxe.setPosition(2000, 830);

				messageText.setFillColor(Color::Red);
				// Change the text of the message
				messageText.setString("YOU DIED!");

				// draw death rectangle

				deathShape.setPosition(1920 / 2.0f, 1080 / 2.0f);

				// Center it on the screen
				FloatRect textRect = messageText.getLocalBounds();

				messageText.setOrigin(textRect.left + textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);

				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
			}

		}

		window.clear();

		// Draw our game scene here
		window.draw(spriteBackground);

		window.draw(spriteTutorial);


		//window.draw(spriteLog);

		window.draw(flyingAncientLog);

		window.draw(flyingRainbowLog);

		window.draw(flyingScifiLog);

		for (int i = 0; i < NUM_BRANCHES; i++) 
		{
			window.draw(branches[i]);
		}

		for (int i = 0; i < NUM_LOGS; i++)
		{
			window.draw(logs[i]);
		}

		//window.draw(spriteRainbow);

		//window.draw(spriteAncient);

		// Draw the player
		window.draw(spriteElf);

		window.draw(spriteDemon);

		window.draw(spriteRobot);

		// Draw the axe
		window.draw(spriteAxe);

		// Draw the gravestone
		window.draw(spriteRIP);

		// Draw rectangle 
		window.draw(scoreShape);

		// Draw the score
		window.draw(scoreText);

		// Draw the timebar
		window.draw(timeBar);

		window.draw(deathShape);

		if (paused)
		{
			// Draw our message
			window.draw(messageText);
		}

		// Show everything we just drew
		window.display();
	}
	return 0;
}

void updateBranches(int seed) 
{
	for (int j = NUM_BRANCHES - 1; j > 0; j--)
	{
		branchPositions[j] = branchPositions[j - 1];
	}

	srand((int)time(0) + seed);
	int r = (rand() % 5);

	switch (r)
	{
	case 0:
		branchPositions[0] = side::LEFT;
		break;
	case 1:
		branchPositions[0] = side::RIGHT;
		break;
	default:
		branchPositions[0] = side::NONE;
		break;
	}
}

void updateLogs(int seed)
{
	for (int j = NUM_LOGS - 1; j > 0; j--)
	{
		logKind[j] = logKind[j - 1];
	}

	srand((int)time(0) + seed);
	int r = (rand() % 5);

	switch (r)
	{
	case 0:
		logKind[0] = kind::ANCIENT;
		break;
	case 1:
		logKind[0] = kind::SCIFI;
		break;
	default:
		logKind[0] = kind::RAINBOW;
		break;
	}
}
