//
// Network.cpp
// Copyright © 2018 Diel Barnes. All rights reserved.
//

#include "Network.h"

#pragma region Init

Network::Network()
{
	m_bIsConnected = false;
	m_bConnectionFailed = false;
	m_bReceiveTimedOut = false;
	m_bConnectionRefused = false;
	m_bCoinLocationsUpdated = false;
}

Network::~Network()
{
}

void Network::SetPlayers(Player &player, std::vector<Player> &players)
{
	m_pPlayer = &player;
	m_pPlayers = &players;
}

void Network::SetFont(sf::Font &font)
{
	m_font = &font;
}

void Network::SetTextures(sf::Texture &girlTexture, sf::Texture &boyTexture)
{
	m_girlTexture = &girlTexture;
	m_boyTexture = &boyTexture;
}

#pragma endregion

#pragma region Connection

void Network::Connect(sf::IpAddress ipAddress)
{
	// Blocking but with timeout
	if (m_tcpSocket.connect(ipAddress, PORT, sf::seconds(TIMEOUT)) == sf::Socket::Done)
	{
		ReceiveWithTimeout();
	}
	else
	{
		m_bConnectionFailed = true;
	}
}

bool Network::IsConnected()
{
	return m_bIsConnected;
}

bool Network::ConnectionFailed()
{
	return m_bConnectionFailed;
}

void Network::ResetConnectionFailed()
{
	m_bConnectionFailed = false;
}

void Network::ReceiveWithTimeout()
{
	sf::SocketSelector selector;
	selector.add(m_tcpSocket);
	if (selector.wait(sf::seconds(TIMEOUT)))
	{
		sf::Packet packetReceived;
		if (m_tcpSocket.receive(packetReceived) == sf::Socket::Done)
		{
			int iPacketType;
			packetReceived >> iPacketType;

			switch (iPacketType)
			{
			case PacketType::ConnectionAccepted:
			{
				// Save player id
				int iPlayerId;
				packetReceived >> iPlayerId;
				m_pPlayer->SetId(iPlayerId);

				// Set initial position and time
				float fInitPositionX, fInitPositionY;
				sf::Int32 iJoinTime;
				packetReceived >> fInitPositionX;
				packetReceived >> fInitPositionY;
				packetReceived >> iJoinTime;
				m_pPlayer->SetCurrentPosition(sf::Vector2f(fInitPositionX, fInitPositionY));
				m_pPlayer->SetCurrentTime(iJoinTime);
				m_pPlayer->SetPreviousPosition(sf::Vector2f(fInitPositionX, fInitPositionY));
				m_pPlayer->SetPreviousTime(iJoinTime);
				m_pPlayer->SetJoinTime(iJoinTime);

				// Add players to list

				int iPlayersCount;
				packetReceived >> iPlayersCount;
				for (int i = 0; i < iPlayersCount; i++)
				{
					int iOtherPlayerId;
					std::string strName;
					int iColorR, iColorG, iColorB;
					int iCharacterType;
					int iAnimationType;
					float fPositionX, fPositionY;
					sf::Int32 iTime;
					int iCoins;
					packetReceived >> iOtherPlayerId;
					packetReceived >> strName;
					packetReceived >> iColorR;
					packetReceived >> iColorG;
					packetReceived >> iColorB;
					packetReceived >> iCharacterType;
					packetReceived >> iAnimationType;
					packetReceived >> fPositionX;
					packetReceived >> fPositionY;
					packetReceived >> iTime;
					packetReceived >> iCoins;

					Player player;
					player.SetId(iOtherPlayerId);
					player.SetFont(*m_font);
					if (iCharacterType == CharacterType::Girl)
					{
						player.SetAttributes(strName, sf::Color(iColorR, iColorG, iColorB, 255), iCharacterType, *m_girlTexture);
					}
					else
					{
						player.SetAttributes(strName, sf::Color(iColorR, iColorG, iColorB, 255), iCharacterType, *m_boyTexture);
					}
					player.SetCurrentAnimation(iAnimationType);
					player.SetPosition(fPositionX, fPositionY);
					player.SetCurrentPosition(sf::Vector2f(fPositionX, fPositionY));
					player.SetCurrentTime(iTime);
					player.SetPreviousPosition(sf::Vector2f(fPositionX, fPositionY));
					player.SetPreviousTime(iTime);
					player.SetCoins(iCoins);
					m_pPlayers->emplace_back(player);
				}

				// Get coin locations

				int iCoinsCount;
				packetReceived >> iCoinsCount;
				for (int i = 0; i < iCoinsCount; i++)
				{
					int iIndex1, iIndex2;
					packetReceived >> iIndex1;
					packetReceived >> iIndex2;

					std::tuple<int, int> tuple = std::make_tuple(iIndex1, iIndex2);
					m_coinLocations.push_back(tuple);
				}

				std::cout << "Player id (id = " << iPlayerId << "), players list, and coin locations received." << std::endl;
				std::cout << std::endl;

				m_bIsConnected = true;

				// Switch to non-blocking mode
				m_tcpSocket.setBlocking(false);

				break;
			}

			case PacketType::MaxPlayersReached:
			{
				m_bConnectionRefused = true;
				break;
			}
			} // End swicth
		} // End receive
	} // End wait
	else
	{
		m_bReceiveTimedOut = true;
	}
}

bool Network::ReceiveTimedOut()
{
	return m_bReceiveTimedOut;
}

void Network::ResetReceiveTimedOut()
{
	m_bReceiveTimedOut = false;
}

bool Network::ConnectionRefused()
{
	return m_bConnectionRefused;
}

void Network::ResetConnectionRefused()
{
	m_bConnectionRefused = false;
}

void Network::Disconnect()
{
	sf::Packet packet;
	packet << PacketType::PlayerDisconnected;
	packet << m_pPlayer->GetId();

	if (m_tcpSocket.send(packet) == sf::Socket::Done)
	{
		std::cout << "Disconnected." << std::endl;
	}
	else
	{
		std::cout << "Failed to notify the server that the player disconnected." << std::endl;
	}
	std::cout << std::endl;

	m_tcpSocket.disconnect();
	m_bIsConnected = false;

	// Switch to blocking mode
	m_tcpSocket.setBlocking(true);
}

#pragma endregion

#pragma region Send/Receive

void Network::SendPlayerData()
{
	sf::Packet packet;
	packet << PacketType::PlayerData;
	packet << m_pPlayer->GetId();
	packet << m_pPlayer->GetName();
	packet << static_cast<int>(m_pPlayer->GetColor().r);
	packet << static_cast<int>(m_pPlayer->GetColor().g);
	packet << static_cast<int>(m_pPlayer->GetColor().b);
	packet << m_pPlayer->GetCharacterType();
	//packet << m_pPlayer->GetCurrentAnimationType();
	//packet << static_cast<float>(m_pPlayer->GetSprite().getPosition().x);
	//packet << static_cast<float>(m_pPlayer->GetSprite().getPosition().y);

	sf::Socket::Status status = m_tcpSocket.send(packet);
	if (status == sf::Socket::Done)
	{
		std::cout << "Sent player data." << std::endl;
	}
	else if (status == sf::Socket::Disconnected)
	{
		m_bIsConnected = false;
		std::cout << "Lost connection to the server." << std::endl;
	}
	else
	{
		// Retry sending packet
		status = m_tcpSocket.send(packet);
		if (status == sf::Socket::Done)
		{
			std::cout << "Sent player data." << std::endl;
		}
		else
		{
			std::cout << "Failed to send player data to the server." << std::endl;
		}
	}
	std::cout << std::endl;
}

void Network::SendPlayerPosition()
{
	sf::Packet packet;
	packet << PacketType::PlayerPositionUpdated;
	packet << m_pPlayer->GetId();
	packet << m_pPlayer->GetCurrentAnimationType();
	packet << m_pPlayer->GetSprite().getPosition().x;
	packet << m_pPlayer->GetSprite().getPosition().y;
	
	/*std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	int iTime = static_cast<int>(ms.count());
	m_pPlayer->SetCurrentTime(iTime);
	packet << iTime;*/
#
	packet << m_pPlayer->GetCurrentTime();

	sf::Socket::Status status = m_tcpSocket.send(packet);
	if (status == sf::Socket::Done)
	{
		//std::cout << "Sent player animation type and position." << std::endl;
		//std::cout << std::endl;
	}
	else if (status == sf::Socket::Disconnected)
	{
		m_bIsConnected = false;
		std::cout << "Lost connection to the server." << std::endl;
	}
	else
	{
		std::cout << "Failed to send animation type and position to the server." << std::endl;
		std::cout << std::endl;
	}
}

void Network::Receive()
{
	sf::Packet packetReceived;
	if (m_tcpSocket.receive(packetReceived) == sf::Socket::Done)
	{
		int iPacketType;
		packetReceived >> iPacketType;

		switch (iPacketType)
		{
		case PlayerData:
		{
			int iPlayerId;
			std::string strName;
			int iColorR, iColorG, iColorB;
			int iCharacterType;
			int iAnimationType;
			float fPositionX, fPositionY;
			sf::Int32 iTime;
			packetReceived >> iPlayerId;
			packetReceived >> strName;
			packetReceived >> iColorR;
			packetReceived >> iColorG;
			packetReceived >> iColorB;
			packetReceived >> iCharacterType;
			packetReceived >> iAnimationType;
			packetReceived >> fPositionX;
			packetReceived >> fPositionY;
			packetReceived >> iTime;

			// Add player to list

			Player player;
			player.SetId(iPlayerId);
			player.SetFont(*m_font);
			if (iCharacterType == CharacterType::Girl)
			{
				player.SetAttributes(strName, sf::Color(iColorR, iColorG, iColorB, 255), iCharacterType, *m_girlTexture);
			}
			else
			{
				player.SetAttributes(strName, sf::Color(iColorR, iColorG, iColorB, 255), iCharacterType, *m_boyTexture);
			}
			player.SetCurrentAnimation(iAnimationType);
			player.SetPosition(fPositionX, fPositionY);
			player.SetCurrentPosition(sf::Vector2f(fPositionX, fPositionY));
			player.SetCurrentTime(iTime);
			player.SetPreviousPosition(sf::Vector2f(fPositionX, fPositionY));
			player.SetPreviousTime(iTime);
			m_pPlayers->emplace_back(player);

			std::cout << "Packet received (type = " << iPacketType << ") from player (id = " << iPlayerId << ")." << std::endl;
			std::cout << "Name: " << strName << std::endl;
			std::cout << "Color  R: " << iColorR << "   G: " << iColorG << "   B: " << iColorB << std::endl;
			std::cout << "Character Type: " << iCharacterType << std::endl;
			std::cout << "Animation Type: " << iAnimationType << std::endl;
			std::cout << "Position  X: " << fPositionX << "   Y: " << fPositionY << std::endl;
			std::cout << "Time: " << iTime << std::endl;
			std::cout << std::endl;

			break;
		}

		case PlayerPositionUpdated:
		{
			int iPlayerId;
			int iAnimationType;
			float fPositionX;
			float fPositionY;
			sf::Int32 iTime;
			packetReceived >> iPlayerId;
			packetReceived >> iAnimationType;
			packetReceived >> fPositionX;
			packetReceived >> fPositionY;
			packetReceived >> iTime;

			/*std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
			int iMyUnixTime = static_cast<int>(ms.count());
			//int iLag = iMyUnixTime - iUnixTime;
			//std::cout << "Lag: " << iLag << std::endl;*/

			// Update players list

			int index = -1;

			int iPlayersCount = static_cast<int>(m_pPlayers->size());
			for (int i = 0; i < iPlayersCount; i++)
			{
				if (iPlayerId == (*m_pPlayers)[i].GetId())
				{
					index = i;
					break;
				}
			}

			if (index > -1)
			{
				(*m_pPlayers)[index].SetCurrentAnimation(iAnimationType);
				(*m_pPlayers)[index].SetCurrentPosition(sf::Vector2f(fPositionX, fPositionY));
				(*m_pPlayers)[index].SetCurrentTime(iTime);

				/*std::cout << "Packet received (type = " << iPacketType << ") from player (id = " << iPlayerId << ")." << std::endl;
				std::cout << "Animation Type: " << iAnimationType << std::endl;
				std::cout << "Position  X: " << fPositionX << "   Y: " << fPositionY << std::endl;
				std::cout << std::endl;*/
			}

			break;
		}

		case PlayerDisconnected:
		{
			int iPlayerId;
			packetReceived >> iPlayerId;

			int index = -1;

			int iPlayersCount = static_cast<int>(m_pPlayers->size());
			for (int i = 0; i < iPlayersCount; i++)
			{
				if (iPlayerId == (*m_pPlayers)[i].GetId())
				{
					index = i;
					break;
				}
			}

			if (index > -1)
			{
				m_pPlayers->erase(m_pPlayers->begin() + index);

				//std::cout << "Player disconnected (id = " << iPlayerId << ")." << std::endl;
				//std::cout << std::endl;
			}

			break;
		}

		case CoinAwarded:
		{
			int iPlayerId;
			int iIndex1, iIndex2;
			packetReceived >> iPlayerId;
			packetReceived >> iIndex1;
			packetReceived >> iIndex2;

			// Add coin to player
			if (iPlayerId == m_pPlayer->GetId())
			{
				m_pPlayer->AddCoin(1);
			}
			else
			{
				int index = -1;

				int iPlayersCount = static_cast<int>(m_pPlayers->size());
				for (int i = 0; i < iPlayersCount; i++)
				{
					if (iPlayerId == (*m_pPlayers)[i].GetId())
					{
						index = i;
						break;
					}
				}

				if (index > -1)
				{
					(*m_pPlayers)[index].AddCoin(1);
				}
			}

			// Remove coin from map

			std::tuple<int, int> tuple = std::make_tuple(iIndex1, iIndex2);
			
			int index = -1;

			int iCoinsCount = static_cast<int>(m_coinLocations.size());
			for (int i = 0; i < iCoinsCount; i++)
			{
				if (m_coinLocations[i] == tuple)
				{
					index = i;
					break;
				}
			}

			if (index > -1)
			{
				m_coinLocations.erase(m_coinLocations.begin() + index);
			}

			m_bCoinLocationsUpdated = true;

			//std::cout << "Coin awarded to player (id = " << iPlayerId << ")." << std::endl;
			//std::cout << std::endl;

			break;
		}

		case CoinLocations:
		{
			int iCoinsCount;
			packetReceived >> iCoinsCount;

			// Update coin locations
			m_coinLocations.clear();
			for (int i = 0; i < iCoinsCount; i++)
			{
				int iIndex1, iIndex2;
				packetReceived >> iIndex1;
				packetReceived >> iIndex2;

				std::tuple<int, int> tuple = std::make_tuple(iIndex1, iIndex2);
				m_coinLocations.push_back(tuple);
			}

			//std::cout << "Coin locations updated." << std::endl;
			//std::cout << std::endl;

			break;
		}
		} // End switch
	} // End receive
}

#pragma endregion

#pragma region Coins

std::vector<std::tuple<int, int>> Network::GetCoinLocations()
{
	return m_coinLocations;
}

int Network::GetCoinsCount()
{
	return static_cast<int>(m_coinLocations.size());
}

void Network::RequestForCoinLocations()
{
	sf::Packet packet;
	packet << PacketType::CoinLocations;
	packet << m_pPlayer->GetId();

	sf::Socket::Status status = m_tcpSocket.send(packet);
	if (status == sf::Socket::Done)
	{
		//std::cout << "Requested updated coin locations." << std::endl;
		//std::cout << std::endl;
	}
	else if (status == sf::Socket::Disconnected)
	{
		m_bIsConnected = false;
		std::cout << "Lost connection to the server." << std::endl;
	}
	else
	{
		std::cout << "Failed to request updated coin locations." << std::endl;
		std::cout << std::endl;
	}

	m_bCoinLocationsUpdated = true;
}

bool Network::CoinLocationsUpdated()
{
	return m_bCoinLocationsUpdated;
}

void Network::ResetCoinLocationsUpdated()
{
	m_bCoinLocationsUpdated = false;
}

/*bool Network::HasCoin(int i, int j)
{
	std::tuple<int, int> tuple = std::make_tuple(i, j);
	return std::find(m_coinLocations.begin(), m_coinLocations.end(), tuple) != m_coinLocations.end();
}*/

#pragma endregion