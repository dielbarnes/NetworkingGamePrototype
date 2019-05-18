//
// GameScreen.h
// Copyright © 2018 Diel Barnes. All rights reserved.
//

#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <iostream>
#include "Screen.h"
#include "TileMap.h"
#include "AnimatedSprite.hpp"
#include "Player.h"

#define RESOURCE_PATH_TILES "Resources/tiles.png"
#define RESOURCE_PATH_FOUNTAIN "Resources/fountain.png"
#define RESOURCE_PATH_COIN "Resources/coin.png"

#define TILE_LENGTH 64
#define FOUNTAIN_FRAME_WIDTH 64
#define FOUNTAIN_FRAME_HEIGHT 96
#define FOUNTAIN_SCALE 2.0f
#define COIN_FRAME_LENGTH 32
#define COIN_SCALE 1.5f
#define MAX_COINS 40 // 304 for full map

class GameScreen : public Screen
{
public:
	GameScreen();
	~GameScreen();

	virtual bool Initialize(sf::RenderWindow &window, Network &network);
	void SetPlayers(Player &player, std::vector<Player> &players);
	virtual int Run();

private:
	TileMap m_map;
	sf::Texture m_fountainTexture;
	Animation m_fountainAnimation;
	AnimatedSprite m_fountainAnimatedSprite;

	sf::Texture m_coinTexture;
	Animation m_coinAnimation;
	AnimatedSprite m_coins[MAX_COINS];

	Player* m_pPlayer;
	std::vector<Player>* m_pPlayers;

	bool m_bWindowHasFocus;
	bool m_bNoKeyPressed;

	void UpdateCoinLocations();
	void AnimateCharacter(sf::Time frameTime, sf::Time elapsedTime);
	void AnimateOtherPlayers(sf::Time frameTime, sf::Time elapsedTime);
	void Draw();
};

#endif
