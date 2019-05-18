//
// WelcomeScreen.cpp
// Copyright © 2018 Diel Barnes. All rights reserved.
//

#include "WelcomeScreen.h"

#pragma region Init

WelcomeScreen::WelcomeScreen()
{
	m_strName = DEFAULT_NAME;
	m_color = COLOR_MINT;
	m_characterType = CharacterType::Girl;
	m_bIsConnecting = false;
	m_bShouldConnect = false;
}

WelcomeScreen::~WelcomeScreen()
{
	SAFE_DELETE(m_pGui);
}

bool WelcomeScreen::Initialize(sf::RenderWindow &window, Network &network)
{
	m_pWindow = &window;
	m_pGui = new tgui::Gui(window);
	m_pNetwork = &network;

	try
	{
		// Load theme
		tgui::Theme theme{ RESOURCE_PATH_THEME };

		// Add UI elements for entering player name

		auto label = tgui::Label::create();
		label->setRenderer(theme.getRenderer("Label"));
		label->setText("Enter Player Name");
		label->setTextSize(TEXT_SIZE);
		label->setPosition((window.getSize().x - label->getSize().x) / 2, WINDOW_PADDING - 2.0f);
		m_pGui->add(label);

		m_spNameTextField = tgui::EditBox::create();
		m_spNameTextField->setRenderer(theme.getRenderer("EditBox"));
		m_spNameTextField->setPosition((window.getSize().x - TEXTFIELD_WIDTH) / 2, label->getPosition().y + label->getSize().y + WINDOW_ITEM_SPACING);
		m_spNameTextField->setSize(TEXTFIELD_WIDTH, TEXTFIELD_HEIGHT);
		m_spNameTextField->setTextSize(TEXT_SIZE);
		m_spNameTextField->setDefaultText(DEFAULT_NAME);
		m_spNameTextField->setAlignment(tgui::EditBox::Alignment::Center);
		m_spNameTextField->setMaximumCharacters(NAME_MAX_CHARACTERS);
		m_spNameTextField->connect("TextChanged", [&]()
		{
			m_strName = m_spNameTextField->getText();
			if (m_strName.empty())
			{
				m_strName = DEFAULT_NAME;
			}
			UpdateName();
		});
		m_pGui->add(m_spNameTextField);

		// Add UI elements for selecting name color

		float fTopSpace = m_spNameTextField->getPosition().y + TEXTFIELD_HEIGHT + WINDOW_ITEM_SPACING + 10.0f;
		float fColorCanvasSideLength = TEXTFIELD_HEIGHT;

		label = tgui::Label::create();
		label->setRenderer(theme.getRenderer("Label"));
		label->setText("Select Color");
		label->setTextSize(TEXT_SIZE);
		label->setPosition((window.getSize().x - label->getSize().x - fColorCanvasSideLength - WINDOW_ITEM_SPACING) / 2, fTopSpace + 7.0f);
		m_pGui->add(label);

		m_spColorCanvas = tgui::Canvas::create({fColorCanvasSideLength, fColorCanvasSideLength});
		m_spColorCanvas->setPosition(label->getPosition().x + label->getSize().x + WINDOW_ITEM_SPACING, fTopSpace);
		m_pGui->add(m_spColorCanvas);

		m_spRedSlider = tgui::Slider::create();
		m_spRedSlider->setRenderer(theme.getRenderer("Slider"));
		m_spRedSlider->setPosition((window.getSize().x - SLIDER_WIDTH) / 2, m_spColorCanvas->getPosition().y + fColorCanvasSideLength + WINDOW_ITEM_SPACING);
		m_spRedSlider->setSize(SLIDER_WIDTH, SLIDER_HEIGHT);
		m_spRedSlider->setMinimum(0);
		m_spRedSlider->setMaximum(255);
		m_spRedSlider->setStep(1);
		m_spRedSlider->setValue(m_color.r);
		m_spRedSlider->connect("ValueChanged", [&]()
		{
			m_color.r = m_spRedSlider->getValue();
			m_spRedValueLabel->setText(std::to_string(m_color.r));
			UpdateNameTagColor();
		});
		m_pGui->add(m_spRedSlider);

		label = tgui::Label::create();
		label->setRenderer(theme.getRenderer("Label"));
		label->setText("R");
		label->setTextSize(TEXT_SIZE);
		label->setPosition(m_spRedSlider->getPosition().x - label->getSize().x - WINDOW_ITEM_SPACING, m_spRedSlider->getPosition().y - 6.0f);
		m_pGui->add(label);

		m_spRedValueLabel = tgui::Label::create();
		m_spRedValueLabel->setRenderer(theme.getRenderer("Label"));
		m_spRedValueLabel->setText(std::to_string(m_color.r));
		m_spRedValueLabel->setTextSize(TEXT_SIZE);
		m_spRedValueLabel->setPosition(m_spRedSlider->getPosition().x + SLIDER_WIDTH + WINDOW_ITEM_SPACING, m_spRedSlider->getPosition().y - 6.0f);
		m_pGui->add(m_spRedValueLabel);

		m_spGreenSlider = tgui::Slider::create();
		m_spGreenSlider->setRenderer(theme.getRenderer("Slider"));
		m_spGreenSlider->setPosition((window.getSize().x - SLIDER_WIDTH) / 2, m_spRedSlider->getPosition().y + SLIDER_HEIGHT + WINDOW_ITEM_SPACING + 10.0f);
		m_spGreenSlider->setSize(SLIDER_WIDTH, SLIDER_HEIGHT);
		m_spGreenSlider->setMinimum(0);
		m_spGreenSlider->setMaximum(255);
		m_spGreenSlider->setStep(1);
		m_spGreenSlider->setValue(m_color.g);
		m_spGreenSlider->connect("ValueChanged", [&]()
		{
			m_color.g = m_spGreenSlider->getValue();
			m_spGreenValueLabel->setText(std::to_string(m_color.g));
			UpdateNameTagColor();
		});
		m_pGui->add(m_spGreenSlider);

		label = tgui::Label::create();
		label->setRenderer(theme.getRenderer("Label"));
		label->setText("G");
		label->setTextSize(TEXT_SIZE);
		label->setPosition(m_spGreenSlider->getPosition().x - label->getSize().x - WINDOW_ITEM_SPACING, m_spGreenSlider->getPosition().y - 6.0f);
		m_pGui->add(label);

		m_spGreenValueLabel = tgui::Label::create();
		m_spGreenValueLabel->setRenderer(theme.getRenderer("Label"));
		m_spGreenValueLabel->setText(std::to_string(m_color.g));
		m_spGreenValueLabel->setTextSize(TEXT_SIZE);
		m_spGreenValueLabel->setPosition(m_spGreenSlider->getPosition().x + SLIDER_WIDTH + WINDOW_ITEM_SPACING, m_spGreenSlider->getPosition().y - 6.0f);
		m_pGui->add(m_spGreenValueLabel);

		m_spBlueSlider = tgui::Slider::create();
		m_spBlueSlider->setRenderer(theme.getRenderer("Slider"));
		m_spBlueSlider->setPosition((window.getSize().x - SLIDER_WIDTH) / 2, m_spGreenSlider->getPosition().y + SLIDER_HEIGHT + WINDOW_ITEM_SPACING + 10.0f);
		m_spBlueSlider->setSize(SLIDER_WIDTH, SLIDER_HEIGHT);
		m_spBlueSlider->setMinimum(0);
		m_spBlueSlider->setMaximum(255);
		m_spBlueSlider->setStep(1);
		m_spBlueSlider->setValue(m_color.b);
		m_spBlueSlider->connect("ValueChanged", [&]()
		{
			m_color.b = m_spBlueSlider->getValue();
			m_spBlueValueLabel->setText(std::to_string(m_color.b));
			UpdateNameTagColor();
		});
		m_pGui->add(m_spBlueSlider);

		label = tgui::Label::create();
		label->setRenderer(theme.getRenderer("Label"));
		label->setText("B");
		label->setTextSize(TEXT_SIZE);
		label->setPosition(m_spBlueSlider->getPosition().x - label->getSize().x - WINDOW_ITEM_SPACING, m_spBlueSlider->getPosition().y - 6.0f);
		m_pGui->add(label);

		m_spBlueValueLabel = tgui::Label::create();
		m_spBlueValueLabel->setRenderer(theme.getRenderer("Label"));
		m_spBlueValueLabel->setText(std::to_string(m_color.b));
		m_spBlueValueLabel->setTextSize(TEXT_SIZE);
		m_spBlueValueLabel->setPosition(m_spBlueSlider->getPosition().x + SLIDER_WIDTH + WINDOW_ITEM_SPACING, m_spBlueSlider->getPosition().y - 6.0f);
		m_pGui->add(m_spBlueValueLabel);

		// Add play button
		float fPlayButtonWidth = 120.0;
		float fPlayButtonHeight = 45.0;
		m_spPlayButton = tgui::Button::create();
		m_spPlayButton->setRenderer(theme.getRenderer("Button"));
		m_spPlayButton->setPosition((window.getSize().x - fPlayButtonWidth) / 2, window.getSize().y - fPlayButtonHeight - WINDOW_PADDING);
		m_spPlayButton->setSize(fPlayButtonWidth, fPlayButtonHeight);
		m_spPlayButton->setText("P L A Y");
		m_spPlayButton->setTextSize(20);
		m_spPlayButton->connect("Pressed", [&]()
		{
			std::string strServerAddress = m_spServerAddressTextField->getText();
			if (m_strName == DEFAULT_NAME)
			{
				Utils::ShowMessage("Please enter your name.", nullptr);
			}
			else if (strServerAddress.empty())
			{
				Utils::ShowMessage("Please enter server address.", nullptr);
			}
			else if (!Utils::StringIsIpAddress(strServerAddress))
			{
				Utils::ShowMessage("Server address is invalid.", nullptr);
			}
			else
			{
				m_pGui->add(m_spConnectingCanvas);
				m_bIsConnecting = true;
				m_bShouldConnect = true;
			}
		});
		m_pGui->add(m_spPlayButton);

		// Add UI elements for entering server address

		m_spServerAddressTextField = tgui::EditBox::create();
		m_spServerAddressTextField->setRenderer(theme.getRenderer("EditBox"));
		m_spServerAddressTextField->setPosition((window.getSize().x - TEXTFIELD_WIDTH) / 2, m_spPlayButton->getPosition().y - TEXTFIELD_HEIGHT - WINDOW_ITEM_SPACING - 10.0f);
		m_spServerAddressTextField->setSize(TEXTFIELD_WIDTH, TEXTFIELD_HEIGHT);
		m_spServerAddressTextField->setText(DEFAULT_IP_ADDRESS);
		m_spServerAddressTextField->setTextSize(TEXT_SIZE);
		m_spServerAddressTextField->setAlignment(tgui::EditBox::Alignment::Center);
		m_pGui->add(m_spServerAddressTextField);

		label = tgui::Label::create();
		label->setRenderer(theme.getRenderer("Label"));
		label->setText("Server Address");
		label->setTextSize(TEXT_SIZE);
		label->setPosition((window.getSize().x - label->getSize().x) / 2, m_spServerAddressTextField->getPosition().y - label->getSize().y - WINDOW_ITEM_SPACING);
		m_pGui->add(label);

		// Add label for character selection

		fTopSpace = m_spBlueSlider->getPosition().y + SLIDER_HEIGHT + WINDOW_ITEM_SPACING + 10.0f;
		float fCanvasHeight = label->getPosition().y - WINDOW_ITEM_SPACING - 8.0f;

		label = tgui::Label::create();
		label->setRenderer(theme.getRenderer("Label"));
		label->setText("Select Character");
		label->setTextSize(TEXT_SIZE);
		label->setPosition((window.getSize().x - label->getSize().x) / 2, fTopSpace);
		m_pGui->add(label);

		// Create canvas for girl character

		fTopSpace += label->getSize().y + WINDOW_ITEM_SPACING;
		fCanvasHeight -= fTopSpace;

		m_spGirlCanvas = tgui::Canvas::create({CANVAS_WIDTH, fCanvasHeight});
		m_spGirlCanvas->setPosition((window.getSize().x - (CANVAS_WIDTH * 2) - WINDOW_ITEM_SPACING) / 2, fTopSpace);
		m_spGirlCanvas->connect("Clicked", [&]()
		{
			m_characterType = CharacterType::Girl;
		});
		m_pGui->add(m_spGirlCanvas);

		m_girlTexture.loadFromFile(RESOURCE_PATH_GIRL);

		m_girlAnimation.setSpriteSheet(m_girlTexture);
		m_girlAnimation.addFrame(sf::IntRect(CHARACTER_FRAME_LENGTH, 0, CHARACTER_FRAME_LENGTH, CHARACTER_FRAME_LENGTH));
		m_girlAnimation.addFrame(sf::IntRect((CHARACTER_FRAME_LENGTH * 2), 0, CHARACTER_FRAME_LENGTH, CHARACTER_FRAME_LENGTH));
		m_girlAnimation.addFrame(sf::IntRect(CHARACTER_FRAME_LENGTH, 0, CHARACTER_FRAME_LENGTH, CHARACTER_FRAME_LENGTH));
		m_girlAnimation.addFrame(sf::IntRect(0, 0, CHARACTER_FRAME_LENGTH, CHARACTER_FRAME_LENGTH));

		float fSpritePositionX = (CANVAS_WIDTH - (CHARACTER_FRAME_LENGTH * CHARACTER_SCALE_LARGE)) / 2;
		float fSpritePositionY = ((fCanvasHeight - (CHARACTER_FRAME_LENGTH * CHARACTER_SCALE_LARGE)) / 2) + 40.0;

		m_girlAnimatedSprite.setPosition(sf::Vector2f(fSpritePositionX, fSpritePositionY));
		m_girlAnimatedSprite.setScale(sf::Vector2f(CHARACTER_SCALE_LARGE, CHARACTER_SCALE_LARGE));
		m_girlAnimatedSprite.play(m_girlAnimation);

		float fNameTagWidth = CANVAS_WIDTH * 0.75f;
		float fNameTagHeight = 40.0f;
		float fNameTagPositionX = (CANVAS_WIDTH - fNameTagWidth) / 2;
		float fNameTagPositionY = m_girlAnimatedSprite.getPosition().y - fNameTagHeight - 20.0f;

		m_girlNameRect.setPosition(fNameTagPositionX, fNameTagPositionY);
		m_girlNameRect.setSize(sf::Vector2f(fNameTagWidth, fNameTagHeight));
		m_girlNameRect.setFillColor(sf::Color(m_color.r, m_color.g, m_color.b, NAMETAG_OPACITY * 255));
		m_girlNameRect.setOutlineColor(m_color);
		m_girlNameRect.setOutlineThickness(OUTLINE_THICKNESS);

		m_font.loadFromFile(RESOURCE_PATH_FONT);

		m_girlNameText.setString(DEFAULT_NAME);
		m_girlNameText.setFillColor(sf::Color::White);
		m_girlNameText.setFont(m_font);
		m_girlNameText.setCharacterSize(TEXT_SIZE - 2);

		float fNamePositionX = fNameTagPositionX + ((fNameTagWidth - m_girlNameText.getLocalBounds().width) / 2);
		float fNamePositionY = fNameTagPositionY + ((fNameTagHeight - m_girlNameText.getLocalBounds().height) / 2) - 3.0f;
		m_girlNameText.setPosition(fNamePositionX, fNamePositionY);

		m_girlSelectedOutline.setPosition(2.0f, 2.0f);
		m_girlSelectedOutline.setSize(sf::Vector2f(CANVAS_WIDTH - (OUTLINE_THICKNESS * 2), fCanvasHeight - (OUTLINE_THICKNESS * 2)));
		m_girlSelectedOutline.setFillColor(sf::Color(0, 0, 0, 0));
		m_girlSelectedOutline.setOutlineColor(COLOR_PINK);
		m_girlSelectedOutline.setOutlineThickness(OUTLINE_THICKNESS);

		// Create canvas for boy character

		m_spBoyCanvas = tgui::Canvas::create({CANVAS_WIDTH, fCanvasHeight});
		m_spBoyCanvas->setPosition(m_spGirlCanvas->getPosition().x + CANVAS_WIDTH + WINDOW_ITEM_SPACING, fTopSpace);
		m_spBoyCanvas->connect("Clicked", [&]()
		{
			m_characterType = CharacterType::Boy;
		});
		m_pGui->add(m_spBoyCanvas);

		m_boyTexture.loadFromFile(RESOURCE_PATH_BOY);

		m_boyAnimation.setSpriteSheet(m_boyTexture);
		m_boyAnimation.addFrame(sf::IntRect(CHARACTER_FRAME_LENGTH, 0, CHARACTER_FRAME_LENGTH, CHARACTER_FRAME_LENGTH));
		m_boyAnimation.addFrame(sf::IntRect((CHARACTER_FRAME_LENGTH * 2), 0, CHARACTER_FRAME_LENGTH, CHARACTER_FRAME_LENGTH));
		m_boyAnimation.addFrame(sf::IntRect(CHARACTER_FRAME_LENGTH, 0, CHARACTER_FRAME_LENGTH, CHARACTER_FRAME_LENGTH));
		m_boyAnimation.addFrame(sf::IntRect(0, 0, CHARACTER_FRAME_LENGTH, CHARACTER_FRAME_LENGTH));

		m_boyAnimatedSprite.setPosition(sf::Vector2f(fSpritePositionX, fSpritePositionY));
		m_boyAnimatedSprite.setScale(sf::Vector2f(CHARACTER_SCALE_LARGE, CHARACTER_SCALE_LARGE));
		m_boyAnimatedSprite.play(m_boyAnimation);

		m_boyNameRect.setPosition(fNameTagPositionX, fNameTagPositionY);
		m_boyNameRect.setSize(sf::Vector2f(fNameTagWidth, fNameTagHeight));
		m_boyNameRect.setFillColor(sf::Color(m_color.r, m_color.g, m_color.b, NAMETAG_OPACITY * 255));
		m_boyNameRect.setOutlineColor(m_color);
		m_boyNameRect.setOutlineThickness(OUTLINE_THICKNESS);

		m_boyNameText.setString(DEFAULT_NAME);
		m_boyNameText.setFillColor(sf::Color::White);
		m_boyNameText.setFont(m_font);
		m_boyNameText.setCharacterSize(TEXT_SIZE - 2);
		m_boyNameText.setPosition(fNamePositionX, fNamePositionY);

		m_boySelectedOutline.setPosition(2.0f, 2.0f);
		m_boySelectedOutline.setSize(sf::Vector2f(CANVAS_WIDTH - (OUTLINE_THICKNESS * 2), fCanvasHeight - (OUTLINE_THICKNESS * 2)));
		m_boySelectedOutline.setFillColor(sf::Color(0, 0, 0, 0));
		m_boySelectedOutline.setOutlineColor(COLOR_PINK);
		m_boySelectedOutline.setOutlineThickness(OUTLINE_THICKNESS);

		// Create canvas for loading

		m_spConnectingCanvas = tgui::Canvas::create({m_pWindow->getSize().x, m_pWindow->getSize().y});
		m_spConnectingCanvas->setPosition(0.0f, 0.0f);

		m_connectingText.setString(CONNECTING_TEXT);
		m_connectingText.setFillColor(COLOR_PINK);
		m_connectingText.setFont(m_font);
		m_connectingText.setCharacterSize(40);
		m_connectingText.setPosition((m_pWindow->getSize().x - m_connectingText.getLocalBounds().width) / 2, ((m_pWindow->getSize().y - m_connectingText.getLocalBounds().height) / 2) - 30.0f);

		// Pass resources to network manager for initializing other players
		m_pNetwork->SetFont(m_font);
		m_pNetwork->SetTextures(m_girlTexture, m_boyTexture);
	}
	catch (const tgui::Exception& e)
	{
		Utils::ShowMessage("Failed to initialize welcome screen.", e.what());
		return false;
	}

	return true;
}

void WelcomeScreen::SetPlayer(Player &player)
{
	m_player = &player;
	m_player->SetFont(m_font);
}

#pragma endregion

#pragma region Run

int WelcomeScreen::Run()
{
	// Initialize time
	sf::Clock frameClock;

	// Run welcome loop
	while (m_pWindow->isOpen())
	{
		// Handle network events
		if (m_bShouldConnect)
		{
			m_bShouldConnect = false;
			m_pNetwork->Connect(sf::IpAddress(m_spServerAddressTextField->getText()));
		}
		else if (m_pNetwork->IsConnected())
		{
			// Go to game screen

			m_pGui->remove(m_spConnectingCanvas);
			m_bIsConnecting = false;

			if (m_characterType == CharacterType::Girl)
			{
				m_player->SetAttributes(m_strName, m_color, m_characterType, m_girlTexture);
			}
			else
			{
				m_player->SetAttributes(m_strName, m_color, m_characterType, m_boyTexture);
			}

			return ScreenIndex::GameScreenIndex;
		}
		else
		{
			HandleConnectionErrors();
		}

		// Handle window events
		sf::Event event;
		while (m_pWindow->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				m_pWindow->close();
			}

			m_pGui->handleEvent(event);
		}

		// Update time
		sf::Time frameTime = frameClock.restart();

		// Animate characters
		if (m_characterType == CharacterType::Girl)
		{
			m_boyAnimatedSprite.stop();
			m_girlAnimatedSprite.play(m_girlAnimation);
			m_girlAnimatedSprite.update(frameTime);
		}
		else
		{
			m_girlAnimatedSprite.stop();
			m_boyAnimatedSprite.play(m_boyAnimation);
			m_boyAnimatedSprite.update(frameTime);
		}

		// Draw
		m_pWindow->clear(COLOR_BACKGROUND);
		Draw();
		m_pWindow->display();
	}

	return -1;
}

void WelcomeScreen::HandleConnectionErrors()
{
	m_pGui->remove(m_spConnectingCanvas);
	m_bIsConnecting = false;

	if (m_pNetwork->ConnectionFailed())
	{
		Utils::ShowMessage("Failed to connect to server.", nullptr);
		m_pNetwork->ResetConnectionFailed();
	}
	else if (m_pNetwork->ReceiveTimedOut())
	{
		Utils::ShowMessage("Timed out. Didn't receive player id and players list.", nullptr);
		m_pNetwork->ResetReceiveTimedOut();
	}
	else if (m_pNetwork->ConnectionRefused())
	{
		Utils::ShowMessage("Can't join game. Maximum players reached.", nullptr);
		m_pNetwork->ResetConnectionRefused();
	}
}

#pragma endregion

#pragma region Update/Draw

void WelcomeScreen::UpdateName()
{
	m_girlNameText.setString(m_strName);
	m_boyNameText.setString(m_strName);

	float fNamePositionX = m_girlNameRect.getPosition().x + ((m_girlNameRect.getSize().x - m_girlNameText.getLocalBounds().width) / 2);
	float fNamePositionY = m_girlNameRect.getPosition().y + ((m_girlNameRect.getSize().y - m_girlNameText.getLocalBounds().height) / 2) - 3.0f;
	
	m_girlNameText.setPosition(fNamePositionX, fNamePositionY);
	m_boyNameText.setPosition(fNamePositionX, fNamePositionY);
}

void WelcomeScreen::UpdateNameTagColor()
{
	m_girlNameRect.setFillColor(sf::Color(m_color.r, m_color.g, m_color.b, NAMETAG_OPACITY * 255));
	m_girlNameRect.setOutlineColor(m_color);
	m_boyNameRect.setFillColor(sf::Color(m_color.r, m_color.g, m_color.b, NAMETAG_OPACITY * 255));
	m_boyNameRect.setOutlineColor(m_color);
}

void WelcomeScreen::Draw()
{
	m_spColorCanvas->clear(m_color);
	m_spColorCanvas->display();

	m_spGirlCanvas->clear(COLOR_DARK_BACKGROUND);
	m_spGirlCanvas->draw(m_girlAnimatedSprite);
	m_spGirlCanvas->display();

	m_spBoyCanvas->clear(COLOR_DARK_BACKGROUND);
	m_spBoyCanvas->draw(m_boyAnimatedSprite);
	m_spBoyCanvas->display();

	if (m_characterType == CharacterType::Girl)
	{
		m_spGirlCanvas->draw(m_girlNameRect);
		m_spGirlCanvas->draw(m_girlNameText);
		m_spGirlCanvas->draw(m_girlSelectedOutline);
	}
	else 
	{
		m_spBoyCanvas->draw(m_boyNameRect);
		m_spBoyCanvas->draw(m_boyNameText);
		m_spBoyCanvas->draw(m_boySelectedOutline);
	}
	
	if (m_bIsConnecting)
	{
		m_spConnectingCanvas->clear(sf::Color(0, 0, 0, 0.8 * 255));
		m_spConnectingCanvas->draw(m_connectingText);
		m_spConnectingCanvas->display();
	}

	m_pGui->draw();
}

#pragma endregion
