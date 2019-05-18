//
// Player.cpp
// Copyright © 2018 Diel Barnes. All rights reserved.
//

#include "Player.h"

#pragma region Init

Player::Player(int iId, sf::TcpSocket &tcpSocket)
{
	m_iId = iId;
	m_iTime = 0;
	m_iCoins = 0;

	m_tcpSocket = &tcpSocket;
}

Player::~Player()
{
}

#pragma endregion

#pragma region Player Data

int Player::GetId()
{
	return m_iId;
}

void Player::SetName(std::string strName)
{
	m_strName = strName;
}

std::string Player::GetName()
{
	return m_strName;
}

void Player::SetColor(sf::Vector3i vColor)
{
	m_vColor = vColor;
}

sf::Vector3i Player::GetColor()
{
	return m_vColor;
}

void Player::SetCharacterType(int type)
{
	m_iCharacterType = type;
}

int Player::GetCharacterType()
{
	return m_iCharacterType;
}

void Player::SetAnimationType(int type)
{
	m_iAnimationType = type;
}

int Player::GetAnimationType()
{
	return m_iAnimationType;
}

void Player::SetPosition(sf::Vector2f vPosition)
{
	m_vPosition = vPosition;
}

sf::Vector2f Player::GetPosition()
{
	return m_vPosition;
}

void Player::SetTime(sf::Int32 iTime)
{
	m_iTime = iTime;
}

sf::Int32 Player::GetTime()
{
	return m_iTime;
}

void Player::AddCoin(int iCoinValue)
{
	m_iCoins += iCoinValue;
}

int Player::GetCoins()
{
	return m_iCoins;
}

#pragma endregion

#pragma region Socket

sf::TcpSocket& Player::GetSocket()
{
	return *m_tcpSocket;
}

#pragma endregion
