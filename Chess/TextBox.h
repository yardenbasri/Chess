#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>

#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27

class TextBox
{
public:
	TextBox();
	TextBox(int size, sf::Color color, bool sel);
	void SetFont(sf::Font& font);
	void SetPosition(sf::Vector2f pos);
	void SetLimit(bool hasLimit);
	void SetLimit(bool hasLimit, int limit);
	void SetSelected(bool selected);

	std::string GetText();
	void DrawTo(sf::RenderWindow& window);
	void TypedOn(sf::Event input);
private:
	sf::Text textbox;
	std::ostringstream text;
	bool isSelected;
	bool hasLimit;
	int limit;
	void inputLogic(int charTyped);
	void deleteLastCharacter();
};

