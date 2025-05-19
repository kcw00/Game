#include "Game.h"

int main(int argc, char** argv)
{
	Game game; // create object
	bool success = game.Initialize(); // use boolian 
	if (success)
	{
		game.RunLoop();
	}
	game.Shutdown();
	return 0;
}