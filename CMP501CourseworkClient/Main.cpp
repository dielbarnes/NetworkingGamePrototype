//
// Main.cpp
// Copyright © 2018 Diel Barnes. All rights reserved.
//

#include "WelcomeScreen.h"
#include "GameScreen.h"

int main()
{
	// Create window
	sf::RenderWindow window(
		sf::VideoMode(1344, 832),
		"Client",
		sf::Style::Titlebar | sf::Style::Close);	// Disable resize
	window.setFramerateLimit(60);

	// Create a player and a vector for other players
	Player player;
	std::vector<Player> players;

	// Initialize network manager
	Network network;
	network.SetPlayers(player, players);

	// Initialize screens
	std::vector<Screen*> screens;

	WelcomeScreen welcomeScreen;
	if (!welcomeScreen.Initialize(window, network))
	{
		return EXIT_FAILURE;
	}
	welcomeScreen.SetPlayer(player);
	screens.push_back(&welcomeScreen);

	GameScreen gameScreen;
	if (!gameScreen.Initialize(window, network))
	{
		return EXIT_FAILURE;
	}
	gameScreen.SetPlayers(player, players);
	screens.push_back(&gameScreen);

	// Run main loop
	int screen = ScreenIndex::WelcomeScreenIndex;
	while (screen >= 0)
	{
		screen = screens[screen]->Run();
	}

	return EXIT_SUCCESS;
}
