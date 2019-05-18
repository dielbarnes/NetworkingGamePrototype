//
// Server.h
// Copyright © 2018 Diel Barnes. All rights reserved.
//

#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <stdlib.h>
#include <iostream>
#include "Player.h"

#define PORT 5555
#define TIMEOUT 5 // Seconds
#define MAX_PLAYERS 5
#define MAX_COINS 40

#define COIN_FRAME_LENGTH 32
#define COIN_SCALE 1.5f

#define FOUNTAIN_FRAME_WIDTH 64
#define FOUNTAIN_FRAME_HEIGHT 96
#define FOUNTAIN_SCALE 2.0f
#define FOUNTAIN_POS_X 608
#define FOUNTAIN_POS_Y 300
#define CHARACTER_FRAME_LENGTH 32
#define CHARACTER_SCALE 2.5f
#define CHAR_INIT_POS_X 528
#define CHAR_INIT_POS_Y 528

enum PacketType : int
{
	ConnectionAccepted = 0,
	MaxPlayersReached,
	PlayerData,
	PlayerPositionUpdated,
	PlayerDisconnected,
	CoinAwarded,
	CoinLocations
};

class Server
{
public:
	Server();
	~Server();

	void Run();

private:
	sf::TcpListener m_tcpListener;
	sf::SocketSelector m_socketSelector;

	int m_iCurrentPlayerId;
	std::vector<Player*> m_players;
	
	std::vector<std::tuple<int, int>> m_coinLocations;

	void InitializeCoins();
	sf::Packet CreateConnectionAcceptedPacket(Player &player, sf::Time time);
	void Broadcast(sf::Packet &packet, int iPacketType, int iSenderIndex, bool bSkipSender);
	void RemoveDisconnectedPlayers(std::vector<int> indices);
};

#endif
