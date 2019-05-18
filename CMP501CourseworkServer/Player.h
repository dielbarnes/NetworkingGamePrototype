//
// Player.h
// Copyright © 2018 Diel Barnes. All rights reserved.
//

#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#define CHARACTER_FRAME_LENGTH 32
#define CHARACTER_SCALE 2.5f

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
	Player(int iId, sf::TcpSocket &tcpSocket);
	~Player();

	int GetId();
	void SetName(std::string strName);
	std::string GetName();
	void SetColor(sf::Vector3i vColor);
	sf::Vector3i GetColor();
	void SetCharacterType(int type);
	int GetCharacterType();
	void SetAnimationType(int type);
	int GetAnimationType();
	void SetPosition(sf::Vector2f vPosition);
	sf::Vector2f GetPosition();
	void SetTime(sf::Int32 iTime);
	sf::Int32 GetTime();
	void AddCoin(int iCoinValue);
	int GetCoins();

	sf::TcpSocket& GetSocket();

private:
	int m_iId;
	std::string m_strName;
	sf::Vector3i m_vColor;
	int m_iCharacterType;
	int m_iAnimationType;
	sf::Vector2f m_vPosition;
	sf::Int32 m_iTime;
	int m_iCoins;

	sf::TcpSocket* m_tcpSocket;
};

#endif
