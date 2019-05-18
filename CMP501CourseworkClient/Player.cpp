//
// Player.cpp
// Copyright © 2018 Diel Barnes. All rights reserved.
//

#include "Player.h"

#pragma region Init

Player::Player()
{
	m_iCurrentTime = 0;
	m_iPreviousTime = 0;
	m_iCoins = 0;

	m_nameRect.setSize(sf::Vector2f(NAMETAG_WIDTH, NAMETAG_HEIGHT));
	m_nameRect.setOutlineThickness(NAMETAG_OUTLINE_THICKNESS);

	m_nameText.setFillColor(sf::Color::Black);
	m_nameText.setCharacterSize(NAME_SIZE);

	m_coinsText.setFillColor(sf::Color::Black);
	m_coinsText.setCharacterSize(NAME_SIZE);
	m_coinsText.setString("0 C");

	m_walkDownAnimation.addFrame(sf::IntRect(CHARACTER_FRAME_LENGTH, 0, CHARACTER_FRAME_LENGTH, CHARACTER_FRAME_LENGTH));
	m_walkDownAnimation.addFrame(sf::IntRect((CHARACTER_FRAME_LENGTH * 2), 0, CHARACTER_FRAME_LENGTH, CHARACTER_FRAME_LENGTH));
	m_walkDownAnimation.addFrame(sf::IntRect(CHARACTER_FRAME_LENGTH, 0, CHARACTER_FRAME_LENGTH, CHARACTER_FRAME_LENGTH));
	m_walkDownAnimation.addFrame(sf::IntRect(0, 0, CHARACTER_FRAME_LENGTH, CHARACTER_FRAME_LENGTH));

	m_walkLeftAnimation.addFrame(sf::IntRect(CHARACTER_FRAME_LENGTH, CHARACTER_FRAME_LENGTH, CHARACTER_FRAME_LENGTH, CHARACTER_FRAME_LENGTH));
	m_walkLeftAnimation.addFrame(sf::IntRect((CHARACTER_FRAME_LENGTH * 2), CHARACTER_FRAME_LENGTH, CHARACTER_FRAME_LENGTH, CHARACTER_FRAME_LENGTH));
	m_walkLeftAnimation.addFrame(sf::IntRect(CHARACTER_FRAME_LENGTH, CHARACTER_FRAME_LENGTH, CHARACTER_FRAME_LENGTH, CHARACTER_FRAME_LENGTH));
	m_walkLeftAnimation.addFrame(sf::IntRect(0, CHARACTER_FRAME_LENGTH, CHARACTER_FRAME_LENGTH, CHARACTER_FRAME_LENGTH));

	m_walkRightAnimation.addFrame(sf::IntRect(CHARACTER_FRAME_LENGTH, (CHARACTER_FRAME_LENGTH * 2), CHARACTER_FRAME_LENGTH, CHARACTER_FRAME_LENGTH));
	m_walkRightAnimation.addFrame(sf::IntRect((CHARACTER_FRAME_LENGTH * 2), (CHARACTER_FRAME_LENGTH * 2), CHARACTER_FRAME_LENGTH, CHARACTER_FRAME_LENGTH));
	m_walkRightAnimation.addFrame(sf::IntRect(CHARACTER_FRAME_LENGTH, (CHARACTER_FRAME_LENGTH * 2), CHARACTER_FRAME_LENGTH, CHARACTER_FRAME_LENGTH));
	m_walkRightAnimation.addFrame(sf::IntRect(0, (CHARACTER_FRAME_LENGTH * 2), CHARACTER_FRAME_LENGTH, CHARACTER_FRAME_LENGTH));

	m_walkUpAnimation.addFrame(sf::IntRect(CHARACTER_FRAME_LENGTH, (CHARACTER_FRAME_LENGTH * 3), CHARACTER_FRAME_LENGTH, CHARACTER_FRAME_LENGTH));
	m_walkUpAnimation.addFrame(sf::IntRect((CHARACTER_FRAME_LENGTH * 2), (CHARACTER_FRAME_LENGTH * 3), CHARACTER_FRAME_LENGTH, CHARACTER_FRAME_LENGTH));
	m_walkUpAnimation.addFrame(sf::IntRect(CHARACTER_FRAME_LENGTH, (CHARACTER_FRAME_LENGTH * 3), CHARACTER_FRAME_LENGTH, CHARACTER_FRAME_LENGTH));
	m_walkUpAnimation.addFrame(sf::IntRect(0, (CHARACTER_FRAME_LENGTH * 3), CHARACTER_FRAME_LENGTH, CHARACTER_FRAME_LENGTH));

	m_animatedSprite.setScale(CHARACTER_SCALE, CHARACTER_SCALE);

	m_fSpeed = CHARACTER_DEFAULT_SPEED;
	m_vVelocity = sf::Vector2f(-1.0f, -1.0f);
}

Player::~Player()
{
}

void Player::SetAttributes(std::string strName, sf::Color color, int iCharacterType, sf::Texture &texture)
{
	m_strName = strName;
	m_color = color;
	m_iCharacterType = iCharacterType;

	m_nameRect.setFillColor(sf::Color(m_color.r, m_color.g, m_color.b, (NAMETAG_OPACITY * 2) * 255));
	m_nameRect.setOutlineColor(m_color);

	m_nameText.setString(m_strName);
	
	m_walkDownAnimation.setSpriteSheet(texture);
	m_walkLeftAnimation.setSpriteSheet(texture);
	m_walkRightAnimation.setSpriteSheet(texture);
	m_walkUpAnimation.setSpriteSheet(texture);
}

void Player::SetId(int iId)
{
	m_iId = iId;
}

int Player::GetId()
{
	return m_iId;
}

std::string Player::GetName()
{
	return m_strName;
}

sf::Color Player::GetColor()
{
	return m_color;
}

int Player::GetCharacterType()
{
	return m_iCharacterType;
}

void Player::AddCoin(int iCoinValue)
{
	m_iCoins += iCoinValue;
	m_coinsText.setString(std::to_string(m_iCoins) + " C");
}

void Player::SetCoins(int iCoins)
{
	m_iCoins = iCoins;
	m_coinsText.setString(std::to_string(m_iCoins) + " C");
}

int Player::GetCoins()
{
	return m_iCoins;
}

void Player::SetFont(sf::Font &font)
{
	m_nameText.setFont(font);
	m_coinsText.setFont(font);
}

#pragma endregion

#pragma region Getters

sf::RectangleShape& Player::GetNameRect()
{
	return m_nameRect;
}

sf::Text& Player::GetNameText()
{
	return m_nameText;
}

sf::Text& Player::GetCoinsText()
{
	return m_coinsText;
}

AnimatedSprite& Player::GetSprite()
{
	return m_animatedSprite;
}

#pragma endregion

#pragma region Animation

void Player::SetCurrentAnimation(int animationType)
{
	m_iCurrentAnimationType = animationType;
}

int Player::GetCurrentAnimationType()
{
	return m_iCurrentAnimationType;
}

void Player::Play()
{
	switch (m_iCurrentAnimationType)
	{
	case CharacterAnimationType::WalkDown:
		m_animatedSprite.play(m_walkDownAnimation);
		break;
	case CharacterAnimationType::WalkLeft:
		m_animatedSprite.play(m_walkLeftAnimation);
		break;
	case CharacterAnimationType::WalkRight:
		m_animatedSprite.play(m_walkRightAnimation);
		break;
	case CharacterAnimationType::WalkUp:
		m_animatedSprite.play(m_walkUpAnimation);
		break;
	}
}

void Player::Update(sf::Time frameTime)
{
	m_animatedSprite.update(frameTime);
}

void Player::Stop()
{
	m_animatedSprite.stop();
}

#pragma endregion

#pragma region Movement

void Player::SetPosition(float fX, float fY)
{
	m_animatedSprite.setPosition(fX, fY);

	float fNameTagPositionX = fX - ((m_nameRect.getSize().x - (CHARACTER_FRAME_LENGTH * CHARACTER_SCALE)) / 2);
	float fNameTagPositionY = fY - m_nameRect.getSize().y - 5.0f;
	m_nameRect.setPosition(fNameTagPositionX, fNameTagPositionY);

	float fNamePositionX = fNameTagPositionX + ((m_nameRect.getSize().x - m_nameText.getLocalBounds().width) / 2);
	float fNamePositionY = fNameTagPositionY + 3.0f;
	m_nameText.setPosition(fNamePositionX, fNamePositionY);

	float fCoinsPositionX = fNameTagPositionX + ((m_nameRect.getSize().x - m_coinsText.getLocalBounds().width) / 2);
	float fCoinsPositionY = fNamePositionY + m_nameText.getLocalBounds().height + 5.0f;
	m_coinsText.setPosition(fCoinsPositionX, fCoinsPositionY);
}

void Player::SetCurrentPosition(sf::Vector2f vPosition)
{
	m_vCurrentPosition = vPosition;
}

sf::Vector2f Player::GetCurrentPosition()
{
	return m_vCurrentPosition;
}

void Player::SetCurrentTime(sf::Int32 iTime)
{
	m_iCurrentTime = iTime;
}

sf::Int32 Player::GetCurrentTime()
{
	return m_iCurrentTime;
}

void Player::SetPreviousPosition(sf::Vector2f vPosition)
{
	m_vPreviousPosition = vPosition;
}

sf::Vector2f Player::GetPreviousPosition()
{
	return m_vPreviousPosition;
}

void Player::SetPreviousTime(sf::Int32 iTime)
{
	m_iPreviousTime = iTime;
}

sf::Int32 Player::GetPreviousTime()
{
	return m_iPreviousTime;
}

void Player::SetJoinTime(sf::Int32 iTime)
{
	m_iJoinTime = iTime;
}

sf::Int32 Player::GetJoineTime()
{
	return m_iJoinTime;
}

void Player::Move(const sf::Vector2f& vOffset)
{
	m_nameRect.move(vOffset);
	m_nameText.move(vOffset);
	m_coinsText.move(vOffset);
	m_animatedSprite.move(vOffset);
}

float Player::GetSpeed()
{
	return m_fSpeed;
}

void Player::SetVelocity(sf::Vector2f vVelocity)
{
	m_vVelocity = vVelocity;
}

sf::Vector2f Player::GetVelocity()
{
	return m_vVelocity;
}

#pragma endregion
