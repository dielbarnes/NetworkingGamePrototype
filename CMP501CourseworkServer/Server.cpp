//
// Server.cpp
// Copyright © 2018 Diel Barnes. All rights reserved.
//

#include "Server.h"

#pragma region Init

Server::Server()
{
	m_iCurrentPlayerId = 0;

	m_tcpListener.listen(PORT);
	m_socketSelector.add(m_tcpListener);
	std::cout << "Server is listening..." << std::endl;
	std::cout << std::endl;
}

Server::~Server()
{
}

void Server::InitializeCoins()
{
	// Create a temporary map for coins
	int coinLocations[28][16];
	for (int i = 0; i < 28; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			if (i > 7 && i < 20 && j > 1 && j < 14)
			{
				// Center square with fountain
				// Don't put coins here
				coinLocations[i][j] = -1;
			}
			else
			{
				coinLocations[i][j] = 0;
			}
		}
	}

	// Randomize coin locations
	int iCoinsCount = 0;
	while (iCoinsCount < 40)
	{
		int i = rand() % 28; // 0 to 27
		int j = rand() % 16; // 0 to 15

		if (coinLocations[i][j] == 0)
		{
			coinLocations[i][j] = 1;

			std::tuple<int, int> tuple = std::make_tuple(i, j);
			m_coinLocations.push_back(tuple);

			iCoinsCount++;
		}
	}
}

#pragma endregion

#pragma region Run

void Server::Run()
{
	// Start time
	sf::Clock clock;
	sf::Time elapsedTime = sf::Time::Zero;

	// Set coin locations
	InitializeCoins();

	while (true)
	{
		// Update time
		elapsedTime += clock.restart();

		// Wait until a new player connects or an existing player sends data
		if (m_socketSelector.wait())
		{
			if (m_socketSelector.isReady(m_tcpListener))
			{
				// Accept new connection
				
				sf::TcpSocket* socket = new sf::TcpSocket();

				if (m_tcpListener.accept(*socket) == sf::Socket::Done)
				{
					int iPlayersCount = static_cast<int>(m_players.size());
					if (iPlayersCount < MAX_PLAYERS)
					{
						std::cout << "New player connected!" << std::endl;

						// Create player
						Player *player = new Player(m_iCurrentPlayerId, *socket);

						// Send player id, players list, and coin locations

						bool b_ShouldWaitForPlayerData = false;

						sf::Packet packet = CreateConnectionAcceptedPacket(*player, elapsedTime);

						sf::Socket::Status status = player->GetSocket().send(packet);
						if (status == sf::Socket::Done)
						{
							b_ShouldWaitForPlayerData = true;
						}
						else if(status == sf::Socket::Disconnected)
						{
							std::cout << "New player disconnected." << std::endl;
						}
						else
						{
							// Retry sending packet
							status = player->GetSocket().send(packet);
							if (status == sf::Socket::Done)
							{
								b_ShouldWaitForPlayerData = true;
							}
							else
							{
								std::cout << "Failed to send new player the id, players list, and coin locations." << std::endl;
							}
						}
						
						if (b_ShouldWaitForPlayerData)
						{
							bool b_ShouldAddPlayer = false;

							// Wait for player data

							sf::SocketSelector tempSelector;
							tempSelector.add(*socket);
							if (tempSelector.wait(sf::seconds(TIMEOUT)))
							{
								sf::Packet packetReceived;
								if ((*socket).receive(packetReceived) == sf::Socket::Done)
								{
									int iPacketType;
									packetReceived >> iPacketType;
									if (iPacketType == PacketType::PlayerData)
									{
										int iPlayerId;
										std::string strName;
										int iColorR, iColorG, iColorB;
										int iCharacterType;
										packetReceived >> iPlayerId;
										packetReceived >> strName;
										packetReceived >> iColorR;
										packetReceived >> iColorG;
										packetReceived >> iColorB;
										packetReceived >> iCharacterType;

										// Update players list
										player->SetName(strName);
										player->SetColor(sf::Vector3i(iColorR, iColorG, iColorB));
										player->SetCharacterType(iCharacterType);
										player->SetAnimationType(CharacterAnimationType::WalkDown);

										std::cout << "Packet received (type = " << iPacketType << ") from player (id = " << iPlayerId << ")." << std::endl;
										std::cout << "Name: " << strName << std::endl;
										std::cout << "Color  R: " << iColorR << "   G: " << iColorG << "   B: " << iColorB << std::endl;
										std::cout << "Character Type: " << iCharacterType << std::endl;

										// Send to other players
										packetReceived << player->GetAnimationType();
										packetReceived << player->GetPosition().x;
										packetReceived << player->GetPosition().y;
										packetReceived << player->GetTime();
										Broadcast(packetReceived, iPacketType, -1, true);

										b_ShouldAddPlayer = true;
									}
									else
									{
										std::cout << "Didn't receive data from new player. Player not added." << std::endl;
									}
								}
								else
								{
									std::cout << "Didn't receive data from new player. Player not added." << std::endl;
								}
							}
							else
							{
								std::cout << "New player timed out. Player not added." << std::endl;
							}

							if (b_ShouldAddPlayer)
							{
								// Add player
								m_players.emplace_back(player);
								m_socketSelector.add(player->GetSocket());

								std::cout << "Players count: " << iPlayersCount + 1 << std::endl;

								m_iCurrentPlayerId++;
							}
						}
						std::cout << std::endl;
					}
					else
					{
						// Maximum players reached

						//std::cout << "Player connection refused. Maximum number of players reached." << std::endl;

						sf::Packet packet;
						packet << PacketType::MaxPlayersReached;

						if (socket->send(packet) != sf::Socket::Done)
						{
							std::cout << "Failed to notify player that the server has reached maximum players." << std::endl;
						}

						//std::cout << std::endl;
					}
				}
			}
			else
			{
				// Receive data

				std::vector<int> disconnectedPlayerIndices;

				int iPlayersCount = static_cast<int>(m_players.size());
				for (int i = 0; i < iPlayersCount; i++)
				{
					if (m_socketSelector.isReady(m_players[i]->GetSocket()))
					{
						sf::Packet packetReceived;
						if (m_players[i]->GetSocket().receive(packetReceived) == sf::Socket::Done)
						{
							int iPacketType;
							int iPlayerId;
							packetReceived >> iPacketType;
							packetReceived >> iPlayerId;

							switch (iPacketType)
							{
							case PlayerPositionUpdated:
							{
								int iAnimationType;
								float fPositionX;
								float fPositionY;
								sf::Int32 iTime;
								packetReceived >> iAnimationType;
								packetReceived >> fPositionX;
								packetReceived >> fPositionY;
								packetReceived >> iTime;

								// Update players list
								m_players[i]->SetAnimationType(iAnimationType);
								m_players[i]->SetPosition(sf::Vector2f(fPositionX, fPositionY));
								m_players[i]->SetTime(iTime);

								/*std::cout << "Packet received (type = " << iPacketType << ") from player (id = " << iPlayerId << ")." << std::endl;
								std::cout << "Animation Type: " << iAnimationType << std::endl;
								std::cout << "Position  X: " << fPositionX << "   Y: " << fPositionY << std::endl;*/

								// Send to other players
								Broadcast(packetReceived, iPacketType, i, true);

								// Check for coin collision
								int iCoinsCount = static_cast<int>(m_coinLocations.size());
								if (iCoinsCount > 0)
								{
									float fPlayerFrameLength = CHARACTER_FRAME_LENGTH * CHARACTER_SCALE;
									float fCoinFrameLength = COIN_FRAME_LENGTH * COIN_SCALE;

									int iCollidedCoinIndex = -1;
									for (int j = 0; j < iCoinsCount; j++)
									{
										int index1 = std::get<0>(m_coinLocations[j]);
										int index2 = std::get<1>(m_coinLocations[j]);
										float fCoinPositionX = index1 * fCoinFrameLength;
										float fCoinPositionY = 32.0f + (index2 * fCoinFrameLength);

										sf::FloatRect playerRect(fPositionX + 10.0f, fPositionY + 10.0f, fPlayerFrameLength - 20.0f, fPlayerFrameLength - 20.0f);
										sf::FloatRect coinRect(fCoinPositionX, fCoinPositionY, fCoinFrameLength, fCoinFrameLength);
										if (playerRect.intersects(coinRect))
										{
											iCollidedCoinIndex = j;
											break;
										}
									}

									if (iCollidedCoinIndex >= 0)
									{
										// Award coin to player
										m_players[i]->AddCoin(1);

										// Inform all players
										sf::Packet packet;
										packet << PacketType::CoinAwarded;
										packet << iPlayerId;
										packet << std::get<0>(m_coinLocations[iCollidedCoinIndex]);
										packet << std::get<1>(m_coinLocations[iCollidedCoinIndex]);

										Broadcast(packet, iPacketType, i, false);

										// Remove coin from map
										m_coinLocations.erase(m_coinLocations.begin() + iCollidedCoinIndex);

										//std::cout << "Coin awarded to player (id = " << iPlayerId << ")." << std::endl;
									}
								}

								//std::cout << std::endl;
								break;
							}

							case PlayerDisconnected:
							{
								disconnectedPlayerIndices.push_back(i);

								std::cout << "Player disconnected (id = " << iPlayerId << ")." << std::endl;

								// Send to other players
								Broadcast(packetReceived, iPacketType, i, true);

								std::cout << std::endl;
								break;
							}

							case CoinLocations:
							{
								// Client requested for updated coin locations

								sf::Packet packet;
								packet << PacketType::CoinLocations;

								int iCoinsCount = static_cast<int>(m_coinLocations.size());
								packet << iCoinsCount;

								for (int j = 0; j < iCoinsCount - 1; j++)
								{
									packet << std::get<0>(m_coinLocations[j]);
									packet << std::get<1>(m_coinLocations[j]);
								}

								sf::Socket::Status status = m_players[i]->GetSocket().send(packet);
								if (status == sf::Socket::Done)
								{
									//std::cout << "Sent coin locations to player (id = " << iPlayerId << ")." << std::endl;
								}
								else if (status == sf::Socket::Disconnected)
								{
									disconnectedPlayerIndices.push_back(i);

									std::cout << "Player disconnected (id = " << iPlayerId << ")." << std::endl;

									// Send to other players
									Broadcast(packetReceived, iPacketType, i, true);
								}
								else
								{
									// Retry sending packet
									status = m_players[i]->GetSocket().send(packet);
									if (status == sf::Socket::Done)
									{
										//std::cout << "Sent coin locations to player (id = " << iPlayerId << ")." << std::endl;
									}
									else
									{
										std::cout << "Failed to send coin locations to player (id = " << iPlayerId << ")." << std::endl;
									}
								}

								//std::cout << std::endl;
								break;
							}
							} // End switch
						}
					}
				} // End for loop
				
				// Remove disconnected players
				RemoveDisconnectedPlayers(disconnectedPlayerIndices);
			}
		}
	}
}

sf::Packet Server::CreateConnectionAcceptedPacket(Player &player, sf::Time time)
{
	// Determine character's initial position

	float fCharacterFrameLength = CHARACTER_FRAME_LENGTH * CHARACTER_SCALE;
	float fPlayerPositionX = FOUNTAIN_POS_X - fCharacterFrameLength;
	float fPlayerPositionY = FOUNTAIN_POS_Y + (FOUNTAIN_FRAME_HEIGHT * FOUNTAIN_SCALE) - fCharacterFrameLength;
	int iPlayersCount = static_cast<int>(m_players.size());
	if (iPlayersCount > 0 && iPlayersCount < 3)
	{
		fPlayerPositionX += (((fCharacterFrameLength + 20.0f) * iPlayersCount) - 45.0f);
		fPlayerPositionY += fCharacterFrameLength + 20.0f;
	}
	else if (iPlayersCount == 3)
	{
		fPlayerPositionX += (FOUNTAIN_FRAME_WIDTH * FOUNTAIN_SCALE) + 85.0f;
	}
	else if (iPlayersCount > 3)
	{
		fPlayerPositionY -= (fCharacterFrameLength + 20.0f);
	}

	player.SetPosition(sf::Vector2f(fPlayerPositionX, fPlayerPositionY));

	// Determine join time
	sf::Int32 iJoinTime = time.asMilliseconds();
	player.SetTime(iJoinTime);

	// Create and send packet

	sf::Packet packet;
	packet << PacketType::ConnectionAccepted;
	packet << m_iCurrentPlayerId;
	packet << fPlayerPositionX;
	packet << fPlayerPositionY;
	packet << iJoinTime;

	packet << iPlayersCount;
	for (int i = 0; i < iPlayersCount; i++)
	{
		packet << m_players[i]->GetId();
		packet << m_players[i]->GetName();
		sf::Vector3i vColor = m_players[i]->GetColor();
		packet << vColor.x;
		packet << vColor.y;
		packet << vColor.z;
		packet << m_players[i]->GetCharacterType();
		packet << m_players[i]->GetAnimationType();
		sf::Vector2f vPosition = m_players[i]->GetPosition();
		packet << vPosition.x;
		packet << vPosition.y;
		packet << m_players[i]->GetTime();
		packet << m_players[i]->GetCoins();
	}

	int iCoinsCount = static_cast<int>(m_coinLocations.size());
	packet << iCoinsCount;
	for (int i = 0; i < iCoinsCount; i++)
	{
		packet << std::get<0>(m_coinLocations[i]);
		packet << std::get<1>(m_coinLocations[i]);
	}

	return packet;
}

void Server::Broadcast(sf::Packet &packet, int iPacketType, int iSenderIndex, bool bSkipSender)
{
	std::vector<int> disconnectedPlayerIndices;

	int iPlayersCount = static_cast<int>(m_players.size());
	for (int i = 0; i < iPlayersCount; i++)
	{
		if (i == iSenderIndex && bSkipSender)
		{
			continue;
		}
		
		sf::Socket::Status status = m_players[i]->GetSocket().send(packet);
		if (status == sf::Socket::Disconnected)
		{
			disconnectedPlayerIndices.push_back(i);

			std::cout << "Player disconnected (id = " << m_players[i]->GetId() << ")." << std::endl;

			// Send to other players
			sf::Packet packet;
			packet << PacketType::PlayerDisconnected;
			packet << m_players[i]->GetId();
			Broadcast(packet, PacketType::PlayerDisconnected, i, true);

			std::cout << std::endl;
		}
		else if (status != sf::Socket::Done)
		{
			std::cout << "Failed to send packet (type = " << iPacketType << ") to player (id = " << i << ")." << std::endl;
		}
	}

	// Remove disconnected players
	RemoveDisconnectedPlayers(disconnectedPlayerIndices);
}

void Server::RemoveDisconnectedPlayers(std::vector<int> indices)
{
	int iCount = static_cast<int>(indices.size());
	for (int i = iCount - 1; i >= 0; i--)
	{
		int index = indices[i];

		m_socketSelector.remove(m_players[index]->GetSocket());
		m_players.erase(m_players.begin() + index);
	}
}

#pragma endregion
