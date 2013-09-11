#include "Imports.hpp"
#include "Engine.hpp"

int main()
{
	if( freopen("stderr.txt", "w", stderr) != NULL )
	{
		Engine* game = new Engine();

		// Main loop
		bool running = true;
		while(running)
		{
			running  = game->eventManager();
			game->run();
		}

		delete game;
		fclose (stderr);
	}
	else
	{
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}