//
// Player.h
// Copyright © 2018 Diel Barnes. All rights reserved.
//

#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "AnimatedSprite.hpp"
#include "Utils.h"

#define RESOURCE_PATH_GIRL "Resources/girl-spritesheet.png"
#define RESOURCE_PATH_BOY "Resources/boy-spritesheet.png"

#define NAMETAG_WIDTH 120.0f
#define NAMETAG_HEIGHT 30.0f
#define NAMETAG_OPACITY 0.3f
#define NAMETAG_OUTLINE_THICKNESS 2.0f
#define NAME_SIZE 10
#define CHARACTER_FRAME_LENGTH 32
#define CHARACTER_SCALE 2.5f
#define CHARACTER_DEFAULT_SPEED 180.0f

enum CharacterType : int
{
	Girl = 0,
	Boy
};

enum CharacterAnimationType : int
{
	WalkDown = 0,
	WalkLeft,
	WalkRight,
	WalkUp
};

class Player
{
public:
	Player();
	~Player();

	void SetAttributes(std::string strName, sf::Color color, int iCharacterType, sf::Texture &texture);
	void SetId(int iId);
	int GetId();
	std::string GetName();
	sf::Color GetColor();
	int GetCharacterType();
	void AddCoin(int iCoinValue);
	void SetCoins(int iCoins);
	int GetCoins();
	void SetFont(sf::Font &font);

	sf::RectangleShape& GetNameRect();
	sf::Text& GetNameText();
	sf::Text& GetCoinsText();
	AnimatedSprite& GetSprite();

	void SetCurrentAnimation(int animationType);
	int GetCurrentAnimationType();
	void Play();
	void Update(sf::Time frameTime);
	void Stop();

	void SetPosition(float fX, float fY);
	void SetCurrentPosition(sf::Vector2f vPosition);
	sf::Vector2f GetCurrentPosition();
	void SetCurrentTime(sf::Int32 iTime);
	sf::Int32 GetCurrentTime();
	void SetPreviousPosition(sf::Vector2f vPosition);
	sf::Vector2f GetPreviousPosition();
	void SetPreviousTime(sf::Int32 iTime);
	sf::Int32 GetPreviousTime();
	void SetJoinTime(sf::Int32 iTime);
	sf::Int32 GetJoineTime();
	void Move(const sf::Vector2f& vOffset);
	float GetSpeed();
	void SetVelocity(sf::Vector2f vVelocity);
	sf::Vector2f GetVelocity();

private:
	int m_iId;
	std::string m_strName;
	sf::Color m_color;
	int m_iCharacterType;
	int m_iCurrentAnimationType;
	sf::Vector2f m_vCurrentPosition;
	sf::Int32 m_iCurrentTime;
	sf::Vector2f m_vPreviousPosition;
	sf::Int32 m_iPreviousTime;
	sf::Int32 m_iJoinTime;
	int m_iCoins;

	sf::RectangleShape m_nameRect;
	sf::Text m_nameText;
	sf::Text m_coinsText;

	Animation m_walkDownAnimation;
	Animation m_walkLeftAnimation;
	Animation m_walkRightAnimation;
	Animation m_walkUpAnimation;
	AnimatedSprite m_animatedSprite;

	float m_fSpeed;
	sf::Vector2f m_vVelocity;
};

#endif
