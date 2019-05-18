//
// Network.h
// Copyright © 2018 Diel Barnes. All rights reserved.
//

#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <chrono>
#include <SFML/Network.hpp>
#include "Player.h"

#define DEFAULT_IP_ADDRESS "192.168.0.34"
#define PORT 5555
#define TIMEOUT 10 // Seconds
#define COIN_LOCATIONS_REQUEST_INTERVAL 500 // Milliseconds

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

class Network
{
public:
	Network();
	~Network();

	void SetPlayers(Player &player, std::vector<Player> &players);
	void SetFont(sf::Font &font);
	void SetTextures(sf::Texture &girlTexture, sf::Texture &boyTexture);

	void Connect(sf::IpAddress ipAddress);
	bool IsConnected();
	bool ConnectionFailed();
	void ResetConnectionFailed();
	bool ReceiveTimedOut();
	void ResetReceiveTimedOut();
	bool ConnectionRefused();
	void ResetConnectionRefused();
	void Disconnect();

	void SendPlayerData();
	void SendPlayerPosition();
	void Receive();

	std::vector<std::tuple<int, int>> GetCoinLocations();
	int GetCoinsCount();
	void RequestForCoinLocations();
	bool CoinLocationsUpdated();
	void ResetCoinLocationsUpdated();

private:
	sf::TcpSocket m_tcpSocket;
	bool m_bIsConnected;
	bool m_bConnectionFailed;
	bool m_bReceiveTimedOut;
	bool m_bConnectionRefused;
	bool m_bCoinLocationsUpdated;

	std::vector<std::tuple<int, int>> m_coinLocations;

	Player* m_pPlayer;
	std::vector<Player>* m_pPlayers;

	sf::Font* m_font;
	sf::Texture* m_girlTexture;
	sf::Texture* m_boyTexture;

	void ReceiveWithTimeout();
};

#endif
