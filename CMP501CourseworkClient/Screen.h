//
// Screen.h
// Copyright © 2018 Diel Barnes. All rights reserved.
//

#ifndef SCREEN_H
#define SCREEN_H

// Use TGUI library for SFML
// https://tgui.eu
// https://www.sfml-dev.org
#include <TGUI/TGUI.hpp>
#include <SFML/Graphics.hpp>
#include "Network.h"
#include "Utils.h"

enum ScreenIndex : int
{
	WelcomeScreenIndex = 0,
	GameScreenIndex
};

class Screen
{
public:
	virtual bool Initialize(sf::RenderWindow &window, Network &network) = 0;
	virtual int Run() = 0;

protected:
	sf::RenderWindow* m_pWindow;
	tgui::Gui* m_pGui;
	Network* m_pNetwork;
};

#endif
