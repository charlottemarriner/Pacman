#include "Pacman.h"
#include <iostream>

//Entry Point for Application
int main(int argc, char* argv[]) 
{

	int difficulty;
	int ghostCount;
	cout << "Select a difficulty: 1. Easy 2. Intermediate 3. Hard" << endl;
	cin >> difficulty;
	if (difficulty == 1)
	{
		ghostCount = 4;
	}
	else if (difficulty == 2)
	{
		ghostCount = 6;
	}
	else if (difficulty == 3)
	{
		ghostCount = 8;
	}
	else
	{
		cout << "Invalid input";
	}

	Pacman* game = new Pacman(argc, argv, ghostCount);

}

