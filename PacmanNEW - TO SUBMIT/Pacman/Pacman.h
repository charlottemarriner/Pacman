#pragma once

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

//Munchie #
#define MUNCHIECOUNT 25

// Just need to include main header file
#include "S2D/S2D.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

//Structure Definition
struct Player
{
	bool dead;
	Vector2* Position;
	Rect* SourceRect;
	Texture2D* Texture;
	int Direction;
	int Frame;
	int CurrentFrameTime;
	float speedMultiplier;
};

struct Enemy
{
	Rect* Rect;
	Texture2D* Texture;
	Vector2* Position;
	int FrameCount;
	int CurrentFrameTime;
	int frameTime;
};

struct MovingEnemy
{
	Vector2* position;
	Texture2D* texture;
	Rect* sourceRect;
	int direction;
	float speed;
};



// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Pacman : public Game
{
private:
	int pacmanScore = 0;
	int munchiesCollected = 0;
	int playerLives = 5;
	int level = 1;
	int ghostsCount;

	// Functions
	void Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState);
	void CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey);
	void CheckStart(Input::KeyboardState* state, Input::Keys startKey);
	void CheckGameOver(Input::KeyboardState* state, Input::Keys RKey);
	void CheckwinGame(Input::KeyboardState* state, Input::Keys RKey);
	void CheckViewportCollision();
	void UpdatePacman(int elapsedTime);
	void UpdateMunchies(Enemy* _munchies, int elapsedTime);
	void UpdateCherry(int elapsedTime);
	void CheckGhostCollisions();
	void UpdateGhost(MovingEnemy&, int elapsedTime);
	void UpdateElf(MovingEnemy&, int elapsedTime);
	void pacmanDeath();
	void resetGame();
	void nextLevel();

	//Data to represent sound
	SoundEffect* _jingle;
	SoundEffect* _gameplayMusic;
	SoundEffect* _hohoDeath;
	SoundEffect* _present;
	SoundEffect* _elfLaugh;


// Data to represent Pacman
	const int _cPacmanFrameTime;
	Player* _pacman;

	// Data to represent Munchie
	Enemy* _munchies[MUNCHIECOUNT];
	const int _cMunchiesFrameTime;

	//Data to represent Enemy
	MovingEnemy* _ghosts;
	MovingEnemy* _elf;
	

	

	//Data to represent Cherry - yummy
	Enemy* _cherry;
	const int _cherryFrameTime;
	

	//Data for Menu
	Texture2D* _menuBackground;
	Rect* _menuRectangle;
	Vector2* _menuStringPosition;
	bool _paused;

	//Data for Start Menu
	Texture2D* _startMenuBackground;
	Rect* _startMenuRectangle;
	Vector2* _startMenuStringPosition;
	bool _startMenu;

	//Data for Game over
	Texture2D* _gameOverBackground;
	Rect* _gameOverRectangle;
	Vector2* _gameOverStringPosition;
	bool _gameOver;

	//Data for Win Game
	Texture2D* _winGameBackground;
	Rect* _winGameRectangle;
	Vector2* _winGameStringPosition;
	bool _winGame;

	// Position for String
	Vector2* _stringPosition;

	//Constant data for Game Variables
	const float _cPacmanSpeed;

	//Checks if P key has been pressed
	bool _pKeyDown;

public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[], int ghostCount);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);
};