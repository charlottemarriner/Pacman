#include "Pacman.h"

#include <sstream>
#include <time.h>
#include <iostream>

//Constructor Method
Pacman::Pacman(int argc, char* argv[], int ghostCount) : Game(argc, argv), _cPacmanSpeed(0.1f), _cPacmanFrameTime(250), _cMunchiesFrameTime(500), _cherryFrameTime(500)
{
	
	int score=0;
	ghostsCount = ghostCount;
	_pacman = new Player();
	_pacman->dead = false;
	_paused = false;
	_pacman->CurrentFrameTime = 0;
	_pacman->Frame = 0;
	_pacman->speedMultiplier = 1.0f;
	int i;

	//Initialise sound
	_jingle = new SoundEffect();
	_gameplayMusic = new SoundEffect();
	_hohoDeath = new SoundEffect();
	_present = new SoundEffect();
	_elfLaugh = new SoundEffect();



	_ghosts = new MovingEnemy[ghostCount];

	_elf = new MovingEnemy();

	//Initialise Ghost
	for (i = 0; i < ghostCount; i++)
	{
		
		_ghosts[i].direction = 0;
		_ghosts[i].speed = 0.2f;
	}

	//Initialise Elf
	_elf->direction = 0;
	_elf->speed = 0.2f;

	//Initialise Cherry
	_cherry = new Enemy();

	//Initialise Munchies
	for (i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i] = new Enemy();
		_munchies[i]->CurrentFrameTime = 0;
		_munchies[i]->FrameCount = rand() % 1;
		_munchies[i]->CurrentFrameTime = rand() % 500 + 50;
	}
	//if (_startMenu == true)
	//{
		//Initialise important Game aspects
		Audio::Initialise();
		Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
		Input::Initialise();


		// Start the Game Loop - This calls Update and Draw in game loop
		Graphics::StartGameLoop();
	//}
}

//Destructor Method
Pacman::~Pacman()
{
	//Clean up pointers within the Pacman structure
	delete _pacman->Texture;
	delete _pacman->Position;
	delete _pacman->SourceRect;

	delete _jingle;
	delete _gameplayMusic;
	delete _hohoDeath;
	delete _present;
	delete _elfLaugh;
	
	delete _cherry->Texture;
	delete _cherry->Rect;
	delete _munchies[0]->Texture;
	int i;
	for (i = 0; i < MUNCHIECOUNT; i++)
	{
		delete _munchies[i]->Texture;
		delete _munchies[i]->Rect;
	}

	//Clean up the Pacman structure pointer
	delete _pacman;
	delete _munchies;
	delete _cherry;
	delete _ghosts;
	delete _elf;
}

void Pacman::LoadContent()
{
	//Load sound
	_jingle->Load("Sounds/jingle.wav");
	_gameplayMusic->Load("Sounds/gameplayMusic.mp3");
	_hohoDeath->Load("Sounds/hohoDeath.wav");
	_present->Load("Sounds/present.wav");
	_elfLaugh->Load("Sounds/elfLaugh.wav");

	if (!Audio::IsInitialised())
	{
		cout << "Audio is not initialised" << endl;
	}

	if (!_gameplayMusic->IsLoaded())
	{
		cout << "gameplayMusic member sound effect has not loaded" << endl;
	}
	else if (!_hohoDeath->IsLoaded())
	{
		cout << "_hohoDeath member sound effect has not loaded" << endl;
	}
	else if (!_present->IsLoaded())
	{
		cout << "_present member sound effect has not loaded" << endl;
	}
	else if (!_jingle->IsLoaded())
	{
		cout << "_jingle member sound effect has not loaded" << endl;
	}
	else if (!_elfLaugh->IsLoaded())
	{
		cout << "_elfLaugh member sound effect has not loaded" << endl;
	}


	// Load Pacman
	_pacman->Texture = new Texture2D();
	_pacman->Texture->Load("Textures/Pacman.png", false);
	_pacman->Position = new Vector2(60.0f, 650.0f);
	_pacman->SourceRect = new Rect(0.0f, 0.0f, 64, 64);
	_pacman->Direction = 0;

	//Load Ghosts
	int i;
	for (i = 0; i < ghostsCount; i++)
	{
		_ghosts[i].texture = new Texture2D();
		_ghosts[i].texture->Load("Textures/GhostPink.png", false);
		_ghosts[i].position = new Vector2((rand() % Graphics::GetViewportWidth()),
			(rand() % Graphics::GetViewportHeight()));
		_ghosts[i].sourceRect = new Rect(0.0f, 0.0f, 48, 49);
		_ghosts->direction = 0;
	}

	//Load Elf
	_elf->texture = new Texture2D();
	_elf->texture->Load("Textures/EvilElf.png", false);
	_elf->position = new Vector2((rand() % Graphics::GetViewportWidth()),
		(rand() % Graphics::GetViewportHeight()));
	_elf->sourceRect = new Rect(0.0f, 0.0f, 52, 64);
	_elf->direction = 0;

	// Load Munchie
	Texture2D* munchieTex = new Texture2D();
	munchieTex->Load("Textures/munchieTexture.png", true);

	for (i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i]->Texture = munchieTex;
		_munchies[i]->Rect = new Rect(0.0f, 0.0f, 64, 64);
		_munchies[i]->Position = new Vector2(100.0f, 100.0f);
		_munchies[i]->Position = new Vector2(rand() %
			Graphics::GetViewportWidth(), rand() % Graphics::GetViewportHeight());
	}
	
	//Load Cherry
	_cherry->Texture = new Texture2D();
	_cherry->Texture->Load("Textures/cherryTexture.png", true);
	_cherry->Rect = new Rect(0.0f, 0.0f, 64, 64);
	_cherry->Position = new Vector2(200.0f, 200.0f);
	
	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);

	//Set Menu Parameters
	_menuBackground = new Texture2D();
	_menuBackground->Load("Textures/Transparency.png", false);
	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);

	//Set Start Menu Parameters
	_startMenuBackground = new Texture2D();
	_startMenuBackground->Load("Textures/Transparency.png", false);
	_startMenuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_startMenuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);

	//Set game over parameters
	_gameOverBackground = new Texture2D();
	_gameOverBackground->Load("Textures/Transparency.png", false);
	_gameOverRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_gameOverStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);

	//Set Win game parameters
	_winGameBackground = new Texture2D();
	_winGameBackground->Load("Textures/Transparency.png", false);
	_winGameRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_winGameStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
}

void Pacman::Update(int elapsedTime)
{
	//Variables
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	//Gets the current state of the mouse
	Input::MouseState* mouseState = Input::Mouse::GetState();

	//Check if game is paused
	CheckPaused(keyboardState, Input::Keys::P);

	//Check if game is started
	CheckStart(keyboardState, Input::Keys::SPACE);

	//Check if game is over
	CheckGameOver(keyboardState, Input::Keys::R);

	//Check if game is won
	CheckwinGame(keyboardState, Input::Keys::R);
	
	if (munchiesCollected == 20)
	{
		_winGame = true;
	}

	//Update Tasks
	if (!_paused)
	{
		Input(elapsedTime, keyboardState, mouseState);
		UpdatePacman(elapsedTime);
		UpdateCherry(elapsedTime);
		CheckViewportCollision();
		for (int i = 0; i < MUNCHIECOUNT; i++)
		{
			UpdateMunchies(_munchies[i], elapsedTime);
		}
		for (int i = 0; i < ghostsCount; i++)
		{
			UpdateGhost(_ghosts[i], elapsedTime);
		}
		CheckGhostCollisions();
	}
}

	// Gets the current state of the keyboard - Start Menu
	void Pacman::CheckStart(Input::KeyboardState * state, Input::Keys SPACE)
	{
		//Start Game
		if (!_startMenu)
		{
			if (state->IsKeyDown(Input::Keys::SPACE))
				_startMenu = true;
		}
	}

	//Gets the current state of the keyboard - GAME OVER MENU
	void Pacman::CheckGameOver(Input::KeyboardState* state, Input::Keys R)
	{
		//Game over
		if (_gameOver)
		{
			if (state->IsKeyDown(Input::Keys::R))
			{
				_gameOver = false;
				level = 1;
				resetGame();
			}
		}
	}

	void Pacman::CheckwinGame(Input::KeyboardState* state, Input::Keys R)
	{
		int i;
		if (_winGame == true)
		{
			if (state->IsKeyDown(Input::Keys::R))
			{
				_winGame = false;
				nextLevel();
				munchiesCollected = 0;
				playerLives = 5;
				for (i = 0; i < ghostsCount; i++)
				_ghosts[i].speed = _ghosts[i].speed + 0.2;
				level++;
			}
		}
	}

	//Gets the current state of the keyboard - Pause Menu
	void Pacman::CheckPaused(Input::KeyboardState * state, Input::Keys P)
	{
		//Pause Game
		if (state->IsKeyDown(Input::Keys::P) && !_pKeyDown)
		{
			_pKeyDown = true;
			_paused = !_paused;
		}

		else if (state->IsKeyUp(Input::Keys::P))
			_pKeyDown = false;
	}

	//When not paused, checks if key for movement is pressed, then moves
	void Pacman::Input(int elapsedTime, Input::KeyboardState * state, Input::MouseState* mouseState)
	{
		float pacmanSpeed = _cPacmanSpeed * elapsedTime * _pacman->speedMultiplier;
		//When not paused - all below is checked
		if (!_paused)
		{

			// Checks if D key is pressed
			if (state->IsKeyDown(Input::Keys::D))
			{
				_pacman->Position->X += pacmanSpeed; //Moves Pacman across X axis
				_pacman->Direction = 0;
			}

			//Check is A key is pressed
			else if (state->IsKeyDown(Input::Keys::A))
			{
				_pacman->Position->X -= pacmanSpeed; //Moves Pacman left on X axis
				_pacman->Direction = -2;
			}

			//Check if W key is pressed
			else if (state->IsKeyDown(Input::Keys::W))
			{
				_pacman->Position->Y -= pacmanSpeed; //Moves Pacman up on Y axis
				_pacman->Direction = -1;
			}

			//Check if S key is pressed
			else if (state->IsKeyDown(Input::Keys::S))
			{
				_pacman->Position->Y += pacmanSpeed; //Moves Pacman down on Y axis
				_pacman->Direction = 1;
			}
			
			//Speed multiplier
			if (state->IsKeyDown(Input::Keys::LEFTSHIFT))
			{
				//Apply multiplier
				_pacman->speedMultiplier = 2.0f;
			}
			else
			{
				//Reset multiplier
				_pacman->speedMultiplier = 1.0f;
			}
			if (state->IsKeyDown(Input::Keys::R))
			{
				_cherry->Position = new Vector2(rand() %
					Graphics::GetViewportWidth(), rand() % Graphics::GetViewportHeight());
			}
		}
	}

	//Update Pacman frames
	void Pacman::UpdatePacman(int elapsedTime)
	{
		//Update _pacmanFrame variable by adding one
		_pacman->CurrentFrameTime += elapsedTime;
		if (_pacman->CurrentFrameTime > _cPacmanFrameTime)
		{
			_pacman->Frame++;

			if (_pacman->Frame >= 2)
				_pacman->Frame = 0;

			_pacman->CurrentFrameTime = 0;
		}
	}

	//Update Munchie frames
	void Pacman::UpdateMunchies(Enemy* _munchies, int elapsedTime)
	{
		//Update _munchie variable by adding one
		_munchies->CurrentFrameTime += elapsedTime;

		if (_munchies->CurrentFrameTime > _cMunchiesFrameTime)
		{
			_munchies->FrameCount++;
			if (_munchies->FrameCount >= 2)
				_munchies->FrameCount = 0;
			_munchies->CurrentFrameTime = 0;
		}
	}

	

	//Update Cherry frames
	void Pacman::UpdateCherry(int elapsedTime)
	{
		//Update _cherry variable by adding one
		_cherry->CurrentFrameTime += elapsedTime;

		if (_cherry->CurrentFrameTime > _cherryFrameTime)
		{
			_cherry->FrameCount++;
			if (_cherry->FrameCount >= 2)
				_cherry->FrameCount = 0;
			_cherry->CurrentFrameTime = 0;
		}
	}

	//Checks collision in viewport
	void Pacman::CheckViewportCollision()
	{
		//Checks if Pacman is trying to disappear
		if (_pacman->Position->X + _pacman->SourceRect->Width > Graphics::GetViewportWidth())
			//Pacman hit right wall - wrap to left
			_pacman->Position->X = 0 - _pacman->SourceRect->Width;

		else if (_pacman->Position->X + _pacman->SourceRect->Width < 0)
			//Pacman hit left wall - wrap to right
			_pacman->Position->X = Graphics::GetViewportWidth() - _pacman->SourceRect->Width;

		else if (_pacman->Position->Y + _pacman->SourceRect->Width > Graphics::GetViewportHeight())
			//Pacman hit bottom wall - wrap to top
			_pacman->Position->Y = 0 - _pacman->SourceRect->Width;

		else if (_pacman->Position->Y + _pacman->SourceRect->Width < 0)
			//Pacman hit top wall - wrap to bottom
			_pacman->Position->Y = Graphics::GetViewportHeight() - _pacman->SourceRect->Width;

		_pacman->SourceRect->Y = _pacman->SourceRect->Height * _pacman->Direction;
		_pacman->SourceRect->X = _pacman->SourceRect->Width * _pacman->Frame;


		int i;
		for (i = 0; i < MUNCHIECOUNT; i++)
		{
			_munchies[i]->Rect->X = _munchies[i]->Rect->Width * _munchies[i]->FrameCount;
		}
		

		_cherry->Rect->X = _cherry->Rect->Width * _cherry->FrameCount;
	}





void Pacman::Draw(int elapsedTime)

{
	SpriteBatch::BeginDraw(); // Starts Drawing

	if (_pacman->dead == true)
	{
		std::stringstream gameOverStream;
		gameOverStream << "GAME OVER, PRESS R TO CONTINUE";
		SpriteBatch::Draw(_gameOverBackground, _gameOverRectangle, nullptr);
		SpriteBatch::DrawString(gameOverStream.str().c_str(), _gameOverStringPosition, Color::Red);
	}
	else if (_winGame==true)
	{
		std::stringstream winGameStream;
		winGameStream << "YOU WON WITH " << pacmanScore << " POINTS, PRESS R TO PLAY AGAIN ON A HARDER MODE";
		SpriteBatch::Draw(_winGameBackground, _winGameRectangle, nullptr);
		SpriteBatch::DrawString(winGameStream.str().c_str(), _winGameStringPosition, Color::Red);
	}
	else if (_startMenu)
	{
		// Allows us to easily create a string
		std::stringstream stream;
		stream << "Level: "<< level << " Score: " << pacmanScore << " Lives: " << playerLives;

		//Draws Pacman if Pacman is NOT dead
		if (!_pacman->dead)
		{
			SpriteBatch::Draw(_pacman->Texture, _pacman->Position, _pacman->SourceRect); // Draws Pacman
		}

		//Draws munchies
		
		int i;
		for (i = 0; i < MUNCHIECOUNT; i++)
		{
			SpriteBatch::Draw(_munchies[i]->Texture, _munchies[i]->Position, _munchies[i]->Rect);
		}
		
		//Draws cherries
		SpriteBatch::Draw(_cherry->Texture, _cherry->Position, _cherry->Rect);

		//Draws ghosts
		for (i = 0; i < ghostsCount; i++)
		{
			SpriteBatch::Draw(_ghosts[i].texture, _ghosts[i].position, _ghosts[i].sourceRect);
		}

		//Draws Elf
		SpriteBatch::Draw(_elf->texture, _elf->position, _elf->sourceRect);

		// Draws String
		SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
		
		if (_paused)
		{
			std::stringstream menuStream;
			menuStream << "PAUSED";
			SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
			SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);
		}

	}
	else 
	{

		std::stringstream startMenuStream;
		startMenuStream << "TO START THE GAME PRESS THE SPACEBAR";
		SpriteBatch::Draw(_startMenuBackground, _startMenuRectangle, nullptr);
		SpriteBatch::DrawString(startMenuStream.str().c_str(), _startMenuStringPosition, Color::Red);
	}
	
	SpriteBatch::EndDraw(); // Ends Drawing
}

void Pacman::UpdateGhost(MovingEnemy& ghost, int elapsedTime)
{
	if (ghost.direction == 0) //Moves right
	{
		ghost.position->X += ghost.speed * elapsedTime;
	}
	else if (ghost.direction == 1) //Moves left
	{
		ghost.position->X -= ghost.speed * elapsedTime;
	}
	if (ghost.position->X + ghost.sourceRect->Width >= Graphics::GetViewportWidth()) // Hits right edge

	{
		ghost.direction = 1; //Change direction
	}
	else if (ghost.position->X <= 0) //Hits left edge
	{
		ghost.direction = 0; //Change direction
	}
}

void Pacman::UpdateElf(MovingEnemy& elf, int elapsedTime)
{
	if (elf.direction == 0) //Moves right
	{
		elf.position->X += elf.speed * elapsedTime;
	}
	else if (elf.direction == 1) //Moves left
	{
		elf.position->X -= elf.speed * elapsedTime;
	}
	if (elf.position->X + elf.sourceRect->Width >= Graphics::GetViewportWidth()) // Hits right edge

	{
		elf.direction = 1; //Change direction
	}
	else if (elf.position->X <= 0) //Hits left edge
	{
		elf.direction = 0; //Change direction
	}
}

void Pacman::CheckGhostCollisions()
{
	//Local variables
	int i = 0;
	int bottom1 = _pacman->Position->Y + _pacman->SourceRect->Height;
	int bottom2 = 0;
	int left1 = _pacman->Position->X;
	int left2 = 0;
	int right1 = _pacman->Position->X + _pacman->SourceRect->Width;
	int right2 = 0;
	int top1 = _pacman->Position->Y;
	int top2 = 0;


	for (i = 0; i < ghostsCount; i++)
	{
		//Populate variables with Ghost data
		bottom2 = _ghosts[i].position->Y + _ghosts[i].sourceRect->Height;
		left2 = _ghosts[i].position->X;
		right2 = _ghosts[i].position->X + _ghosts[i].sourceRect->Width;
		top2 = _ghosts[i].position->Y;

		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			playerLives--;
			Audio::Play(_hohoDeath);
			pacmanScore = pacmanScore - 50;
			_pacman->Position->X = 60;
			_pacman->Position->Y = 650;
			if (playerLives <= 0)
			{
				pacmanDeath();
				resetGame();
				i = ghostsCount;
				_pacman->Position->X = -100;
				_pacman->Position->Y = -100;
			}

		}
	}

	for (i = 0; i < MUNCHIECOUNT; i++)
	{
		//Populate variables with Munchie data
		bottom2 = _munchies[i]->Position->Y + _munchies[i]->Rect->Height;
		left2 = _munchies[i]->Position->X;
		right2 = _munchies[i]->Position->X + _munchies[i]->Rect->Width;
		top2 = _munchies[i]->Position->Y;

		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			_munchies[i]->Position->X = -100;
			_munchies[i]->Position->Y = -100;
			Audio::Play(_jingle);
			pacmanScore = pacmanScore + 30;
			munchiesCollected++;
			i = MUNCHIECOUNT;
		}
	}

	//Populate variables with Cherry Data
	bottom2 = _cherry->Position->Y + _cherry->Rect->Height;
	left2 = _cherry->Position->X;
	right2 = _cherry->Position->X + _cherry->Rect->Width;
	top2 = _cherry->Position->Y;

	if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
	{
		_cherry->Position->X = -100;
		_cherry->Position->Y = -100;
		Audio::Play(_present);
		pacmanScore = pacmanScore + 100;
	}

	//Populate variables with Elf Data
	bottom2 = _elf->position->Y + _elf->sourceRect->Height;
	left2 = _elf->position->X;
	right2 = _elf->position->X + _elf->sourceRect->Width;
	top2 = _elf->position->Y;

	if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
	{
		playerLives = 0;
		Audio::Play(_elfLaugh);
		_pacman->Position->X = -100;
		_pacman->Position->Y = -100;
		if (playerLives <= 0)
		{
			pacmanDeath();
			_pacman->Position->X = -100;
			_pacman->Position->Y = -100;
		}

	}
}

void Pacman::pacmanDeath()
{
	_pacman->dead = true;
	//switch (_pacman->dead == true)
	//{
	//default:
		_startMenu = false;
		_gameOver = true;
	//}
}

void Pacman::resetGame()
{
	_pacman->dead = false;
	LoadContent();
	playerLives = 5;
	pacmanScore = 0;
}

void Pacman::nextLevel()
{
	_pacman->dead = false;
	LoadContent();
}


