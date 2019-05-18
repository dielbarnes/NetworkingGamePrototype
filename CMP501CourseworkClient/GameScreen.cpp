//
// GameScreen.cpp
// Copyright © 2018 Diel Barnes. All rights reserved.
//

#include "GameScreen.h"

#pragma region Init

GameScreen::GameScreen()
{
	m_bWindowHasFocus = true;
	m_bNoKeyPressed = true;
}

GameScreen::~GameScreen()
{
}

bool GameScreen::Initialize(sf::RenderWindow &window, Network &network)
{
	m_pWindow = &window;
	m_pNetwork = &network;

	// 9x9
	/*const int tileIndices[] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 6, 7, 8, 0, 0, 0,
		0, 0, 6, 1, 12, 4, 8, 0, 0,
		0, 6, 1, 12, 12, 12, 4, 8, 0,
		0, 11, 12, 12, 12, 12, 12, 13, 0,
		0, 16, 2, 12, 12, 12, 3, 18, 0,
		0, 0, 16, 2, 12, 3, 18, 0, 0,
		0, 0, 0, 16, 17, 18, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0,
	};*/

	// 21x13
	const int tileIndices[] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 7, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 6, 1, 12, 4, 8, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 6, 1, 12, 12, 12, 4, 8, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 11, 12, 12, 12, 12, 12, 13, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 16, 2, 12, 12, 12, 3, 18, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 16, 2, 12, 3, 18, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 17, 18, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};

	if (!m_map.load(RESOURCE_PATH_TILES, sf::Vector2u(TILE_LENGTH, TILE_LENGTH), tileIndices, 21, 13))
	{
		Utils::ShowMessage("Failed to load map.", nullptr);
		return false;
	}

	m_fountainTexture.loadFromFile(RESOURCE_PATH_FOUNTAIN);

	m_fountainAnimation.setSpriteSheet(m_fountainTexture);
	m_fountainAnimation.addFrame(sf::IntRect(FOUNTAIN_FRAME_WIDTH, 0, FOUNTAIN_FRAME_WIDTH, FOUNTAIN_FRAME_HEIGHT));
	m_fountainAnimation.addFrame(sf::IntRect(0, 0, FOUNTAIN_FRAME_WIDTH, FOUNTAIN_FRAME_HEIGHT));

	float fFountainPositionX = (m_pWindow->getSize().x - (FOUNTAIN_FRAME_WIDTH * FOUNTAIN_SCALE)) / 2;
	float fFountainPositionY = ((m_pWindow->getSize().y - (FOUNTAIN_FRAME_HEIGHT * FOUNTAIN_SCALE)) / 2) - 20.0f;

	m_fountainAnimatedSprite.setPosition(sf::Vector2f(fFountainPositionX, fFountainPositionY));
	m_fountainAnimatedSprite.setScale(sf::Vector2f(FOUNTAIN_SCALE, FOUNTAIN_SCALE));

	m_coinTexture.loadFromFile(RESOURCE_PATH_COIN);

	m_coinAnimation.setSpriteSheet(m_coinTexture);
	m_coinAnimation.addFrame(sf::IntRect(0, 0, COIN_FRAME_LENGTH, COIN_FRAME_LENGTH));
	m_coinAnimation.addFrame(sf::IntRect(COIN_FRAME_LENGTH, 0, COIN_FRAME_LENGTH, COIN_FRAME_LENGTH));
	m_coinAnimation.addFrame(sf::IntRect((COIN_FRAME_LENGTH * 2), 0, COIN_FRAME_LENGTH, COIN_FRAME_LENGTH));
	m_coinAnimation.addFrame(sf::IntRect((COIN_FRAME_LENGTH * 3), 0, COIN_FRAME_LENGTH, COIN_FRAME_LENGTH));
	m_coinAnimation.addFrame(sf::IntRect((COIN_FRAME_LENGTH * 4), 0, COIN_FRAME_LENGTH, COIN_FRAME_LENGTH));
	m_coinAnimation.addFrame(sf::IntRect((COIN_FRAME_LENGTH * 5), 0, COIN_FRAME_LENGTH, COIN_FRAME_LENGTH));
	m_coinAnimation.addFrame(sf::IntRect((COIN_FRAME_LENGTH * 6), 0, COIN_FRAME_LENGTH, COIN_FRAME_LENGTH));
	m_coinAnimation.addFrame(sf::IntRect((COIN_FRAME_LENGTH * 7), 0, COIN_FRAME_LENGTH, COIN_FRAME_LENGTH));

	for (int i = 0; i < MAX_COINS; i++)
	{
		m_coins[i].setScale(sf::Vector2f(COIN_SCALE, COIN_SCALE));
	}

	return true;
}

void GameScreen::SetPlayers(Player &player, std::vector<Player> &players)
{
	m_pPlayer = &player;
	m_pPlayers = &players;
}

#pragma endregion

#pragma region Run

int GameScreen::Run()
{
	// Initialize time
	sf::Clock clock;
	sf::Time elapsedTime = sf::milliseconds(m_pPlayer->GetJoineTime());
	sf::Time lastCoinLocationsRequestTime = sf::Time::Zero;

	// Set coin locations
	UpdateCoinLocations();

	// Set initial animation
	m_pPlayer->SetCurrentAnimation(CharacterAnimationType::WalkLeft);
	m_pPlayer->Play();
	m_pPlayer->SetCurrentAnimation(CharacterAnimationType::WalkDown);
	m_pPlayer->Play();

	// Set initial character position
	/*float fCharacterFrameLength = CHARACTER_FRAME_LENGTH * CHARACTER_SCALE;
	float fPlayerPositionX = m_fountainAnimatedSprite.getPosition().x - fCharacterFrameLength;
	float fPlayerPositionY = m_fountainAnimatedSprite.getPosition().y + (FOUNTAIN_FRAME_HEIGHT * FOUNTAIN_SCALE) - fCharacterFrameLength;
	int iPlayersCount = static_cast<int>(m_pPlayers->size());
	if (iPlayersCount == 0)
	{
		m_pPlayer->SetPosition(fPlayerPositionX, fPlayerPositionY);
	}
	else if (iPlayersCount < 3) 
	{
		m_pPlayer->SetPosition(fPlayerPositionX + ((fCharacterFrameLength + 20.0f) * iPlayersCount) - 45.0f, fPlayerPositionY + fCharacterFrameLength + 20.0f);
	}
	else if (iPlayersCount == 3)
	{
		m_pPlayer->SetPosition(fPlayerPositionX + (FOUNTAIN_FRAME_WIDTH * FOUNTAIN_SCALE) + 85.0f, fPlayerPositionY);
	}
	else
	{
		m_pPlayer->SetPosition(fPlayerPositionX, fPlayerPositionY - fCharacterFrameLength - 20.0f);
	}*/

	float fInitPositionX = m_pPlayer->GetCurrentPosition().x;
	float fInitPositionY = m_pPlayer->GetCurrentPosition().y;
	m_pPlayer->SetPosition(fInitPositionX, fInitPositionY);
	
	// Send player data to server
	m_pNetwork->SendPlayerData();

	// Run game loop
	while (m_pWindow->isOpen())
	{
		// Update time
		sf::Time frameTime = clock.restart();
		elapsedTime += frameTime;

		// Handle network events
		if (m_pNetwork->CoinLocationsUpdated())
		{
			UpdateCoinLocations();
			m_pNetwork->CoinLocationsUpdated();
		}
		else if (!m_pNetwork->IsConnected())
		{
			Utils::ShowMessage("Lost connection to server.", nullptr);

			// Go back to welcome screen
			return ScreenIndex::WelcomeScreenIndex;
		}

		if (elapsedTime - lastCoinLocationsRequestTime >= sf::milliseconds(COIN_LOCATIONS_REQUEST_INTERVAL))
		{
			m_pNetwork->RequestForCoinLocations();
			lastCoinLocationsRequestTime = elapsedTime;
		}

		// Handle window events
		sf::Event event;
		while (m_pWindow->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				m_pNetwork->Disconnect();
				m_pWindow->close();
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			{
				// Go back to welcome screen
				m_pNetwork->Disconnect();
				return ScreenIndex::WelcomeScreenIndex;
			}
			else if (event.type == sf::Event::GainedFocus)
			{
				m_bWindowHasFocus = true;
			}
			else if (event.type == sf::Event::LostFocus)
			{
				m_bWindowHasFocus = false;
			}
		}

		// Animate fountain
		m_fountainAnimatedSprite.play(m_fountainAnimation);
		m_fountainAnimatedSprite.update(frameTime);

		// Animate coins
		for (int i = 0; i < MAX_COINS; i++)
		{
			m_coins[i].play(m_coinAnimation);
			m_coins[i].update(frameTime);
		}

		// Animate character
		m_pPlayer->Play();
		if (m_bWindowHasFocus)
		{
			AnimateCharacter(frameTime, elapsedTime);
		}
		else
		{
			m_pPlayer->Stop();
		}
		m_pPlayer->Update(frameTime);

		// Send and receive data
		if (m_pPlayer->GetSprite().isPlaying())
		{
			m_pNetwork->SendPlayerPosition();
		}
		m_pNetwork->Receive();

		// Animate other players
		AnimateOtherPlayers(frameTime, elapsedTime);
		
		// Draw
		m_pWindow->clear();
		Draw();
		m_pWindow->display();
	}

	return -1;
}

#pragma endregion

#pragma region Update/Draw

void GameScreen::UpdateCoinLocations()
{
	int iCoinIndex = 0;
	float fCoinFrameLength = COIN_FRAME_LENGTH * COIN_SCALE;
	std::vector<std::tuple<int, int>> coinLocations = m_pNetwork->GetCoinLocations();
	int iCoinsCount = static_cast<int>(coinLocations.size());
	for (int i = 0; i < iCoinsCount; i++)
	{
		int index1 = std::get<0>(coinLocations[i]);
		int index2 = std::get<1>(coinLocations[i]);

		float fX = index1 * fCoinFrameLength;
		float fY = 32.0f + (index2 * fCoinFrameLength);
		m_coins[iCoinIndex].setPosition(fX, fY);

		iCoinIndex++;
		if (iCoinIndex == MAX_COINS)
		{
			break;
		}
	}
}

void GameScreen::AnimateCharacter(sf::Time frameTime, sf::Time elapsedTime)
{
	sf::Vector2f vMovement(0.f, 0.f);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		m_pPlayer->SetCurrentAnimation(CharacterAnimationType::WalkUp);
		vMovement.y -= m_pPlayer->GetSpeed();
		m_bNoKeyPressed = false;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		m_pPlayer->SetCurrentAnimation(CharacterAnimationType::WalkDown);
		vMovement.y += m_pPlayer->GetSpeed();
		m_bNoKeyPressed = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		m_pPlayer->SetCurrentAnimation(CharacterAnimationType::WalkLeft);
		vMovement.x -= m_pPlayer->GetSpeed();
		m_bNoKeyPressed = false;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		m_pPlayer->SetCurrentAnimation(CharacterAnimationType::WalkRight);
		vMovement.x += m_pPlayer->GetSpeed();
		m_bNoKeyPressed = false;
	}

	if (m_bNoKeyPressed)
	{
		m_pPlayer->Stop();
	}
	else
	{
		const sf::Vector2f& vOffset = vMovement * frameTime.asSeconds();
		const sf::Vector2f& vFuturePosition = m_pPlayer->GetSprite().getPosition() + vOffset;
		float fFuturePlayerMaxX = vFuturePosition.x + m_pPlayer->GetSprite().getGlobalBounds().width;
		float fFuturePlayerMaxY = vFuturePosition.y + m_pPlayer->GetSprite().getGlobalBounds().height;

		// Check if movement is within screen
		if (vFuturePosition.x >= 0.0f && fFuturePlayerMaxX <= m_pWindow->getSize().x &&
			vFuturePosition.y >= 0.0f && fFuturePlayerMaxY <= m_pWindow->getSize().y)
		{
			float fFountainMaxX = m_fountainAnimatedSprite.getPosition().x + m_fountainAnimatedSprite.getGlobalBounds().width;
			float fFountainMaxY = m_fountainAnimatedSprite.getPosition().y + m_fountainAnimatedSprite.getGlobalBounds().height;
			
			// Check if character will collide with fountain
			if (fFuturePlayerMaxY < m_fountainAnimatedSprite.getPosition().y + 20.0f || // Top
				fFuturePlayerMaxX < m_fountainAnimatedSprite.getPosition().x + 25.0f || // Left
				vFuturePosition.y > fFountainMaxY - 90.0f || // Bottom
				vFuturePosition.x > fFountainMaxX - 25.0f || // Right
				(vFuturePosition.y > fFountainMaxY - 120.0f && fFuturePlayerMaxX < m_fountainAnimatedSprite.getPosition().x + 40.0f) || // Lower left
				(vFuturePosition.y > fFountainMaxY - 120.0f && vFuturePosition.x > fFountainMaxX - 40.0f) || // Lower right
				(fFuturePlayerMaxY < m_fountainAnimatedSprite.getPosition().y + 100.0f && fFuturePlayerMaxX < m_fountainAnimatedSprite.getPosition().x + 45.0f) || // Upper left
				(fFuturePlayerMaxY < m_fountainAnimatedSprite.getPosition().y + 100.0f && vFuturePosition.x > fFountainMaxX - 45.0f) || // Upper right
				(fFuturePlayerMaxY < m_fountainAnimatedSprite.getPosition().y + 45.0f && fFuturePlayerMaxX < m_fountainAnimatedSprite.getPosition().x + 70.0f) || // Uppermost left
				(fFuturePlayerMaxY < m_fountainAnimatedSprite.getPosition().y + 45.0f && vFuturePosition.x > fFountainMaxX - 70.0f)) // Uppermost right
			{
				// Check if character will collide with other players
				
				/*bool bWillCollide = false;
				float characterFrameLength = CHARACTER_FRAME_LENGTH * CHARACTER_SCALE;
				sf::FloatRect futurePlayerRect(vFuturePosition.x, vFuturePosition.y, characterFrameLength, characterFrameLength);
				
				int iPlayersCount = static_cast<int>(m_pPlayers->size());
				for (int i = 0; i < iPlayersCount; i++)
				{
					sf::FloatRect otherPlayerRect = (*m_pPlayers)[i].GetSprite().getGlobalBounds();
					if (futurePlayerRect.intersects(otherPlayerRect))
					{
						bWillCollide = true;
						break;
					}
				}

				if (!bWillCollide)
				{
					m_pPlayer->Move(vOffset);
				}*/

				m_pPlayer->Move(vOffset);
				m_pPlayer->SetCurrentTime(elapsedTime.asMilliseconds());
			}
		}
	}

	m_bNoKeyPressed = true;
}

void GameScreen::AnimateOtherPlayers(sf::Time frameTime, sf::Time elapsedTime)
{
	int iPlayersCount = static_cast<int>(m_pPlayers->size());
	for (int i = 0; i < iPlayersCount; i++)
	{
		(*m_pPlayers)[i].Play();

		sf::Vector2f vCurrentPosition = (*m_pPlayers)[i].GetCurrentPosition();
		sf::Vector2f vPreviousPosition = (*m_pPlayers)[i].GetPreviousPosition();
		sf::Vector2f vDeltaPosition = vCurrentPosition - vPreviousPosition;

		sf::Int32 iCurrentTime = (*m_pPlayers)[i].GetCurrentTime();
		sf::Int32 iPreviousTime = (*m_pPlayers)[i].GetPreviousTime();
		sf::Int32 iDeltaTime = iCurrentTime - iPreviousTime;

		/*std::cout << "Current Pos X: " << vCurrentPosition.x << "   Y: " << vCurrentPosition.y << std::endl;
		std::cout << "Previous Pos X: " << vPreviousPosition.x << "   Y: " << vPreviousPosition.y << std::endl;
		std::cout << "Delta Pos X: " << vDeltaPosition.x << "   Y: " << vDeltaPosition.y << std::endl;

		std::cout << "Current Time: " << iCurrentTime << std::endl;
		std::cout << "Previous Time: " << iPreviousTime << std::endl;
		std::cout << "Delta Time: " << iDeltaTime << std::endl;*/

		if (iDeltaTime > 0)
		{
			sf::Vector2f vVelocity = sf::Vector2f(vDeltaPosition.x / iDeltaTime, vDeltaPosition.y / iDeltaTime);
			//std::cout << "Velocity X: " << vVelocity.x << "   Y: " << vVelocity.y << std::endl;
			(*m_pPlayers)[i].SetVelocity(vVelocity);

			if (vVelocity == sf::Vector2f(0.0f, 0.0f))
			{
				(*m_pPlayers)[i].Stop();
			}

			/*sf::Int32 lag = elapsedTime.asMilliseconds() - iCurrentTime;
			std::cout << "Lag: " << lag << std::endl;
			if (lag > -2700)
			{
				std::cout << "Predict" << std::endl;
				(*m_pPlayers)[i].SetPosition(vCurrentPosition.x + (vVelocity.x*2), vCurrentPosition.y + (vVelocity.y*2));
			}
			else
			{
				std::cout << "Normal" << std::endl;
				(*m_pPlayers)[i].SetPosition(vCurrentPosition.x + vVelocity.x, vCurrentPosition.y + vVelocity.y);
			}*/

			(*m_pPlayers)[i].SetPosition(vCurrentPosition.x + vVelocity.x, vCurrentPosition.y + vVelocity.y);
			(*m_pPlayers)[i].SetPreviousPosition(vCurrentPosition);
			(*m_pPlayers)[i].SetPreviousTime(iCurrentTime);
		}
		else
		{
			if (vDeltaPosition == sf::Vector2f(0.0f, 0.0f))
			{
				//std::cout << "Stop" << std::endl;
				(*m_pPlayers)[i].Stop();
			}
			else
			{
				//std::cout << "Move" << std::endl;
				(*m_pPlayers)[i].Move(vDeltaPosition);
				(*m_pPlayers)[i].SetPreviousPosition(vCurrentPosition);
				(*m_pPlayers)[i].SetPreviousTime(iCurrentTime);
			}
		}

		(*m_pPlayers)[i].Update(frameTime);
	}
}

void GameScreen::Draw()
{
	// Draw map
	m_pWindow->draw(m_map);
	m_pWindow->draw(m_fountainAnimatedSprite);
	
	// Draw coins
	int iCoinsCount = m_pNetwork->GetCoinsCount();
	for (int i = 0; i < iCoinsCount; i++)
	{
		m_pWindow->draw(m_coins[i]);
	}

	// Draw players

	int iPlayersCount = static_cast<int>(m_pPlayers->size());
	for (int i = 0; i < iPlayersCount; i++)
	{
		m_pWindow->draw((*m_pPlayers)[i].GetSprite());
		//std::cout << "Draw Pos X: " << (*m_pPlayers)[i].GetSprite().getPosition().x << "   Y: " << (*m_pPlayers)[i].GetSprite().getPosition().y << std::endl;
	}

	m_pWindow->draw(m_pPlayer->GetSprite());

	for (int i = 0; i < iPlayersCount; i++)
	{
		m_pWindow->draw((*m_pPlayers)[i].GetNameRect());
		m_pWindow->draw((*m_pPlayers)[i].GetNameText());
		m_pWindow->draw((*m_pPlayers)[i].GetCoinsText());
	}

	m_pWindow->draw(m_pPlayer->GetNameRect());
	m_pWindow->draw(m_pPlayer->GetNameText());
	m_pWindow->draw(m_pPlayer->GetCoinsText());
}

#pragma endregion
