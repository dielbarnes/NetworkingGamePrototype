//
// WelcomeScreen.h
// Copyright © 2018 Diel Barnes. All rights reserved.
//

#ifndef WELCOMESCREEN_H
#define WELCOMESCREEN_H

#include "Screen.h"
#include "AnimatedSprite.hpp"
#include "Player.h"

#define RESOURCE_PATH_THEME "Resources/Theme.txt"
#define RESOURCE_PATH_FONT "Resources/DejaVuSans.ttf"
#define RESOURCE_PATH_CONNECTING "Resources/connecting.png"  

#define WINDOW_PADDING 30.0f
#define WINDOW_ITEM_SPACING 15.0f
#define TEXTFIELD_WIDTH 300.0f
#define TEXTFIELD_HEIGHT 35.0f
#define TEXT_SIZE 17
#define SLIDER_WIDTH 200.0f
#define SLIDER_HEIGHT 10.0f
#define CANVAS_WIDTH 300.0f
#define OUTLINE_THICKNESS 2.0f
#define CHARACTER_SCALE_LARGE 5.0f

#define COLOR_BACKGROUND sf::Color(15, 15, 30, 255)
#define COLOR_DARK_BACKGROUND sf::Color(7, 7, 15, 255)
#define COLOR_MINT sf::Color(11, 255, 221, 255)
#define COLOR_PINK sf::Color(209, 26, 255, 255)
//#define COLOR_PURPLE sf::Color(124, 32, 215, 255)

#define DEFAULT_NAME "<name here>"
#define NAME_MAX_CHARACTERS 14

#define CONNECTING_TEXT "Connecting..."

class WelcomeScreen : public Screen
{
public:
	WelcomeScreen();
	~WelcomeScreen();

	virtual bool Initialize(sf::RenderWindow &window, Network &network);
	void SetPlayer(Player &player);
	virtual int Run();

private:
	tgui::EditBox::Ptr m_spNameTextField;
	std::string m_strName;

	tgui::Canvas::Ptr m_spColorCanvas;
	sf::Color m_color;
	tgui::Slider::Ptr m_spRedSlider;
	tgui::Label::Ptr m_spRedValueLabel;
	tgui::Slider::Ptr m_spGreenSlider;
	tgui::Label::Ptr m_spGreenValueLabel;
	tgui::Slider::Ptr m_spBlueSlider;
	tgui::Label::Ptr m_spBlueValueLabel;

	sf::Font m_font;

	Player* m_player;
	CharacterType m_characterType;

	tgui::Canvas::Ptr m_spGirlCanvas;
	sf::RectangleShape m_girlNameRect;
	sf::Text m_girlNameText;
	sf::Texture m_girlTexture;
	Animation m_girlAnimation;
	AnimatedSprite m_girlAnimatedSprite;
	sf::RectangleShape m_girlSelectedOutline;

	tgui::Canvas::Ptr m_spBoyCanvas;
	sf::RectangleShape m_boyNameRect;
	sf::Text m_boyNameText;
	sf::Texture m_boyTexture;
	Animation m_boyAnimation;
	AnimatedSprite m_boyAnimatedSprite;
	sf::RectangleShape m_boySelectedOutline;

	tgui::EditBox::Ptr m_spServerAddressTextField;
	tgui::Button::Ptr m_spPlayButton;

	tgui::Canvas::Ptr m_spConnectingCanvas;
	sf::Text m_connectingText;
	bool m_bIsConnecting;
	bool m_bShouldConnect;

	void HandleConnectionErrors();
	void UpdateName();
	void UpdateNameTagColor();
	void Draw();
};

#endif
