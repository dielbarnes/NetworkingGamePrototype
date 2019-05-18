//
// Utils.cpp
// Copyright © 2018 Diel Barnes. All rights reserved.
//

#include <windows.h>
#include "Utils.h"

void Utils::ShowMessage(std::string strCustomMessage, const char* pSystemMessage)
{
	std::string text;
	text = (pSystemMessage == nullptr) ? strCustomMessage : strCustomMessage + "\n\n" + std::string(pSystemMessage);

	MessageBox(0, text.c_str(), "", 0);
}

bool Utils::StringIsIpAddress(const std::string &rString)
{
	if (rString.length() < 7)
	{
		return false;
	}
	
	std::string::const_iterator iterator = rString.begin();
	while (iterator != rString.end() && (std::isdigit(*iterator) || '.' == *iterator))
	{
		++iterator;
	}
	return iterator == rString.end();
}
